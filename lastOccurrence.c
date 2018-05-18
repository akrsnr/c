// return index of a character which is closer to end of string.
// null karakterine yakın aranan karakterin indexini bul

#include<stdio.h>
#include<string.h>
#include<stdlib.h>

// bir kere bulduysam artık geriye doğru artırarak gitmem gerek
int lastOccur(const char * str, int key) {
    if (*str == '\0')
        return -1;

    int result = lastOccur(str + 1, key);
    if (result == -1) {
        if (*str == key)
            return 0;
        else
            return -1;
    }
    // bir kere buldum artık artırarak git
    return result + 1;

}


int main() {
    const char* hello = "abba";
    printf("%d\n", lastOccur(hello, 'b'));

    return 0;
}

