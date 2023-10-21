#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>

/* blank the entire screen */
#define CLEAR "\033[2J"
/* move cursor to top left */
#define HOME "\033[H"
/* blank from cursor to end of line, plus send newline */
#define CLEAREOL "\033[K\n"

#define eprintf(...) ( fprintf(stderr, __VA_ARGS__) )
#define usage(name) ( eprintf("usage: %s height [xoffset] [yoffset]\n", name) )

int hsleep(int delay) {
	/* 1000000 is the number of us in s */
	long usec = delay * (1000000/15);

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
	char *xoffset = CLEAREOL;
	char *yoffset = HOME;

	{
		int xoffn = 0;
		int yoffn = 0;

		if (argc > 2) {
			xoffn = atoi(argv[2]);
		}
		if (argc > 3) {
			yoffn = atoi(argv[3]);
		}

		/* sizeof is oversized by 1 due to it
		 * counting null bytes, which gives us a
		 * free null byte as calloc zeros first */
		yoffset = calloc(sizeof 'h', sizeof(HOME)+
				sizeof(CLEAREOL)+xoffn+yoffn-1);

		if (!yoffset) {
			eprintf("Failed to allocate offsets: %s\n",
					strerror(errno));
			return errno;
		}

		/* resource saving trick: xoffset is the
		 * second half of yoffset, reading yoffset
		 * will get both */
		xoffset = yoffset + yoffn + sizeof(HOME)-1;

		memcpy(yoffset, HOME, sizeof(HOME)-1);
		memset(yoffset+sizeof(HOME)-1, '\n', yoffn);
		memcpy(xoffset, CLEAREOL, sizeof(CLEAREOL)-1);
		memset(xoffset+sizeof(CLEAREOL)-1, ' ', xoffn);
	}

	fputs(CLEAR, stdout);

	for (;;) {
		int delay = 0;
		char cur = 0;

		{
			char delaybuf[5] = "";

			while (delay < 4 && (cur = getchar())) {
				if (cur == (char)EOF) {
					putchar('\n');
					return 0;
				}
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
				if (cur == (char)EOF) {
					putchar('\n');
					return 0;
				}
				if (cur == '\n') {
					fputs(xoffset, stdout);
					lines++;
					continue;
				}
				putchar(cur);
			}
		}

		hsleep(delay);
	}
}

