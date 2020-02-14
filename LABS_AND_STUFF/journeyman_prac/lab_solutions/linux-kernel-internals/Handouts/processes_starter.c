#include <linux/module.h>
#include <linux/sched.h>
#include <linux/kernel.h>
#include <linux/sched.h>
#include <linux/init.h>
#include <linux/init_task.h>
#include <asm/processor.h>
#include <asm/ptrace.h>
#include <linux/sched/signal.h>

static void list_children(struct task_struct* task, int tab);

static int __init mod_start(void) {
	printk(KERN_ALERT "Hello, World\n");
	list_children(&init_task, 0);
	return 0;
}

static void __exit mod_stop(void) {
	printk(KERN_ALERT "Goodbye, World\n");
	return;
}


static void list_children(struct task_struct* task, int tab) {
	static char tab_buf[] = "\t\t\t\t\t\t\t\t\t\t\t\t\t";
	struct task_struct* child;
	struct task_struct* thread;
	struct list_head* tmp;
	struct list_head* list;
	
	printk(KERN_ALERT "%.*s%s [PID: %d TID %p threads: %d STATE: %c]\n",
		tab, 
		tab_buf,
	       	task->comm,
	       	task->pid,
	       	task->thread_pid,
		get_nr_threads(task)
	);
	
	
	printk(KERN_ALERT "%.*s-----------------------------------------\n", tab, tab_buf);
	list_for_each_safe(list, tmp, &task->children) {
		child = list_entry(list, struct task_struct, sibling);
		list_children(child, tab+1);
	}
	return;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("AAA");
MODULE_DESCRIPTION("YYY");
module_init(mod_start);
module_exit(mod_stop); 
