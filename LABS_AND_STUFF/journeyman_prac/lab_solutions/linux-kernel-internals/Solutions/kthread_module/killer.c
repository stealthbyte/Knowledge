#include <linux/sched/signal.h>
#include <linux/rcupdate.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/dcache.h>
#include <linux/delay.h>
#include <linux/sched.h>

struct task_struct* g_kthread;

static int kthread_start(void* data);
static void search_and_destroy(const char* contains);

static int __init mod_start(void) {
	g_kthread = kthread_run(kthread_start, (void*)5, "ACTP Daemon");
	printk(KERN_ALERT "Kernel thread started\n");
	return 0;
}

static void __exit mod_stop(void) {
	kthread_stop(g_kthread);
	printk(KERN_ALERT "Kernel thread terminated\n");
	return;
}

static int kthread_start(void* data) {
	int counter = (int)(long)data;
	while (!kthread_should_stop() && counter > 0) {
		printk(KERN_ALERT "Counter is %d\n", counter);
		counter--;
		ssleep(1);
	}
	while (!kthread_should_stop()) {
        search_and_destroy("killme");

		// could use schdule here to to a real tight loop
		// dont' set current state as interruptible or uninterruptible first though
		// otherwise you will just sleep
		//schedule();

		// instead, because we are printing a bunch, let's just sleep for a second
		ssleep(1);
	}
	printk(KERN_ALERT "Exiting kernel thread...\n");
	return 0;
}

static void search_and_destroy(const char* contains) {
    struct task_struct* tsk;
	const char* name;
    // find and remove any process containing the str
    tsk = current;
    for_each_process(tsk) {
		// print both comm and the exe_file name
        printk(KERN_ALERT "comm %s\n", tsk->comm);
		rcu_read_lock();
        if (
				tsk->mm &&
				tsk->mm->exe_file != NULL &&
				tsk->mm->exe_file->f_path.dentry != NULL &&
				tsk->mm->exe_file->f_path.dentry->d_name.name != NULL
		) {
			name = tsk->mm->exe_file->f_path.dentry->d_name.name;
            printk(KERN_ALERT "exef %s\n", name);
			// use this one to compare
			// don't want to kill anything without a exe_file
			if (strstr(name, contains)) {
				force_sig(SIGKILL, tsk);
			}
        }
		rcu_read_unlock();
    }
}

MODULE_LICENSE("GPL");
module_init(mod_start);
module_exit(mod_stop);
