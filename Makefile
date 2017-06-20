obj-m += src/vuln_driver.o

EXTRA_CFLAGS += -fno-stack-protector
all:
	make -C /lib/modules/`uname -r`/build M=$(PWD) modules
clean:
	rm -rf src/*.ko
	rm -rf src/*.mod
	rm -rf src/*.mod.*
	rm -rf src/*.o
	rm .tmp_versions/vuln_driver.mod
	rm src/.vuln_driver.ko.cmd
	rm src/.vuln_driver.mod.o.cmd
	rm src/.vuln_driver.o.cmd
	rm Module.symvers
	rm modules.order

