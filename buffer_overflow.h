#ifndef _DEVICE_BUFFER_OVERFLOW_
        #define _DEVICE_BUFFER_OVERFLOW_

      	//static  int buffer_overflow(char __user *buff);

	static int buffer_overflow(char __user *buff)
        {
                char kernel_buff[512];
                size_t size;

                size = strlen(buff);

                printk(KERN_WARNING "[x] Triggering buffer overflow [x]\n");

                /**
                * Pretty simple buffer overflow. We shouldnt be using memcpy to
                * start with, copy_from_user does bounds checking for us (hence
                * why its not used here).
                */
                memcpy(kernel_buff, buff, size);

                return 0;
	}

#endif
