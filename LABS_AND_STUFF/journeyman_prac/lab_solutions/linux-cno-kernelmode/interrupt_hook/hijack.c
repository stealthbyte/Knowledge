#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/slab.h>
#include <asm/desc.h>
#include <linux/moduleparam.h>

typedef void (*idt_func_t)(void);

static int g_divzero_count;

struct desc_ptr orig_idt_addr;
gate_desc* orig_idt;

struct desc_ptr new_idt_addr;
gate_desc* new_idt;

static unsigned long addr_divide_error;
static unsigned long addr_do_divide_error;
static unsigned long addr_error_entry;
static unsigned long addr_error_exit;

MODULE_LICENSE("GPL");
static void compat_load_idt(void* info);

typedef void (*do_divide_error_t)(struct pt_regs*, long);

void new_do_divide_error(struct pt_regs* regs, long err) {
	do_divide_error_t orig_func;
	printk(KERN_INFO "Hello world\n");
	orig_func = (do_divide_error_t)addr_do_divide_error;
	orig_func(regs, err);
}

module_param(addr_divide_error, ulong, S_IRUGO);
module_param(addr_do_divide_error, ulong, S_IRUGO);
module_param(addr_error_entry, ulong, S_IRUGO);
module_param(addr_error_exit, ulong, S_IRUGO);

asmlinkage void new_divide_error(void);
asm("   .text");
asm("   .type new_divide_error,@function");
asm("new_divide_error:");
asm("nop");
asm("nop");
asm("nop");
asm("pushq $0xffffffffffffffff");
asm("callq *addr_error_entry");
asm("mov %rsp, %rdi");
asm("xor %esi, %esi");
asm("callq new_do_divide_error");
//asm("callq *addr_do_divide_error");
asm("jmpq *addr_error_exit");

/*asm("   nopl (%rax)");
asm("   pushq $0xffffffffffffffff");
asm("   callq *addr_error_entry");
asm("   mov %rsp,%rdi");
asm("   xor %esi,%esi");
//asm("   callq do_new_divide_error");
asm("	callq *addr_divide_error");
asm("   jmpq *addr_error_exit");*/

void print_idt(gate_desc* idt) {
	int i;
	printk(KERN_INFO "IDT is\n");
	for (i = 0; i < 256; i++) {
		printk(KERN_INFO "\t[%d] %08lx\n", 
			i,
			gate_offset(&idt[i]));
	}
	return;
}

static int __init mod_init(void) {
	/*unsigned long __IDT_page;
	struct desc_ptr newidtr;
	struct desc_ptr __IDT_register;
	gate_desc *oldidt, *newidt;*/

	printk(KERN_INFO "addr_divide_error is %016lx\n", addr_divide_error);
	printk(KERN_INFO "addr_do_divide_error is %016lx\n", addr_do_divide_error);
	printk(KERN_INFO "addr_error_entry is %016lx\n", addr_error_entry);
	printk(KERN_INFO "addr_error_exit is %016lx\n", addr_error_exit);


	/*store_idt(&__IDT_register);
	oldidt = (gate_desc *)__IDT_register.address;
	printk(KERN_INFO "Current IDT at %p\n", oldidt);

	__IDT_page =__get_free_page(GFP_KERNEL);
	if(!__IDT_page) {
		return -1;
	}
	printk(KERN_INFO "new ID will be at %p\n", __IDT_page);
	printk(KERN_INFO "new_divide_error is %p\n", new_divide_error);
	newidtr.address = __IDT_page;
	newidtr.size = __IDT_register.size;
	newidt = (gate_desc *)newidtr.address;
	memcpy(newidt, oldidt, __IDT_register.size);
	print_idt(oldidt);
	pack_gate(&newidt[0], 
		GATE_INTERRUPT, 
		(unsigned long)new_divide_error, 
		0 ,0, __KERNEL_CS);
	print_idt(newidt);
	printk(KERN_INFO "uhhh\n");
	load_idt((void *)&newidtr);*/

	/*printk(KERN_INFO "old %d %p\n", __IDT_register.size, __IDT_register.address);
	printk(KERN_INFO "new %d %p\n", newidtr.size, newidtr.address);*/

	g_divzero_count = 0;
	printk(KERN_INFO "Module loaded\n");

	store_idt(&orig_idt_addr);
	orig_idt = (gate_desc*)orig_idt_addr.address;

	new_idt = (gate_desc*)__get_free_page(GFP_KERNEL);
	if (new_idt == NULL) {
		printk(KERN_INFO "Failed to allocate page\n");
		return -1;
	}
	new_idt_addr.address = (unsigned long)new_idt;
	new_idt_addr.size = orig_idt_addr.size;

	print_idt(orig_idt);
	memcpy(new_idt, orig_idt, new_idt_addr.size);
	pack_gate(&new_idt[0], 
		GATE_INTERRUPT, 
		(unsigned long)new_divide_error, 
		0 ,0, __KERNEL_CS);
	print_idt(new_idt);
	load_idt(&new_idt_addr);
	smp_call_function(compat_load_idt, (void*)&new_idt_addr, 1);

	return 0;
}

static void __exit mod_exit(void) {
	printk(KERN_INFO "Divide by zero invoked %d times\n", g_divzero_count);

	load_idt(&orig_idt_addr);
	smp_call_function(compat_load_idt, (void*)&orig_idt_addr, 1);
	if (new_idt != NULL) {
		free_page((unsigned long)new_idt);
	}

	return;
}

static void compat_load_idt(void* info) {
	load_idt((struct desc_ptr*)info);
	return;
}

module_init(mod_init);
module_exit(mod_exit);
