#include<stdio.h>
#include <inttypes.h>

int main() {

    /*
    volatile uint32_t i=0x01234567;
    // return 01 for big endian, 67 for little endian.
    printf ("%x\n", (*((uint8_t*)(&i))));
    */

    void * x = (void*)0x00465457; // 57544600 for big endian
    printf("%s\n", (char*)&x);

    /* 
    char const * c = (char*)&x;

    printf("\n %p --> %x", (void*)c, *c);
    c++;
    printf("\n %p --> %x", (void*)c, *c);
    c++;
    printf("\n %p --> %x", (void*)c, *c);
    c++;
    printf("\n %p --> %x", (void*)c, *c);
    */

    return 0;
}



