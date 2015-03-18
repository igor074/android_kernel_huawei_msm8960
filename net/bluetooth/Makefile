#
# Makefile for the Linux Bluetooth subsystem.
#

obj-$(CONFIG_BT)	+= bluetooth.o
obj-$(CONFIG_BT_RFCOMM)	+= rfcomm/
obj-$(CONFIG_BT_BNEP)	+= bnep/
obj-$(CONFIG_BT_CMTP)	+= cmtp/
obj-$(CONFIG_BT_HIDP)	+= hidp/

<<<<<<< HEAD
bluetooth-y := af_bluetooth.o hci_core.o hci_conn.o hci_event.o mgmt.o hci_sock.o hci_sysfs.o lib.o smp.o
bluetooth-$(CONFIG_BT_L2CAP)	+= l2cap_core.o l2cap_sock.o amp.o
bluetooth-$(CONFIG_BT_SCO)	+= sco.o
=======
bluetooth-y := af_bluetooth.o hci_core.o hci_conn.o hci_event.o mgmt.o \
	hci_sock.o hci_sysfs.o l2cap_core.o l2cap_sock.o smp.o sco.o lib.o \
	amp.o
>>>>>>> cm-10.0
