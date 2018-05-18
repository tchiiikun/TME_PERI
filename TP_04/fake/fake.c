#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define MAXServerResquest 1024

void monitor(fd_set set, struct timeval tv, int from, int to, char *buf)
{
	int nbchar;

	FD_ZERO(&set);
	FD_SET(from, &set);

	if (select(from+1, &set, NULL, NULL, &tv) != 0) {
		if (FD_ISSET(from, &set)) {
			nbchar = read(from, buf, MAXServerResquest);
			if (nbchar == 0)
				return;
			buf[nbchar] = 0;
			write(to, buf, nbchar);
		}
	}

	return;
}

int main()
{

	int nbchar_s2f;
	int nbchar_stdin;
	int     f2s, s2f;                                       // fifo file descriptors
	char    *f2sName = "/tmp/f2s_lf";                       // filo names
	char    *s2fName = "/tmp/s2f_lf";                       //

	char    serverRequest_s2f[MAXServerResquest];               // buffer for the request
	fd_set  rfds_s2f;                                           // flag for select
	struct  timeval tv_s2f;                                     // timeout
	tv_s2f.tv_sec = 1;                                          // 1 second
	tv_s2f.tv_usec = 0;                                         //
	int bool_s2f = 0;

	char    serverRequest_stdin[MAXServerResquest];               // buffer for the request
	fd_set  rfds_stdin;                                           // flag for select
	struct  timeval tv_stdin;                                     // timeout
	tv_stdin.tv_sec = 1;                                          // 1 second
	tv_stdin.tv_usec = 0;                                         //
	int bool_stdin = 0;
	int bool_stdin_prec = 0;

	mkfifo(s2fName, 0666);                                  // fifo creation
	mkfifo(f2sName, 0666);

	/* open both fifos */
	s2f = open(s2fName, O_RDWR);                            // fifo openning
	f2s = open(f2sName, O_RDWR);

	do {
		/* initialise s2f */
		FD_ZERO(&rfds_s2f);
		FD_SET(s2f, &rfds_s2f);

		/* initialise stdin*/
		FD_ZERO(&rfds_stdin);
		FD_SET(0, &rfds_stdin);

		/* check if there is something on both stdin and s2f*/
		bool_stdin |= select(0+1, &rfds_stdin, NULL, NULL, &tv_stdin);
		bool_s2f |= select(s2f+1, &rfds_s2f, NULL, NULL, &tv_s2f);

		/* check if this turn there is something */
		if (bool_stdin){
			nbchar_stdin = read(0, serverRequest_stdin, MAXServerResquest);
			bool_stdin_prec = 1;
			bool_stdin = 0;
		}

		/* check if there have been somthing previously on stdin and if
		 * there is something of s2f */
		if (bool_s2f && bool_stdin_prec){
			printf(" Both conditions are okay\n");
				nbchar_s2f = read(s2f, serverRequest_s2f, MAXServerResquest);
				if (nbchar_s2f == 0)
					break;
				serverRequest_s2f[nbchar_s2f] = 0;
				write(1, serverRequest_s2f, nbchar_s2f);
				write(f2s, serverRequest_stdin, nbchar_stdin);
				bool_s2f = bool_stdin = bool_stdin_prec = 0;
		}
	} while (1);

	close(f2s);
	close(s2f);

	return 0;
}
