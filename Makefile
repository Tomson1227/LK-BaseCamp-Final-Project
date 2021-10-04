
obj-m := hc-sr04.o

ALL:
	$(MAKE) -C ~/repos/busybox/_install/lib/modules/5.9.16/build M=$(PWD) modules
clean:
	$(MAKE) -C ~/repos/busybox/_install/lib/modules/5.9.16/build M=$(PWD) clean
