#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
	if (argc < 2) {
		if (argc != 0)
			printf("usage: %s height\n", *argv);
		return 22;
	}

	while (1) {
		char delaybuf[5] = "";
		char cur = 0;
		int delay = 0;
		while (delay < 4 && (cur = getchar())) {
			if (cur == EOF)
				return 0;
			if (cur == '\n' || cur == ' ')
				break;
			delaybuf[delay++] = cur;
		}
		delay = atoi(delaybuf);
		printf("%d\n", delay);
	}
}

