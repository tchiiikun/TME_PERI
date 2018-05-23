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
#include <sqlite3.h>
#include <stdio.h>

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
sqlite3 *db;
char *err_msg = 0;

int rc = sqlite3_open("test.db", &db);

if (rc != SQLITE_OK) {
	fprintf(stderr, "Cannot open database: %s\n", sqlite3_errmsg(db));
	sqlite3_close(db);

	return;
}	

char *sql = "DROP TABLE IF EXISTS Arduino;" 
			"CREATE TABLE Arduino(Value TEXT);"

char *ins = ""; 

void loop() {
	
	if( radio.available()){
		radio.read( buffer, sizeof(buffer) );
		//~ write(fd, buffer, sizeof(buffer) );	
		//~ write(fd, "\n", 1);
		ins = strcat(ins, "INSERT INTO Arduino VALUES('");
		ins = strcat(ins, buffer);
		ins = strcat(ins, "')");
		sql = strcat(sql, ins);
		rc = sqlite3_exec(db, sql, 0, 0, &err_msg);
		sleep(1);
	}
	
	if (rc != SQLITE_OK ) {
    
        fprintf(stderr, "SQL error: %s\n", err_msg);
        
        sqlite3_free(err_msg);        
        sqlite3_close(db);
        
        return 1;
    } 
    sqlite3_close(db);
    return;
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
