obj-m += vuln_driver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	rm -rf *.ko
	rm -rf *.mod
	rm -rf *.mod.*
	rm -rf *.o
