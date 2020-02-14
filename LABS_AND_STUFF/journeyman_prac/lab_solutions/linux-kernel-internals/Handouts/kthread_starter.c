#include <linux/sched/signal.h>
#include <linux/rcupdate.h>
#include <linux/kthread.h>
#include <linux/module.h>
#include <linux/string.h>
#include <linux/dcache.h>
#include <linux/delay.h>
#include <linux/sched.h>

struct task_struct* g_kthread;

static int YOUR_KTHREAD_FUNC_NAME(void* data);


static int kthread_start(void* data) {

    func_to_check_to_send_sig_to_kill_process

}

int func_to_check_to_send_sig_to_kill_process(//TODO FILL IN SO IT CAN DYNAMICALLY TAKE STRING TO SEE WHAT TO KILL)
{
struct task_struct* tsk;
const char* name;
tsk = current;
for_each_process(tsk) {
		// print both comm and the exe_file name
        printk(KERN_ALERT "comm %s\n", tsk->comm);
		rcu_read_lock();
        //TODO FILL IN HOW TO SEND IN SIG TO KILL PROCESS		
		
		rcu_read_unlock();
		
}

static int __init mod_start(void) {
	g_kthread = kthread_run(kthread_start, (void*)5, "FILL IN");
	printk(KERN_ALERT "Kernel thread started\n");
	return 0;
}
