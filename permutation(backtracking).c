#include <stdio.h>
#include <string.h>

void printResult(int const *arr, int size) {
    for (int i = 0; i < size; ++i) {
        printf("%d ", arr[i]);
    }
    printf("\n");
}


int perm(const int *src, int len, int *dest, char *destbits, int n) {
    if (n == len) {
        printResult(dest, len);
        return 1;
    } else {
        int count = 0;
        for (int i = 0; i < len; i++) {
            if (destbits[i] == 0) {
                destbits[i] = 1;
                dest[n] = src[i];
                count += perm(src, len, dest, destbits, n + 1);
                destbits[i] = 0;
            }
        }
        return count;

    }
}



int main(int argc, char *argv[]) {
     const int src[] = {9, 8, 7};
    int len = 3;
    int dest[len];
    char destbits[len];

    memset(destbits, 0, sizeof destbits);
    int total = perm(src, len, dest, destbits, 0);
    //printf("%d combinations\n", total);





    return 0;
}
