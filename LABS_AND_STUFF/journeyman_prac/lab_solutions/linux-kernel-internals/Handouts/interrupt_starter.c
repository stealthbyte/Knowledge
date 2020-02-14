#include <linux/module.h>
#include <linux/interrupt.h>
#include <linux/workqueue.h>
#include <asm-generic/atomic-long.h>

#define ACTP_IRQ    0x1

static char* welcome_msg = "Interrupt fun";
static volatile int g_counter;
static spinlock_t g_lock;

irqreturn_t int_handler(int irq_nr, void* dev_id);
void print_work_handler(struct work_struct* work);

DECLARE_WORK(print_work, print_work_handler);

static int __init mod_start(void) {
    int err;
    spin_lock_init(&g_lock);
	printk(KERN_ALERT "%s\n", welcome_msg);
    err = request_irq(ACTP_IRQ, int_handler, IRQF_SHARED, "test", (void*)&g_counter);
    if (err != 0) {
        printk(KERN_ALERT "Failed to request irq: %d\n", err);
        return err;
    }
	return 0;
}


static void __exit mod_stop(void) {
    free_irq(ACTP_IRQ, (void*)&g_counter);
    printk(KERN_ALERT "Handler invoked %d times\n", g_counter);
	printk(KERN_ALERT "Module Exiting\n");
	return;
}


MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark O'Neill");
MODULE_DESCRIPTION("Interrupt Handler Demo");
module_init(mod_start);
module_exit(mod_stop);