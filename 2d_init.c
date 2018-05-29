// 2D array init

#include <stdio.h>
#include <unistd.h>
#include <assert.h>
#include <fcntl.h>
#include <sys/types.h>
#include <stdlib.h>

int
main(int argc, char *argv[])
{
    int (*arr)[2];
    arr = malloc(sizeof *arr * 5);

    printf("size: %ld\n", sizeof(*arr));

    free(arr);

    return 0;
}

// https://stackoverflow.com/a/50545501/4990642
