// on middle endian machine
// https://stackoverflow.com/questions/11617684/when-is-htonlx-ntohlx-or-when-is-converting-to-and-from-network-byte-o

#include <stdio.h>
#include <stdint.h>

uint32_t ntohlx(uint32_t n)
{
    unsigned char x = n & 0xff;
    uint32_t result = n >> 8;
    result |= x << 24;
    return result;
}

uint32_t htonlx(uint32_t n)
{
    unsigned char x = (n & 0xff000000) >> 24;
    uint32_t result = n << 8;
    result |= x;
    return result;
}

int main()
{
    uint32_t inMemory   = 0x04010203;
    uint32_t normal     = 0x01020304;

    printf("in Memory -> \t 0x0%x   ASSUMED\n", inMemory);
    printf("ntoh -> \t\t 0x0%x\n", ntohlx(normal));
    printf("ntoh(ntoh) -> \t 0x0%x\n", ntohlx(ntohlx(normal)));
    printf("hton -> \t\t 0x0%x\n", htonlx(inMemory));
    printf("\nitself ->  0x0%x\n", htonlx(ntohlx(inMemory)));
    printf("itself ->  0x0%x\n", ntohlx(htonlx(inMemory)));

    return 0;
}
