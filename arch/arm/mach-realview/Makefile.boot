ifeq ($(CONFIG_REALVIEW_HIGH_PHYS_OFFSET),y)
<<<<<<< HEAD
   zreladdr-y	:= 0x70008000
params_phys-y	:= 0x70000100
initrd_phys-y	:= 0x70800000
else
   zreladdr-y	:= 0x00008000
=======
   zreladdr-y	+= 0x70008000
params_phys-y	:= 0x70000100
initrd_phys-y	:= 0x70800000
else
   zreladdr-y	+= 0x00008000
>>>>>>> cm-10.0
params_phys-y	:= 0x00000100
initrd_phys-y	:= 0x00800000
endif
