#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include "RF24/RF24.h"
#include <ctime>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef uint8_t byte;

using namespace std;

RF24 radio(15,8); // radio(CE,CS)

byte addresses[][6] = {"0XXXX"};

int fd;

void setup() {
  radio.begin();
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(1, addresses[0]);
  radio.printDetails();
  radio.startListening();
}

char buffer[32];

void loop() {
  if( radio.available()){
	  radio.read( buffer, sizeof(buffer) );
	  write(fd, buffer, sizeof(buffer) );	
	  write(fd, "\n", 1);
	  sleep(1);
	}
}

int main(int argc, char **argv)
{
	fd = open("test.txt", O_RDWR);
  setup();
  while (1){
		loop();
  }
  return 0;
}
