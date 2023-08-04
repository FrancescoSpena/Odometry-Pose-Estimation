BINS=comm_test.elf 


OBJS =  uart.o timer.o beth_drive_internal.o beth_drive.o packet_handler.o 
OBJS += encoder.o beth_joint_internal.o pins.o digio.o pwm.o beth_joint.o beth_globals.o
OBJS += comm_uart.o print_packets.o beth_comm.o 


HEADERS =  uart.h pins.h digio.h pwm.h  beth_globals.h timer.h beth_drive_internal.h 
HEADERS += encoder.h beth_packets.h beth_joint.h beth_joint_internal.h
HEADERS += beth_drive.h packet_handler.h packet_status.h comm_uart.h 
HEADERS += print_packets.h packet_operations.h beth_comm.h 

include avr.mk




