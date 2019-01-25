#include <stdlib.h>
#include "script.h"
#include "utils.h"
#include "error.h"

int main(int argc, char *argv[]) {
	script_init();
	script_run();
	script_quit();

	return EXIT_SUCCESS;
}
