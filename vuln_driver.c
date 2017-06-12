#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/fs.h>
#include <linux/init.h>
#include <linux/miscdevice.h>
#include <linux/slab.h>
#include <linux/ioctl.h>
#include <linux/uaccess.h>

#include "vuln_driver.h"
#include "buffer_overflow.h"
#include "null_pointer_deref.h"
#include "use_after_free.h"


static long do_ioctl(struct file *filp, unsigned int cmd, unsigned long args)
{
	switch(cmd) {
		case DRIVER_TEST:
			printk(KERN_WARNING "[x] Talking to device [x]\n");
			break;
		case BUFFER_OVERFLOW:
			buffer_overflow((char *) args);
			break;
		case NULL_POINTER_DEREF:
			null_pointer_deref(args);
			break;
		case ALLOC_UAF_OBJ:
			alloc_uaf_obj();
			break;
		case USE_UAF_OBJ:
			use_uaf_obj();
			break;
		case ALLOC_K_OBJ:
			alloc_k_obj((k_object *) args);
			break;
		case FREE_UAF_OBJ:
			free_uaf_obj();
			break;
	}

	return 0;
}

/**
* The operations allowed by userspace applications.
* We only really allow access through the ioctl interface.
*/
static struct file_operations vuln_ops = {
	.owner = THIS_MODULE,
	.unlocked_ioctl = do_ioctl,
};

/**
* The miscdevice api is much simpler than creating a class
* and attaching it to a node using class_create etc.
*/
static struct miscdevice vuln_device = {
	MISC_DYNAMIC_MINOR, DEVICE_NAME, &vuln_ops
};

/**
* Register the device.
*/
static int vuln_module_init(void)
{
	int ret;

	ret = misc_register(&vuln_device);

	if(ret < 0) {
		printk(KERN_WARNING "[-] Error registering device [-]\n");
	}

	return ret;
}

/**
* Deregister the device.
*/
static void vuln_module_exit(void)
{
	misc_deregister(&vuln_device);
}

module_init(vuln_module_init);
module_exit(vuln_module_exit);
MODULE_LICENSE("GPL");
MODULE_AUTHOR("Invictus");
MODULE_DESCRIPTION("");
