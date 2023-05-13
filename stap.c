#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#define CLEAR "\033[2J\033[H\033[2J"
#define usage(name) ( printf("usage: %s height\n", name) )

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

		printf(CLEAR);

		{
			int lines = 0;
			while (lines < height && (cur = getchar())) {
				if (cur == EOF)
					return 0;
				if (cur == '\n')
					lines++;
				putchar(cur);
			}
		}

		hsleep(delay);
	}
}

