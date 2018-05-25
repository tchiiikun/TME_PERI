#include <cstdlib>
#include <iostream>
#include <sstream>
#include <string.h>
#include <unistd.h>
#include "RF24/RF24.h"
#include <ctime>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sqlite3.h>

typedef uint8_t byte;
using namespace std;
RF24 radio(15,8); // radio(CE,CS)
byte addresses[][6] = {"0XXXX"};
int fd;
int prec = 300;
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
	sqlite3_stmt *stmt;
	int ret;
	int verification = 0;

	if( radio.available()){

		radio.read( buffer, sizeof(buffer) );
		query = (char*) (malloc(1024*sizeof(char)));
		verification = atoi(buffer) % 1200; // valeur max de la luminosite pour eviter d'avoir des trop grosses valeurs.

		if (verification ==  0){
			verification = prec;
		} else {
			prec = verification;
		}


		sprintf(query, "INSERT INTO Ard VALUES(strftime('%s', 'now'), %d);","%s", verification);
		printf(" COMMAND: %s\n", query);
		printf("val = %d\n", atoi(buffer));

		ret = sqlite3_prepare_v2(db, query, strlen(query), &stmt, NULL);
		fprintf(stderr, "Read = %s \n %s\n", buffer, query);
		
		if (ret != SQLITE_OK){
			fprintf(stderr, "Could not insert data.\n");
			sqlite3_finalize(stmt);
			free(query);
			return;
		}
		
		ret = sqlite3_step(stmt);
		if (ret != SQLITE_DONE){
			fprintf(stderr, "Could not step (ecxecute stmt).\n");
			sqlite3_finalize(stmt);
			free(query);
			return;
		}

		free(query);

		//sleep(1);
		usleep(1000000);
	}
}

int main(int argc, char **argv)
{
	int ret;
	char * errmsg;
	sqlite3_stmt *stmt;

	setup();

	ret = sqlite3_open("toto.db", &db);
	printf(" ret : %d\n", ret);
	if ( ret != SQLITE_OK){
		fprintf(stderr, "Could not open database");
		goto out_open;
	}

	ret = sqlite3_exec(db, "CREATE TABLE Ard(time INT, lumiere INT);", 0, 0, &errmsg);
	printf(" ret : %d\n", ret);
	if ( ret != SQLITE_OK){
		fprintf(stderr, "Error in statement: %s [%s].\n", stmt, errmsg);
		goto out_create;
	}

	printf(" Going for the loop\n", ret);

	while (1){
		loop();
	}

out_create:
	sqlite3_finalize(stmt);

out_open:
	sqlite3_close(db);
	return 0;
}
