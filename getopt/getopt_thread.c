// arguments: -s 16384 soner test ediyor
/*

16384
argc = 6 optind = 3
Thread 1: top of stack near 0x70000e641ef0; argv_string=soner
Thread 3: top of stack near 0x70000e64fef0; argv_string=ediyor
Thread 2: top of stack near 0x70000e648ef0; argv_string=test
Joined with thread 1; returned value was SONER
Joined with thread 2; returned value was TEST
Joined with thread 3; returned value was EDIYOR


*/

#include <pthread.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <ctype.h>

#define handle_error_en(en, msg) \
               do { errno = en; perror(msg); exit(EXIT_FAILURE); } while (0)

#define handle_error(msg) \
               do { perror(msg); exit(EXIT_FAILURE); } while (0)

struct thread_info {    /* Used as argument to thread_start() */
    pthread_t thread_id;        /* ID returned by pthread_create() */
    int       thread_num;       /* Application-defined thread # */
    char     *argv_string;      /* From command-line argument */
};

/* Thread start function: display address near top of our stack,
   and return upper-cased copy of argv_string */

static void *
thread_start(void *arg)
{
    struct thread_info *tinfo = arg;
    char *uargv, *p;

    printf("Thread %d: top of stack near %p; argv_string=%s\n",
           tinfo->thread_num, &p, tinfo->argv_string);

    uargv = strdup(tinfo->argv_string);
    if (uargv == NULL)
        handle_error("strdup");

    for (p = uargv; *p != '\0'; p++)
        *p = toupper(*p);

    return uargv;
}

int
main(int argc, char *argv[])
{
    int s, tnum, opt, num_threads;
    struct thread_info *tinfo;
    pthread_attr_t attr;
    int stack_size;
    void *res;

    /* The "-s" option specifies a stack size for our threads */

    stack_size = -1;
    while ((opt = getopt(argc, argv, "s:")) != -1) {
        switch (opt) {
            case 's':
                stack_size = strtoul(optarg, NULL, 0);
                puts(optarg);
                break;

            default:
                fprintf(stderr, "Usage: %s [-s stack-size] arg...\n",
                        argv[0]);
                exit(EXIT_FAILURE);
        }
    }
    printf("argc = %d optind = %d\n", argc, optind);

    num_threads = argc - optind;

    /* Initialize thread creation attributes */

    s = pthread_attr_init(&attr);
    if (s != 0)
        handle_error_en(s, "pthread_attr_init");

    if (stack_size > 0) {
        s = pthread_attr_setstacksize(&attr, stack_size);
        if (s != 0)
            handle_error_en(s, "pthread_attr_setstacksize");
    }

    /* Allocate memory for pthread_create() arguments */

    tinfo = calloc(num_threads, sizeof(struct thread_info));
    if (tinfo == NULL)
        handle_error("calloc");

    /* Create one thread for each command-line argument */

    for (tnum = 0; tnum < num_threads; tnum++) {
        tinfo[tnum].thread_num = tnum + 1;
        tinfo[tnum].argv_string = argv[optind + tnum];

        /* The pthread_create() call stores the thread ID into
           corresponding element of tinfo[] */

        s = pthread_create(&tinfo[tnum].thread_id, &attr,
                           &thread_start, &tinfo[tnum]);
        if (s != 0)
            handle_error_en(s, "pthread_create");
    }

    /* Destroy the thread attributes object, since it is no
       longer needed */

    s = pthread_attr_destroy(&attr);
    if (s != 0)
        handle_error_en(s, "pthread_attr_destroy");

    /* Now join with each thread, and display its returned value */

    for (tnum = 0; tnum < num_threads; tnum++) {
        s = pthread_join(tinfo[tnum].thread_id, &res);
        if (s != 0)
            handle_error_en(s, "pthread_join");

        printf("Joined with thread %d; returned value was %s\n",
               tinfo[tnum].thread_num, (char *) res);
        free(res);      /* Free memory allocated by thread */
    }

    free(tinfo);
    exit(EXIT_SUCCESS);
}
