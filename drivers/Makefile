SRC := src/
INC := inc/

obj-m := hmc5883l_driver.o #hc-sr04.o stepper_motor.o

ALL:
	$(MAKE) -C ~/repos/busybox/_install/lib/modules/5.9.16/build M=$(PWD) modules
clean:
	$(MAKE) -C ~/repos/busybox/_install/lib/modules/5.9.16/build M=$(PWD) clean
