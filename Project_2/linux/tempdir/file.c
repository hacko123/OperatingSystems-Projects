#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

int main (int argc, char *argv[]) {
    if (argc != 2) {
        return EXIT_FAILURE;
    }
    const char *filename = argv[1];
    struct stat st;
    if (stat(filename, &st) != 0) {
        return EXIT_FAILURE;
    }
    fprintf(stdout, "file size: %zd\n", st.st_size);
    return EXIT_SUCCESS;
}