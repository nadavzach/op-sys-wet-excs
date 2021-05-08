// signals.c
// contains signal handler funtions
// contains the function/s that set the signal handlers



/*******************************************/
/* Name: handler_cntlc
   Synopsis: handle the Control-C */
#include "signals.h"
#include "commands.h"
void smash_sigtstp_handler(int sig)
{
    if(sig == SIGTSTP && fg_process_pid != NO_PROCESS_IN_FG ) {
        kill(fg_process_pid, SIGTSTP);
    }
}
void smash_sigint_handler(int sig)
{
    if(sig == SIGINT )
    {
        kill(fg_process_pid,SIGINT);
    }
}

