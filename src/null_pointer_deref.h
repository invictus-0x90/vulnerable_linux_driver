#ifndef _NULL_POINTER_DEREF_
	#define _NULL_POINTER_DEREF_
	
	/**
	* a simple callback function to test you are talking
	* to this vuln properly.
	*/
	static void callback(void)
	{
		printk(KERN_WARNING "[x] Hit callback [x]\n");
	}

	/**
	* Pretty obvious null pointer dereference. If the user
	* supplied argument != 0xdeadbeef then fp() calls a null
	* pointer.
	*/
	static void null_pointer_deref(unsigned long arg)
	{
		void (*fp)(void) = NULL;

		if((arg ^ 0xdeadbeef) == 0) {
			fp = callback;
		}

		fp();
	}
#endif
