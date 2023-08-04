#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <linux/joystick.h>
#include <stdlib.h>
#include <stdio.h>

//  device : /dev/input/js1
// max val --> axis y: 30.000
//             axis x: 30.000
int main(int argc, char** argv) {
  if (argc < 2) {
    printf("usage: %s <joy_device>\n", argv[0]);
    exit(EXIT_FAILURE);
  }

  printf("opening device [ %s ]\n", argv[1]);
  int fd = open(argv[1], O_RDONLY);

  if (fd < 0) {
    printf("cannot open device [ %s ]\n", argv[1]);
    exit(EXIT_FAILURE);
  }


  struct js_event e;
  while(1) {
    while (read (fd, &e, sizeof(e)) > 0 ){
      if(e.number == 0 && e.type == 2){
        printf ("axis: %d, type: %d, value: %d\n",
          e.number,
          e.type,
          abs(e.value%255));
        fflush(stdout);
      }
    }
  }
}
