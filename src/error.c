#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#ifdef _WIN32
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif
#include "error.h"

/*
 * Shows an error message and exits the program with EXIT_FAILURE status.
 */
void error(const char *format, ...) {
	va_list args;

#ifdef _WIN32
	/* On Windows, show a message box */
	char message[4096];
	va_start(args, format);
	vsprintf(message, format, args);
	va_end(args);
	MessageBox(NULL, message, "Error", MB_ICONERROR);
#else
	/* On other systems, print to standard error stream */
	fprintf(stderr, "Error: ");
	va_start(args, format);
	vfprintf(stderr, format, args);
	va_end(args);
#endif

	exit(EXIT_FAILURE);
}
