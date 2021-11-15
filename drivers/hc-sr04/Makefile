# INC := inc/
# SRC := src/

# SRC_C := $(wildcard $(SRC)*.c)
# OBJ := $(SRC_C:$(SRC)%.c=$(SRC)%.o)

# userccflags += -Iinc/
# subdir-m += src/hc-sr04.o

obj-m := hc-sr04.o
# hc_sr04-m := $(OBJ)

ALL:
	$(MAKE) -C ~/repos/busybox/_install/lib/modules/5.9.16/build M=$(PWD) modules
clean:
	$(MAKE) -C ~/repos/busybox/_install/lib/modules/5.9.16/build M=$(PWD) clean
