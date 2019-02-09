#include <stdio.h>
#include <stdint.h>
#include <netinet/in.h>
#include <stdlib.h>

int main()
{
    uint32_t x;
    FILE *fp;

    x = 0x01020304;

    fprintf(stderr,"HOST BYTE ORDER - LITTLE ENDIAN\n\tNumber: 0x%x\n", x);
    fp = fopen("tr","w+");
    if (fp != NULL)
    {
        fwrite(&x,sizeof(x),1,fp);
        fclose(fp);
        fprintf(stderr,"\nIn memory:\n\n");
        system("hexdump -C tr");
    }

    fprintf(stderr,"\nNETWORK BYTE ORDER - BIG ENDIAN\n\tNumber: 0x%x\n", x);

    x= htonl(x);

    fp = fopen("tr" ,"w+");
    if (fp != NULL)
    {
        fwrite(&x,sizeof(x),1,fp);
        fclose(fp);
        fprintf(stderr,"\nIn memory:\n\n");
        system("hexdump -C tr");
    }
    return 0;
}
