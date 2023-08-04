BINS=../../src/test/comm_test.elf

#atmega2560
OBJS = ../../src/arch/atmega2560/comm_uart.o ../../src/arch/atmega2560/digio.o ../../src/arch/atmega2560/encoder.o
OBJS += ../../src/arch/atmega2560/pins.o ../../src/arch/atmega2560/pwm.o ../../src/arch/atmega2560/timer.o 
OBJS += ../../src/arch/atmega2560/uart.o
#common
OBJS += ../../src/common/beth_comm.o ../../src/common/buffer.o ../../src/common/packet_handler.o
OBJS += ../../src/common/print_packets.o
#firware 
OBJS += ../../src/beth_firmware/beth_drive_internal.o ../../src/beth_firmware/beth_drive.o 
OBJS += ../../src/beth_firmware/beth_globals.o ../../src/beth_firmware/beth_joint_internal.o
OBJS += ../../src/beth_firmware/beth_joint.o

#atmege2560
HEADERS = ../../src/arch/include/comm_uart.h ../../src/arch/include/digio.h ../../src/arch/include/encoder.h
HEADERS += ../../src/arch/include/pins.h ../../src/arch/include/pwm.h ../../src/arch/include/timer.h
HEADERS += ../../src/arch/include/uart.h
#common 
HEADERS += ../../src/common/beth_comm.h ../../src/common/beth_packets.h ../../src/common/buffer.h 
HEADERS += ../../src/common/packet_handler.h ../../src/common/packet_operations.h 
HEADERS += ../../src/common/packet_status.h ../../src/common/print_packets.h
#firmware
HEADERS += ../../src/beth_firmware/beth_drive_internal.h ../../src/beth_firmware/beth_drive.h
HEADERS += ../../src/beth_firmware/beth_globals.h ../../src/beth_firmware/beth_joint_internal.h
HEADERS += ../../src/beth_firmware/beth_joint.h

include avr.mk




