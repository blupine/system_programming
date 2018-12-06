KERNELDIR = /lib/modules/$(shell uname -r)/build

obj-m := pir_km.o

PWD := $(shell pwd)

default :
	$(MAKE) -C $(KERNELDIR) M=$(PWD) modules
clean :
	rm *.o *.ko *.mod.c *.order *.symvers image*
