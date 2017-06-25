#ifndef _ARBITRARY_RW_
	#define _ARBITRARY_RW_

	typedef struct mem_buffer {
		size_t data_size;
		char *data;
		loff_t pos;
	}mem_buffer;

	typedef struct init_args {
		size_t size;
	}init_args;

	typedef struct realloc_args {
		int grow;
		size_t size;
	}realloc_args;

	typedef struct read_args {
		char *buff;
		size_t count;
	}read_args;

	typedef struct seek_args {
		loff_t new_pos;
	}seek_args;

	typedef struct write_args {
		char *buff;
		size_t count;
	}write_args;

	mem_buffer *g_mem_buffer = NULL;

	/**
	* Initialise a new buffer with __user size.
	* point the global pointer to it.
	* Don't allow 0 sized allocations.
	*/
	static int arbitrary_rw_init(init_args *args)
	{
		if(args->size == 0 || g_mem_buffer != NULL)
			return -EINVAL;

		g_mem_buffer = kmalloc(sizeof(mem_buffer), GFP_KERNEL);

		if(g_mem_buffer == NULL)
			goto error_no_mem;

		g_mem_buffer->data = kmalloc(args->size, GFP_KERNEL);

		if(g_mem_buffer->data == NULL)
			goto error_no_mem_free;

		g_mem_buffer->data_size = args->size;
		g_mem_buffer->pos = 0;

		printk(KERN_INFO "[x] Allocated memory with size %lu [x]\n", g_mem_buffer->data_size);

		return 0;

		error_no_mem:
			return -ENOMEM;

		error_no_mem_free:
			kfree(g_mem_buffer);
			return -ENOMEM;
	}

	static int realloc_mem_buffer(realloc_args *args)
	{
		if(g_mem_buffer == NULL)
			return -EINVAL;

		size_t new_size;
		char *new_data;

		//We can overflow size here by making new_size = -1
		if(args->grow)
			new_size = g_mem_buffer->data_size + args->size;
		else
			new_size = g_mem_buffer->data_size - args->size;

		//new_size here will equal 0 krealloc(..., 0) = ZERO_SIZE_PTR
		new_data = krealloc(g_mem_buffer->data, new_size+1, GFP_KERNEL);

		//missing check for return value ZERO_SIZE_PTR
		if(new_data == NULL)
			return -ENOMEM;

		g_mem_buffer->data = new_data;
		g_mem_buffer->data_size = new_size;

		printk(KERN_INFO "[x] g_mem_buffer->data_size = %lu [x]\n", g_mem_buffer->data_size);

		return 0;
	}

	static int read_mem_buffer(char __user *buff, size_t count)
	{
		if(g_mem_buffer == NULL)
			return -EINVAL;

		loff_t pos;
		int ret;

		pos = g_mem_buffer->pos;

		if((count + pos) > g_mem_buffer->data_size)
			return -EINVAL;

		ret = copy_to_user(buff, g_mem_buffer->data + pos, count);

		return ret;
	}

	static int write_mem_buffer(write_args *w_args)
	{
		if(g_mem_buffer == NULL)
			return -EINVAL;

		int ret;
		loff_t pos;
		size_t count;

		count = w_args->count;
		pos = g_mem_buffer->pos;

		if((count + pos) > g_mem_buffer->data_size)
			return -EINVAL;

		ret = copy_from_user(g_mem_buffer->data + pos, w_args->buff, count);

		return ret;
	}

	static int seek_mem_buffer(seek_args *s_args)
	{
		if(g_mem_buffer == NULL)
			return -EINVAL;

		if(s_args->new_pos < g_mem_buffer->data_size) {
			g_mem_buffer->pos = s_args->new_pos;
			return g_mem_buffer->pos;
		}
		else
			return 0;
	}


#endif