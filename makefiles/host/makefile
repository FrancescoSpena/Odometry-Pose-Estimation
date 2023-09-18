
#file beth_host
OBJS = ../../beth_host/beth_host.c ../../beth_host/HOST.c  ../../beth_host/serial.c 
OBJS += ../../beth_host/packet_structure.c ../../beth_host/beth_comm_host.c
#file common
OBJS += ../../src/common/packet_handler.c ../../src/common/print_packets.c ../../src/common/joystick.c

main: $(OBJS) 
	gcc -Wall -Og -o host $(OBJS) -lncurses -lpanel
clean:
	rm host