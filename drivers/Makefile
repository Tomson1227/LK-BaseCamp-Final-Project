ifndef MODE
MODE=m
endif

MAKE_M = make

DIR_LIST := $(shell find -type d \( ! -iname "." \) -printf '%p ')

all: exports $(DIR_LIST)

exports:
	export ARCH=arm
	export PATH=/opt/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin:$PATH
	export CROSS_COMPILE="/opt/gcc-arm-8.3-2019.03-x86_64-arm-linux-gnueabihf/bin/arm-linux-gnueabihf-"

$(DIR_LIST):
	$(MAKE_M) MODE=$(MODE) -C ./$@ ALL

.PHONY: $(DIR_LIST)
