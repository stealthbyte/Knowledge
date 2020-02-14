#include <linux/mm_types.h>
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/string.h>
#include <linux/sched/signal.h>
#include <linux/gfp.h>
#include <linux/mm.h>
#include <asm/page.h>

static int __init mod_start(void) {
	struct task_struct* shared_mem_tasks[2];
	struct task_struct* p;
	int tasks_found;

	printk(KERN_ALERT "Starting module\n");
	tasks_found = 0;
	for_each_process(p) {
		if (strstr(p->comm, "shareit") != NULL) {
			shared_mem_tasks[tasks_found] = p;
			tasks_found++;
		}
		if (tasks_found >= 2) {
			break;
		}
	}
	printk(KERN_ALERT "Found %d tasks\n", tasks_found);
	if (tasks_found != 2) {
		return 0;
	}

	if (shared_mem_tasks[0]->mm == NULL || shared_mem_tasks[1]->mm == NULL) {
		printk(KERN_ALERT "Some tasks do not have an mm!\n");
		return 0;
	}

	printk(KERN_ALERT "[0] %s's pgd is %#016llx\n", shared_mem_tasks[0]->comm,
		(u64)shared_mem_tasks[0]->mm->pgd);
	printk(KERN_ALERT "[1] %s's pgd is %#016llx\n", shared_mem_tasks[1]->comm,
		(u64)shared_mem_tasks[1]->mm->pgd);

	return 0;
}

static void __exit mod_stop(void) {
	printk(KERN_ALERT "Exiting module\n");
	return;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark O'Neill");
MODULE_DESCRIPTION("Demo for PGD Question");
module_init(mod_start);
module_exit(mod_stop);
