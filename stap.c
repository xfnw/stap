#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

#define CLEAR "\033[2J\033[H\033[2J"
#define usage(name) ( printf("usage: %s height [xoffset] [yoffset]\n", name) )

int hsleep(int delay) {
	/* 1000000 is the number of us in s */
	long usec = delay * (1000000/10);

	/* TODO: consider using nanosleep
	 * usleep is deprecated, however nanosleep is a lot
	 * more complicated to use */
	return usleep(usec);
}

int main(int argc, char *argv[]) {
	if (argc < 2) {
		if (argc != 0)
			usage(*argv);
		return EINVAL;
	}

	int height = atoi(argv[1]);
	if (height == 0) {
		usage(*argv);
		return EINVAL;
	}

	/* to make the offsetting faster, we pre-compute
	 * them in memory, and hide CLEAR in there too */
	char *xoffset = "";
	char *yoffset = CLEAR;

	{
		int xoffn = 0;
		int yoffn = 0;

		if (argc > 2) {
			xoffn = atoi(argv[2]);
		}
		if (argc > 3) {
			yoffn = atoi(argv[3]);
		}

		/* sizeof(CLEAR) is oversized by 1 due to
		 * its null byte, which gives us a free null
		 * byte as calloc zeros first */
		yoffset = calloc(sizeof 'h', sizeof(CLEAR)+xoffn+yoffn);

		/* resource saving trick: xoffset is the
		 * second half of yoffset, reading yoffset
		 * will get both */
		xoffset = yoffset + yoffn + sizeof(CLEAR)-1;

		memcpy(yoffset, CLEAR, sizeof(CLEAR)-1);
		memset(yoffset+sizeof(CLEAR)-1, '\n', yoffn);
		memset(xoffset, ' ', xoffn);
	}

	while (1) {
		int delay = 0;
		char cur = 0;

		{
			char delaybuf[5] = "";

			while (delay < 4 && (cur = getchar())) {
				if (cur == EOF)
					return 0;
				if (cur == '\n' || cur == ' ')
					break;
				delaybuf[delay++] = cur;
			}

			delay = atoi(delaybuf);
		}

		fputs(yoffset, stdout);

		{
			int lines = 0;
			while (lines < height && (cur = getchar())) {
				if (cur == EOF)
					return 0;
				putchar(cur);
				if (cur == '\n') {
					fputs(xoffset, stdout);
					lines++;
				}
			}
		}

		hsleep(delay);
	}
}

