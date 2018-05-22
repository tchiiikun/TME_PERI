#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <RF24/RF24.h>
#include <ctime>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

typedef uint8_t byte;

using namespace std;

RF24 radio(15,8); // radio(CE,CS)

byte addresses[][6] = {"08122"};

int fd;

void setup() {
  radio.begin();
  radio.setRetries(15,15);
  radio.setPALevel(RF24_PA_LOW);
  radio.openReadingPipe(1, addresses[0]);
  radio.printDetails();
  radio.startListening();
  Serial.begin(9600);
}

//~ char mess[16];
char buffer[32];
//~ unsigned long timer;

void loop() {
  //~ cout << "." << flush;
  //~ sprintf(buffer,"%s %d", mess, timer);
  if( radio.available()){
	  radio.read( buffer, sizeof(buffer) );
	  write(fd, buf, sizeof(buffer) );	
	  write(fd, '\n', 1);
	  Serial.println(buffer);
	  //~ timer++;  
	  sleep(1);
	}
}

int main(int argc, char **argv)
{
	fd = open("test.txt", 0_CREAT | O_RDWR);
    //~ if (argc > 1) strcpy(mess, argv[1]);
    setup();
    while (1)
        loop();
    return 0;
}
