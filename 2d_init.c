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


/*

                                  +-------------+
        +-------+         +------>+  arr[0][0]  |
arr[0]  |       |         |       |      |    arr[0][1]
        |   +-------------+       +------+------+
        +-------+
        |       |     +------+------+
arr[1]  |    +------->+      |      |
        +-------+     |      |      |
        |       |     +-------------+              +------+------+
arr[2]  |    +-------------------------------------+      |      |
        +-------+                                  |      |      |
        |     | |                 +------+------+  +------+------+
arr[3]  |     +------------------>+      |      |
        +-------+                 |      |      |
        |       |                 +------+------+
arr[4]  |    +  |
        +-------+
             |                        +------+------+
             +----------------------->+      |      |
                                      |      |      |
                                      +------+------+


*/

// https://stackoverflow.com/a/50545501/4990642
