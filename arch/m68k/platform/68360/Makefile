#
# Makefile for arch/m68knommu/platform/68360.
#
<<<<<<< HEAD
=======
model-y			  := ram
model-$(CONFIG_ROMKERNEL) := rom
>>>>>>> cm-10.0

obj-y := config.o commproc.o entry.o ints.o

extra-y := head.o

<<<<<<< HEAD
$(obj)/head.o: $(obj)/head-$(MODEL).o
	ln -sf head-$(MODEL).o $(obj)/head.o
=======
$(obj)/head.o: $(obj)/head-$(model-y).o
	ln -sf head-$(model-y).o $(obj)/head.o
>>>>>>> cm-10.0
