
#file beth_host
OBJS = ../../beth_host/beth_host.c ../../beth_host/host_joystick.c  ../../beth_host/serial.c
#file common
OBJS += ../../src/common/packet_handler.c ../../src/common/print_packets.c ../../src/common/joystick.c

main: $(OBJS) 
	gcc -Wall -O3 -o host $(OBJS) 
clean:
	rm host