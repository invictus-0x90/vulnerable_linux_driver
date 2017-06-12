/**
* This header file defines the IOCTL numbers and macros for interacting 
* with the different vulnerabilities.
*/

#ifndef _VULN_DRIVER_

	#define _VULN_DRIVER_

	#define DEVICE_NAME "vulnerable_device"

	#define IOCTL_NUM 0xFE

	#define DRIVER_TEST _IO (IOCTL_NUM, 0)
	#define BUFFER_OVERFLOW _IOR (IOCTL_NUM, 1, char *)
	#define NULL_POINTER_DEREF _IOR (IOCTL_NUM, 2, unsigned long)
	#define ALLOC_UAF_OBJ _IO (IOCTL_NUM, 3)
	#define USE_UAF_OBJ _IO (IOCTL_NUM, 4)
	#define ALLOC_K_OBJ _IOR (IOCTL_NUM, 5, unsigned long)
#endif
