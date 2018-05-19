#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <unistd.h>
#include <string.h>

#define MAXServerResquest 1024
#define NBLED 2
#define NBBP 1

char led[NBLED];
char bp[NBBP];

void monitor(fd_set set, struct timeval tv, int from, int to, char *buf);
int server(int fd_in, int fd_out, char *bp2f2s, char *s2f2led);

int main()
{
	int fd_in, fd_out;

	fd_out = fd_in = open("/dev/ledbpLF", O_RDWR);
	if (fd_in < 0) {
		fprintf(stderr, "Erreur d'ouverture du pilote LED et Boutons\n");
		exit(1);
	}

	/* fd_in = 0;*/
	/* fd_out = 1;*/
	server(fd_in, fd_out, bp, led);
	return 0;
}

int server(int fd_in, int fd_out, char *bp2f2s, char *s2f2led)
{
	int nbchar_s2f;
	int nbchar_fd_in;
	int     f2s, s2f;                                       // fifo file descriptors
	char    *f2sName = "/tmp/f2s_lf";                       // filo names
	char    *s2fName = "/tmp/s2f_lf";                       //

	/* char    s2f2led[MAXServerResquest];               // buffer for the request*/
	fd_set  rfds_s2f;                                           // flag for select
	struct  timeval tv_s2f;                                     // timeout
	tv_s2f.tv_sec = 1;                                          // 1 second
	tv_s2f.tv_usec = 0;                                         //
	int bool_s2f = 0;

	/* char    bp2f2s[MAXServerResquest];               // buffer for the request*/
	fd_set  rfds_fd_in;                                           // flag for select
	struct  timeval tv_fd_in;                                     // timeout
	tv_fd_in.tv_sec = 1;                                          // 1 second
	tv_fd_in.tv_usec = 0;                                         //
	int bool_fd_in = 0;
	int bool_fd_in_prec = 0;

	mkfifo(s2fName, 0666);                                  // fifo creation
	mkfifo(f2sName, 0666);

	/* open both fifos */
	s2f = open(s2fName, O_RDWR);                            // fifo openning
	f2s = open(f2sName, O_RDWR);

	do {
		/* initialise s2f */
		FD_ZERO(&rfds_s2f);
		FD_SET(s2f, &rfds_s2f);

		/* initialise fd_in*/
		FD_ZERO(&rfds_fd_in);
		FD_SET(fd_in, &rfds_fd_in);

		/* check if there is something on both fd_in and s2f*/
		bool_fd_in |= select(fd_in+1, &rfds_fd_in, NULL, NULL, &tv_fd_in);
		bool_s2f |= select(s2f+1, &rfds_s2f, NULL, NULL, &tv_s2f);

		/* check if this turn there is something */
		if (bool_fd_in){
			nbchar_fd_in = read(fd_in, bp2f2s, MAXServerResquest);
			bool_fd_in_prec = 1;
			bool_fd_in = 0;
		}

		/* check if there have been somthing previously on fd_in and if
		 * there is something of s2f */
		if (bool_s2f && bool_fd_in_prec){
			printf(" Both conditions are okay\n");
			nbchar_s2f = read(s2f, s2f2led, MAXServerResquest);
			if (nbchar_s2f == 0)
				break;
			s2f2led[nbchar_s2f] = 0;
			write(fd_out, s2f2led, nbchar_s2f);
			write(f2s, bp2f2s, nbchar_fd_in);
			bool_s2f = bool_fd_in = bool_fd_in_prec = 0;
		}
	} while (1);

	close(f2s);
	close(s2f);

	return 0;

}

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
