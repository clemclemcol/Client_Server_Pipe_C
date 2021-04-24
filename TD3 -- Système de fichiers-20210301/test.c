#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <unistd.h>
#include "io_file.h"

int main(int argc, char **argv) {
    printf("%d\n", O_RDONLY);
    printf("%d\n", O_RDWR);
    printf("%d\n", O_WRONLY);
	return 0;
}
