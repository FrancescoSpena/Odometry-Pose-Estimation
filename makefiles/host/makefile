
#file beth_host
OBJS = ../../beth_host/beth_host.c ../../beth_host/client.c  ../../beth_host/serial.c
#file common
OBJS += ../../src/common/packet_handler.c ../../src/common/print_packets.c

main: $(OBJS) 
	gcc -Wall -O3 -o client $(OBJS) 
clean:
	rm client