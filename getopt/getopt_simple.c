// arguments: -b arg1forb -c arg2forc extraArg

/*

argc = 6 main optind = 5
aflag = 0, bflag = 1, cvalue = arg2forc
Non-option argument extraArg

*/

// *** **** optind counts only coomand-line arguments if any matching exists
// watch out that if the option takes argument optind is incremented by 2, otherwise 1

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int
main (int argc, char **argv)
{
    int aflag = 0;
    int bflag = 0;
    char *cvalue = NULL;
    int index;
    int c;



    while ((c = getopt (argc, argv, "ab:c:")) != -1
    )
        switch (c)
        {
            case 'a':
                aflag = 1;
                break;
            case 'b':
                bflag = 1;
                break;
            case 'c':
                cvalue = optarg;
                break;
            case '?':
                if (optopt == 'c')
                    fprintf (stderr, "Option -%c requires an argument.\n", optopt);
                else if (isprint (optopt))
                    fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                else
                    fprintf (stderr,
                             "Unknown option character `\\x%x'.\n",
                             optopt);
                return 1;
            default:
                abort ();
        }

    printf("argc = %d main optind = %d\n",argc, optind);


    printf ("aflag = %d, bflag = %d, cvalue = %s\n",
            aflag, bflag, cvalue);

    for (index = optind; index < argc; index++)
        printf ("Non-option argument %s\n", argv[index]);
    return 0;
}
