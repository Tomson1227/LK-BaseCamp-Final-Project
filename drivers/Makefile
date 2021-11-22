ifndef MODE
MODE=m
endif

obj-$(MODE) += hc-sr04/
obj-$(MODE) += hmc5883l/
obj-$(MODE) += stepper/

ALL:
	$(MAKE) -C ~/repos/busybox/_install/lib/modules/5.9.16/build M=$(PWD) modules
clean:
	$(MAKE) -C ~/repos/busybox/_install/lib/modules/5.9.16/build M=$(PWD) clean

exports:
	export ARCH=arm
	export PATH=/opt/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin:$PATH
	export CROSS_COMPILE="/opt/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-"
