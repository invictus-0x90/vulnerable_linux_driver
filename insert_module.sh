sudo rmmod vuln_driver
sudo insmod src/vuln_driver.ko
sudo chmod 666 /dev/vulnerable_device
