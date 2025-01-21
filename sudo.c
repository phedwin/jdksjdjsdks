
#include <fcntl.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <termios.h>
#include <unistd.h>

static char *password;

const struct termios original;

void disableLocalFlags(int fd, struct termios *tp, int options, int mode, ...);

// this should switch between stdin & -f flag that takes an extra file arg

char *readInput(FILE *stream) {
	char *lineptr = NULL;
	size_t size, bytes_read;

	size = 0;

	bytes_read = getline(&lineptr, &size, stream);
	// null terminate if its stdin
	if (stream->_fileno == 0)
		lineptr[bytes_read - 1] = '\0';

	return lineptr;
}

#include <pwd.h>
#include <sys/stat.h>

// figure out how to cmp against actual password
int passRoutine(char *password) {
	int ret = -1;
	struct passwd *pwd;
	setpwent();  // reset
	while ((pwd = getpwent()))
		if (strcmp(password, pwd->pw_name) == 0)
			ret = 1;
	endpwent();
	return ret;
}

void resetTermFlags() {
	// if there's a typeahead buffer, do not interfere
	tcsetattr(STDIN_FILENO, TCSADRAIN, &original);
}

static char *filename;
// you have 3 attempts at getting your password right
void main(int argc, char **argv) {
	struct termios tp;
	int attempts = 3;

	disableLocalFlags(STDIN_FILENO, &tp, TCSAFLUSH, 1, ECHO);

	int opt = getopt(argc, argv, "f:");
	switch (opt) {
		case 'f':
			filename = strdup(optarg);
			break;
		case '?':
			break;
	}

	FILE *stream;
	if (!filename)
		stream = stdin;
	else
		stream = fopen(filename, "r");
	free(filename);

	char *txt = readInput(stream);
	printf("%s", txt);
	while (attempts >= 0) {
		if (!filename && passRoutine(txt))
			attempts--;
		else
			attempts = 3;
	}
	// stty sane
	resetTermFlags();

	free(txt);
}

void disableLocalFlags(int fd, struct termios *tp, int options, int mode, ...) {
	if (!isatty(fd))
		return;

	if (tcgetattr(fd, tp) == -1)
		return;
	va_list args;
	int k, flags = 0;
	va_start(args, mode);
	for (k = 0; k < mode; k++)
		flags |= va_arg(args, int);
	va_end(args);

	tp->c_lflag &= ~flags;
	if (tcsetattr(fd, options, tp) == -1)
		return;
}