#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>


char const * hupMsg = "CHILD: I have received a SIGHUP\n";
int hupMsgLen;
void sighup(int x) {
    write(STDERR_FILENO, hupMsg, hupMsgLen);

}

char const * intMsg = "CHILD: I have received a SIGINT\n";
int intMsgLen;
void sigint(int x) {
    write(STDERR_FILENO, intMsg, intMsgLen);
}

char const * die = "CHILD: My DADDY has Killed me!!!\n";
int dieLen;
void sigtstp(int x) {
    write(STDERR_FILENO, die, dieLen);
    _exit(111);
}


char const * sigusr1Msg = "SIGUSR1 is being caught\n";
int sigusr1MsgLen;
void sigusr1(int x) {
    write(STDERR_FILENO, sigusr1Msg, sigusr1MsgLen);
}




int main()
{
    dieLen = strlen(die);
    sigusr1MsgLen = strlen(sigusr1Msg);
    hupMsgLen = strlen(hupMsg);
    intMsgLen = strlen(intMsg);

    int         pid;
    sigset_t    allBlockedMask, prevMask, parentUSR1Mask,
                sighupSet, sigintSet, sigtstpSet;

    sigfillset(&sighupSet);
    sigdelset(&sighupSet, SIGHUP);
    sigfillset(&sigintSet);
    sigdelset(&sigintSet, SIGINT);
    sigfillset(&sigtstpSet);
    sigdelset(&sigtstpSet, SIGTSTP);


    sigfillset(&allBlockedMask);

    sigfillset(&parentUSR1Mask);
    sigdelset(&parentUSR1Mask, SIGUSR1);


    struct sigaction saINT, saHUP, saTSTP;
    memset(&saINT, 0, sizeof(saINT));
    memset(&saHUP, 0, sizeof(saHUP));
    memset(&saTSTP, 0, sizeof(saTSTP));
    saINT.sa_flags = 0;
    saINT.sa_handler = sigint;
    sigemptyset(&saINT.sa_mask);
    //sigaddset(&saINT.sa_mask, SIGTSTP);
    sigfillset(&saINT.sa_mask);
    saHUP.sa_flags = 0;
    saHUP.sa_handler = sighup;
    sigemptyset(&saHUP.sa_mask);
    sigfillset(&saHUP.sa_mask);
    saTSTP.sa_flags = 0;
    saTSTP.sa_handler = sigtstp;
    sigemptyset(&saTSTP.sa_mask);
    sigfillset(&saTSTP.sa_mask);




    struct sigaction saUSR1;
    memset(&saUSR1, 0, sizeof(saUSR1));
    saUSR1.sa_flags = 0;
    saUSR1.sa_handler = sigusr1;
    sigaction(SIGUSR1, &saUSR1, 0);


    sigprocmask(SIG_BLOCK, &allBlockedMask, &prevMask);


    /* get child process */

    if ((pid = fork()) < 0) {
        perror("fork");
        exit(1);
    }

    if (pid == 0) {   /* child */

        fprintf(stderr, "child pid %i\n", getpid());

        /* critical section to set signal handlers */
        fprintf(stderr, "child in blocking\n");

        /* set function calls */
        sigaction(SIGHUP, &saHUP, 0);
        sigaction(SIGINT, &saINT, 0);
        sigaction(SIGTSTP, &saTSTP, 0);

        /* let parent know, you can continue your process */
        kill(getppid(), SIGUSR1);

        fprintf(stderr, "child now UNblocked \n");
        sigprocmask(SIG_SETMASK, &prevMask, 0);

        /* here, I want to receive signals in turn */
        sigsuspend(&sighupSet);
        sigsuspend(&sigintSet);
        sigsuspend(&sigtstpSet);


    }
    else {
        /* parent */
        usleep(500);
        fprintf(stderr, "parent pid %i\n", getpid());


        fprintf(stderr, "parent is waiting\n");
        sigsuspend(&parentUSR1Mask);
        fprintf(stderr, "parent is running\n");


        fprintf(stderr, "\nPARENT: sending SIGHUP\n");
        kill(pid, SIGHUP);

        fprintf(stderr, "\nPARENT: sending SIGINT\n");
        kill(pid, SIGINT);

        fprintf(stderr, "\nPARENT: sending SIGTSTP\n\n");
        kill(pid, SIGTSTP);

        wait(0);
    }

    return 0;
}

