obj-m += vuln_driver.o

all:
	make -C /lib/modules/$(shell uname -r)/build M=$(PWD) modules
clean:
	rm -rf *.ko
	rm -rf *.mod
	rm -rf *.mod.*
	rm -rf *.o
	rm .tmp_versions/vuln_driver.mod
	rm .vuln_driver.ko.cmd
	rm .vuln_driver.mod.o.cmd
	rm .vuln_driver.o.cmd
	rm Module.symvers
	rm modules.order

