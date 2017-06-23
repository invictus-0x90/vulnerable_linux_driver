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
#include "arbitrary_rw.h"
#include "uninitialised_stack_var.h"

static long do_ioctl(struct file *filp, unsigned int cmd, unsigned long args)
{
	int ret;
	unsigned long *p_arg = (unsigned long *)args;
 	ret = 0;

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
			alloc_uaf_obj(args);
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
		case ARBITRARY_RW_INIT:
		{
			init_args i_args;
			int ret;

			if(copy_from_user(&i_args, p_arg, sizeof(init_args)))
				return -EINVAL;

			ret = arbitrary_rw_init(&i_args);
			break;
		}
		case ARBITRARY_RW_REALLOC:
		{
			realloc_args r_args;

			if(copy_from_user(&r_args, p_arg, sizeof(realloc_args)))
				return -EINVAL;

			ret = realloc_mem_buffer(&r_args);
			break;
		}
		case ARBITRARY_RW_READ:
		{
			read_args r_args;

			if(copy_from_user(&r_args, p_arg, sizeof(read_args)))
				return -EINVAL;

			ret = read_mem_buffer(r_args.buff, r_args.count);
			break;
		}
		case ARBITRARY_RW_SEEK:
		{
			seek_args s_args;

			if(copy_from_user(&s_args, p_arg, sizeof(seek_args)))
				return -EINVAL;

			ret = seek_mem_buffer(&s_args);
			break;
		}
		case ARBITRARY_RW_WRITE:
		{
			write_args w_args;

			if(copy_from_user(&w_args, p_arg, sizeof(write_args)))
				return -EINVAL;

			ret = write_mem_buffer(&w_args);
			break;
		}
		case UNINITIALISED_STACK_ALLOC:
		{
			ret = copy_to_stack((char *)p_arg);
			break;
		}
		case UNINITIALISED_STACK_USE:
		{
			use_obj_args use_obj_arg;
			/*
			if(copy_from_user(&use_obj_arg, p_arg, sizeof(use_obj_args)))
				return -EINVAL;
			*/
			ret = copy_to_stack((char *)p_arg);
			use_stack_obj(&use_obj_arg);
	
			break;
		}
	}

	return ret;
}

static int vuln_release(struct inode *inode, struct file *filp)
{
	if(g_mem_buffer != NULL)
	{
		if(g_mem_buffer->data != NULL)
			kfree(g_mem_buffer->data);
		kfree(g_mem_buffer);
		g_mem_buffer = NULL;
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
	.release = vuln_release,
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

	printk(KERN_WARNING "[!!!] use_stack_obj @%p [!!!]\n", use_stack_obj);

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
