/* Soner Akar - 141044085 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

enum { TOTALNUMBEROFLINES = 1000, BUFSIZE = 1024 };

/*
    pre: it takes file name and 2D array of char
    post: holds content of the file in the 2D array and return number of lines
*/
int storeLinesInArray(const char *file, char lines[][BUFSIZE])
{
    int i = 0;
    FILE* fp = fopen(file, "r");
    if (!fp)
    {
        perror("Error file reading!");
        exit(-1);
    }
    
    while (i < TOTALNUMBEROFLINES && fgets(lines[i], sizeof(lines[0]), fp))
    {
        lines[i][strlen(lines[i]) - 1] = '\0';
        ++i;
    }
    fclose(fp);
    return i;
}


int main(int argc, char *argv[])
{
    
    if ( argc != 3 ) // argc should be 2 for correct execution
    {
        printf( "Usage is  < filename.xxx >   < a word for searchig >\n" );
        exit(-2);
    }
    
    
    FILE* fp = fopen("gfF.log", "a+");
    if (!fp)
    {
        perror("Error file appending!");
        exit(-1);
    }
    
    int i = 0;
    int wordCount = 0;
    int totalLines;
    char arr[TOTALNUMBEROFLINES][BUFSIZE];
    char const *start;
    char const *temp;
    totalLines = storeLinesInArray(argv[1], arr);
    
    fprintf(fp, "File name is = %s\n", argv[1]);
    for (i = 0; i < totalLines; ++i)
    {
        start = arr[i];
        
        while ((temp = strstr (start, argv[2])))
	{
            ++wordCount;
            fprintf (fp, "%d)  Found # %d line # %d index\n",wordCount ,i + 1 ,(int) (temp - arr[i]) + 1);
            start = temp + strlen(argv[2]) - 1;
        }
    }
    
    printf ("~~~~ Number of total \" %s \" words => %d ~~~~\n",argv[2] ,wordCount);
    fprintf (fp, "~~~~ Number of total words => %d ~~~~\n\n\n", wordCount);
    fclose(fp);
    
    return 0;
}
