CC=gcc
CCOPTS=-std=gnu99
BINS=../../src/test/capture_joystick

.phony: clean all

all: $(BINS)

capture_joystick:	capture_joystick.c
	$(CC) -$(CCOPTS) -o $@ $^  

clean:
	rm -rf *~ $(BINS) *.pgm

