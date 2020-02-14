#include <linux/module.h>
#include <linux/fs.h>
#include <linux/uaccess.h>
#include <linux/cdev.h>
#include <linux/major.h>
#include "xorshared.h"

static int xor_open(struct inode* node, struct file* filp);
static int xor_release(struct inode* node, struct file* filp);
static long xor_ioctl(struct file* filp, unsigned int cmd, unsigned long arg);
loff_t xor_llseek(struct file *, loff_t, int);
static ssize_t xor_read(struct file *, char __user *, size_t, loff_t *);
static ssize_t xor_write(struct file *, const char __user *, size_t, loff_t *);

struct file_operations xorops = {
    .owner = THIS_MODULE,
    .open = xor_open,
    .release = xor_release,
    .unlocked_ioctl = xor_ioctl,
    .llseek = xor_llseek,
    .read = xor_read,
    .write = xor_write,
};

struct xor_device {
    struct cdev cdev;
    char key;
    char xorbuf[XOR_BUF_SZ];
};

struct xor_device devs[XOR_NUMDEV];

static int __init mod_start(void) {
    int err = 0;
    int i;
    printk(KERN_ALERT "Starting xor module\n");

    err = register_chrdev_region(MKDEV(XOR_MAJ, XOR_MIN), XOR_NUMDEV, "xor_device_driver");
    if (err != 0) {
        printk(KERN_ALERT "Got error while registering region (%d,%d)\n", XOR_MAJ, XOR_MIN);
        return err;
    }

    for (i=0; i<XOR_NUMDEV; i++) {
        devs[i].key = DEF_KEY;

        cdev_init(&(devs[i].cdev), &xorops);
        err = cdev_add(&(devs[i].cdev), MKDEV(XOR_MAJ, XOR_MIN+i), 1);
	if (err != 0) {
		printk(KERN_ALERT "Got error while adding cdev %d\n", i);
		return err;
	}
    }

    printk(KERN_ALERT "Setup %d devices from %d:%d to %d:%d\n", XOR_NUMDEV, XOR_MAJ, XOR_MIN, XOR_MAJ, XOR_MIN+XOR_NUMDEV-1);

	return 0;
}

static void __exit mod_stop(void) {
	int i;
	printk(KERN_ALERT "xor module Exiting\n");
	
	unregister_chrdev_region(MKDEV(XOR_MAJ, XOR_MIN), XOR_NUMDEV);

	for (i = 0; i < XOR_NUMDEV; i++) {
		cdev_del(&(devs[i].cdev));
	}

	return;
}

static int xor_open(struct inode* node, struct file* filp) {
    struct xor_device* dev;

    printk(KERN_ALERT "Open got called\n");

    dev = container_of(node->i_cdev, struct xor_device, cdev);

    // set the dev so it is easy to get off the file later
    filp->private_data = dev;

    return 0;
}

static int xor_release(struct inode* node, struct file* filp) {
    printk(KERN_ALERT "Release got called\n");

    return 0;
}

static long xor_ioctl(struct file* filp, unsigned int cmd, unsigned long arg) {
    struct xor_device* dev;

    printk(KERN_ALERT "Got ioctl called with command %u and arg %lu\n", cmd, arg);

    dev = filp->private_data;

    if (cmd == KEYCMD) {
        dev->key = (char)arg;
        printk(KERN_ALERT "Key is now %02X\n", dev->key);
    }

    return 0;
}

loff_t xor_llseek(struct file* filp, loff_t offset, int whence) {
    loff_t newoff = 0;
    if (whence == SEEK_SET) {
        // seek based off the beginning
        newoff = offset;
    } else if (whence == SEEK_CUR) {
        // seek based off the current
        newoff = offset + filp->f_pos;
    } else if (whence == SEEK_END) {
        // seek based off the end
        newoff = XOR_BUF_SZ + offset;
    }

    if (newoff > XOR_BUF_SZ) {
        newoff = XOR_BUF_SZ;
    }

    if (newoff < 0) {
        newoff = 0;
    }

    filp->f_pos = newoff;

    printk(KERN_ALERT "llseek was called, new offset is %lld\n", newoff);

    return newoff;
}

static ssize_t xor_read(struct file* filp, char __user* buf, size_t size, loff_t* offset) {
    struct xor_device* dev;
    ssize_t len = min((ssize_t)(XOR_BUF_SZ - *offset), (ssize_t)size);

    printk(KERN_ALERT "Got read of size %lu\n", size);

    dev = filp->private_data;

    if (len <= 0) {
        return 0;
    }

    if (copy_to_user(buf, dev->xorbuf + *offset, len)) {
        printk(KERN_ALERT "Failed to copy to user!\n");
        return -EFAULT;
    }

    *offset += len;

    return len;
}

static ssize_t xor_write(struct file* filp, const char __user* buf, size_t size, loff_t* offset) {
    int i;
    struct xor_device* dev;
    ssize_t len = min((ssize_t)(XOR_BUF_SZ - *offset), (ssize_t)size);

    printk(KERN_ALERT "Got write of size %lu\n", size);

    dev = filp->private_data;

    if (len <= 0) {
        return 0;
    }

    if (copy_from_user(dev->xorbuf + *offset, buf, len)) {
        printk(KERN_ALERT "Failed to copy from user!\n");
        return -EFAULT;
    }

    // xor it
    for (i = 0; i < len; i++) {
        dev->xorbuf[*offset+i] ^= dev->key;
    }

    *offset += len;

    return len;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Jordan Whitehead");
MODULE_DESCRIPTION("Xor character device");
module_init(mod_start);
module_exit(mod_stop);
