#include <linux/netfilter.h>
#include <linux/netfilter_ipv4.h>
#include <linux/net.h>
#include <linux/in.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/tcp.h>

// For TCP packets (iph->protocol == IPPROTO_TCP)
#define INCOMING_TCPH(x)	((struct tcphdr*)((__u32*)x + x->ihl))
// For UDP packets (iph->protocol == IPPROTO_UDP)
#define INCOMING UDPH(x)	((struct udphdr*)((__u32*)x + x->ihl))

static unsigned int hook(void* priv, struct sk_buff* skb, const struct nf_hook_state* state);

static struct nf_hook_ops hook_ops = {
	.hook        = hook,
	.hooknum     = NF_INET_LOCAL_IN,
	.pf          = NFPROTO_IPV4,
	.priority    = NF_IP_PRI_FIRST
};

unsigned int hook(void* priv, struct sk_buff* skb, const struct nf_hook_state* state) {


}


static int __init mod_start(void) {
	return nf_register_net_hook(&init_net, &hook_ops);
}

static void __exit mod_stop(void) {
	nf_unregister_net_hook(&init_net, &hook_ops);
	printk(KERN_ALERT "Goodbye, World\n");
	return;
}

MODULE_LICENSE("GPL");
MODULE_AUTHOR("Mark O'Neill");
MODULE_DESCRIPTION("Netfilter Lab Solution");
module_init(mod_start);
module_exit(mod_stop); 