#include <linux/init.h>
#include <linux/module.h>
#include <linux/kernel.h>

MODULE_LICENSE("GPL");
MODULE_AUTHOR("milin");
MODULE_DESCRIPTION("Hello World Kernel Module");
MODULE_VERSION("0.1");


static int __init hello_init(void)
{
	pr_info("Hello world!\n");
	return 0;
}

static void __exit hello_exit(void)
{
	pr_info("Cleaning hello world! module.\n");
}

module_init(hello_init);
module_exit(hello_exit);

// rule : Be careful—the module must compile on any system. (<– version compatibility hint) : -> pr_info assure la portabilite sur toute les versions recente de kernel :)))))).
