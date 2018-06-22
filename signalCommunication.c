// My synchronization workout of
// https://users.cs.cf.ac.uk/Dave.Marshall/C/node24.html

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
    _Exit(111);

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

    /*   ᴧ
         |
         |
         the three of masks are filled because
         we don't want signals to interrupt their handlers (per se)
         or the other handlers

         En passant, just filling is adequate,
         there is no need to empty them priorly
    */




    struct sigaction saUSR1;
    memset(&saUSR1, 0, sizeof(saUSR1));
    saUSR1.sa_flags = 0;
    saUSR1.sa_handler = sigusr1;
    sigaction(SIGUSR1, &saUSR1, 0);

    // let the child assert signal handlers by using sigaction()
    // then let parent wake up by sending SIGUSR1 signal
    // actually it's critical section for child until unblocking (by using SIG_SETMASK &prevMask) firstly
    // and then blocking parent until waking up
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
        /*
                sigsuspend, how it returnsi its work principle
                https://stackoverflow.com/a/15284248/4990642
        */
        sigsuspend(&parentUSR1Mask);
        // after returning from sigsuspend,
        // still all signals blocked here cuz of restoration of signal mask
        fprintf(stderr, "parent is running\n");

        // sleep(5);
        // try to send ctrl c ctrl z etc. signals they are already blocked


        fprintf(stderr, "\nPARENT: sending SIGHUP\n");
        kill(pid, SIGHUP);

        fprintf(stderr, "\nPARENT: sending SIGINT\n");
        kill(pid, SIGINT);

        fprintf(stderr, "\nPARENT: sending SIGTSTP\n\n");
        kill(pid, SIGTSTP);


        int status;
        wait(&status);
        if (WIFEXITED(status)){
            int returned = WEXITSTATUS(status);
            fprintf(stderr, "\nPARENT: child exited normally with status %d\n",returned);
        }
    }
    return 0;
}
