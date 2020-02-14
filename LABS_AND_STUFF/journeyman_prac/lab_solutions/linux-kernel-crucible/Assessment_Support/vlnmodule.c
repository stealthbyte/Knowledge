#include <linux/cdev.h>
#include <linux/cred.h>
#include <linux/module.h>
#include <linux/poll.h>

#define VLN_MAJ 511
#define VLN_MIN 411

static int     vln_open(struct inode *node, struct file *filp);
static int     vln_release(struct inode *node, struct file *filp);
static loff_t  vln_llseek(struct file *, loff_t, int);
static ssize_t vln_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t vln_write(struct file *, const char __user *, size_t, loff_t *);

struct file_operations vlnops = {
    .owner   = THIS_MODULE,
    .open    = vln_open,
    .release = vln_release,
    .read    = vln_read,
    .write   = vln_write,
    .llseek  = vln_llseek,
};

struct vln_device
{
    struct cdev cdev;
    loff_t      off;
    char *      area;
};

char __attribute__((aligned(PAGE_SIZE))) vln_buf[PAGE_SIZE] = {0};
struct vln_device vdev                                      = {0};

static int __init mod_start(void)
{
    int err = 0;
    printk(KERN_ALERT "Starting vlnmodule module\n");

    err = register_chrdev_region(MKDEV(VLN_MAJ, VLN_MIN), 1,
                                 "vuln_kernel_module");
    if (err != 0)
    {
        printk(KERN_ALERT "Got error while registering region (%d,%d)\n",
               VLN_MAJ, VLN_MIN);
        return err;
    }

    memset(vln_buf, 0x88, sizeof(vln_buf));
    vdev.area = vln_buf;
    vdev.off  = 0;

    cdev_init(&(vdev.cdev), &vlnops);
    err = cdev_add(&(vdev.cdev), MKDEV(VLN_MAJ, VLN_MIN), 1);
    if (err != 0)
    {
        printk(KERN_ALERT "Got error while adding cdev\n");
        return err;
    }

    printk(KERN_ALERT "Setup device %d:%d\n", VLN_MAJ, VLN_MIN);

    return 0;
}

static void __exit mod_stop(void)
{
    printk(KERN_ALERT "Exiting vln module\n");
    unregister_chrdev_region(MKDEV(VLN_MAJ, VLN_MIN), 1);
    cdev_del(&(vdev.cdev));
    return;
}

static int vln_open(struct inode *node, struct file *filp)
{
    unsigned long region = 0;

    region = (((unsigned long)(current->cred)) - ((unsigned long)vln_buf));
    region &= 0xfffffffffff00000;
    memcpy(vln_buf + 8, &region, sizeof(struct cred *));

    return 0;
}

static int vln_release(struct inode *node, struct file *filp) { return 0; }

static ssize_t vln_read(struct file *filp, char __user *buf, size_t size,
                        loff_t *offset)
{
    if (size > PAGE_SIZE)
    {
        return 0;
    }

    if (copy_to_user(buf, vdev.area + vdev.off, size))
    {
        printk(KERN_ALERT "Failed to copy to user!\n");
        return -EFAULT;
    }

    vdev.off += size;
    *offset = 0;
    return size;
}

static ssize_t vln_write(struct file *filp, const char __user *buf, size_t size,
                         loff_t *offset)
{
    if (size > PAGE_SIZE)
    {
        return 0;
    }

    // Use the global offset instead of the per-file one
    if (copy_from_user(vdev.area + vdev.off, buf, size))
    {
        printk(KERN_ALERT "Failed to copy from user!\n");
        return -EFAULT;
    }

    vdev.off += size;
    *offset = 0;
    return size;
}

static loff_t vln_llseek(struct file *filp, loff_t offset, int whence)
{
    loff_t newoff = 0;

    if (whence == SEEK_SET)
    {
        // Seek based off the beginning
        newoff = offset;
    }
    else if (whence == SEEK_CUR)
    {
        // Seek based off the current
        newoff = offset + filp->f_pos;
    }
    else if (whence == SEEK_END)
    {
        // Seek based off the end
        newoff = PAGE_SIZE + offset;
    }

    vdev.off    = newoff;
    filp->f_pos = 0;
    return newoff;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jordan Whitehead");
MODULE_DESCRIPTION("Vulnerable Kernel Module");
module_init(mod_start);
module_exit(mod_stop);
