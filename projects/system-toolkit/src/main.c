#include "snapshot.h"

#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    struct system_snapshot snap;
    char text[2048];
    int err;

    if (snapshot_collect(&snap) != 0) {
        err = errno;
        fprintf(stderr, "système: lecture impossible: %s\n", strerror(err));
        return EXIT_FAILURE;
    }
    if (snapshot_format(&snap, text, sizeof text) != 0) {
        err = errno;
        fprintf(stderr, "système: mise en forme impossible: %s\n",
                strerror(err));
        return EXIT_FAILURE;
    }
    if (fputs(text, stdout) == EOF) {
        err = errno;
        fprintf(stderr, "système: écriture impossible: %s\n", strerror(err));
        return EXIT_FAILURE;
    }
    if (fflush(stdout) == EOF) {
        err = errno;
        fprintf(stderr, "système: écriture impossible: %s\n", strerror(err));
        return EXIT_FAILURE;
    }
    return EXIT_SUCCESS;
}
