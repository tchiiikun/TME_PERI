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

typedef uint8_t byte;
using namespace std;
RF24 radio(15,8); // radio(CE,CS)
byte addresses[][6] = {"0XXXX"};
int fd;
char buffer[32];
sqlite3 *db;

void setup()
{
	radio.begin();
	radio.setPALevel(RF24_PA_LOW);
	radio.openReadingPipe(1, addresses[0]);
	radio.printDetails();
	radio.startListening();
}


void loop()
{
	char *query = NULL;
	//~ char* date; // TODO CALCULER LA DATE
	sqlite3_stmt *stmt;
	int ret;

	if( radio.available()){
		radio.read( buffer, sizeof(buffer) );
		asprintf(&query, "insert into lux (lumiere) values('%s');", buffer);
		ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
		if (ret != SQLITE_DONE){
			fprintf(stderr, "problem on insertion of data");
			goto out_write;
		}

		/* write(fd, buffer, sizeof(buffer) );*/
		/* write(fd, "\n", 1);*/

		sleep(1);
	}
out_write:
		sqlite3_finalize(stmt);
		free(query);
}

int main(int argc, char **argv)
{
	int ret;
	char * errmsg;
	sqlite3_stmt *stmt;

	/* fd = open("test.txt", O_RDWR);*/
	setup();

	ret = sqlite3_open("arduino_db", &db);
	if ( ret != SQLITE_OK){
		fprintf(stderr, "problem on open of database");
		goto out_open;
	}

	sqlite3_prepare_v2(db, "CREATE TABLE lux (date TEXT, lumiere TEXT);", -1, &stmt, NULL);
	ret = sqlite3_step(stmt);
	if (ret != SQLITE_DONE){
		fprintf(stderr, "problem on insetion of data");
		goto out_create;
	}

	while (1){
		loop();
	}
out_create:
	sqlite3_finalize(stmt);

out_open:
	sqlite3_close(db);
	return 0;
}
