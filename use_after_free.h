/**
*	This file includes the relevant methods and objects to trigger the 
*	use after free vulnerability. The available methods to userland applications
*	allow a uaf_object with a function pointer to be allocated, a trash object
*	with 256 bytes of data to be allocated, the uaf_obj to be freed, and the uaf_obj
*	to be used.
*/

#ifndef _USE_AFTER_FREE_
	#define _USE_AFTER_FREE_

	typedef struct uaf_obj
	{
		void (*fn)(void);
		char buff[128];
	}uaf_obj;

	typedef struct k_object
	{
		char buff[256];
	}k_object;

	/* this is our global uaf object that will eventually be freed and used */
	uaf_obj *global_uaf_obj = NULL;

	/**
	* A simple callback function
	*/
	static void uaf_callback(void)
	{
		printk(KERN_WARNING "[-] Hit callback [-]\n");
	}

	/**
	* Allocate a use after free object on the kernel heap.
	* This objects buffer is then filled with A's, and the 
	* global uaf pointer is set to it.
	*/
	static int alloc_uaf_obj(void)
	{
		struct uaf_obj *target;

		target = kmalloc(sizeof(uaf_obj), GFP_KERNEL);

		if(!target) {
			printk(KERN_WARNING "[-] Error no memory [-]\n");
			return -ENOMEM;
		}

		target->fn = uaf_callback;
		memset(target->buff, 0x41, sizeof(target->buff));

		global_uaf_obj = target;

		return 0;
	}

	/**
	* Use the function pointer callback as long as its not null.
	*/
	static void use_uaf_obj(void)
	{
		if(global_uaf_obj->fn)
		{
			printk(KERN_WARNING "[x] Calling %p [x]\n", global_uaf_obj->fn);
			global_uaf_obj->fn();
		}
	}

	static int alloc_k_obj(k_object *user_kobj)
	{
		k_object *trash_object = kmalloc(sizeof(k_object), GFP_KERNEL);

		if(!trash_object) {
			printk(KERN_WARNING "[x] Error allocating k_object memory [-]\n");
			return -ENOMEM;
		}

		copy_from_user(trash_object, user_kobj, sizeof(k_object));

		//printk(KERN_WARNING "%s\n", trash_object->buff);
		return 0;
	}

#endif
