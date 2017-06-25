#ifndef _UNINITIALISED_STACK_VAR
	#define _UNINITIALISED_STACK_VAR
	#define BUFF_SIZE 4096

	typedef struct stack_obj 
	{
		int do_callback;
		long fn_arg;
		void (*fn)(long);
		char buff[48];
	}stack_obj;

	typedef struct use_obj_args
	{
		int option;
		long fn_arg;
	}use_obj_args;
	
	static void uninitialised_callback(long arg)
	{
		printk(KERN_WARNING "[x] Hit callback with arg %lu [x]\n", arg);
	}

	noinline static int copy_to_stack(char __user *user_buff)
	{
		int ret;
		char buff[BUFF_SIZE];

		ret = copy_from_user(buff, user_buff, BUFF_SIZE);
		buff[BUFF_SIZE - 1] = 0;

		return ret;
	}


	noinline static void use_stack_obj(use_obj_args *use_obj_arg)
	{
		volatile stack_obj s_obj;

		if(use_obj_arg->option == 0)
		{
			s_obj.fn = uninitialised_callback;
			s_obj.fn_arg = use_obj_arg->fn_arg;
		}

		s_obj.fn(s_obj.fn_arg);		

	}

	
#endif
