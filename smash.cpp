/*	smash.c
main file. This file contains the main function of smash
*******************************************************************/
#include <sys/types.h>
#include <sys/wait.h> // can only work in linux
#include <unistd.h>  // same
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>
#include "commands.h"
#include "signals.h"
#define MAX_LINE_SIZE 80
//#define MAXARGS 20

char* L_Fg_Cmd;
std::list <job*> jobs; //This represents the list of jobs. Please change to a preferred type (e.g array of char*)
int job::org_num = 0;
char lineSize[MAX_LINE_SIZE];
//**************************************************************************************
// function name: main
// Description: main function of smash. get command from user and calls command functions
//**************************************************************************************
int main(int argc, char* argv[])
{
    char cmdString[MAX_LINE_SIZE];
    char prv_dir[MAX_LINE_SIZE] = "";
    list <string> history_commands;
    //map<int,map<string,vector<int>>> list_of_jobs;

    //signal declaretions
    //NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
    /* add your code here */

    /************************************/
    //NOTE: the signal handlers and the function/s that sets the handler should be found in siganls.c
    //set your signal handlers here
    /* add your code here */

    /************************************/

    /************************************/
    // Init globals



    L_Fg_Cmd = (char*)malloc(sizeof(char) * (MAX_LINE_SIZE + 1));
    if (L_Fg_Cmd == NULL)
        exit(-1);
    L_Fg_Cmd[0] = '\0';
    /************************************/
    //~~~~~~~~signal handling~~~~~~~~~~~
    /************************************/
    struct sigaction *smash_sigtstp_struct;
    smash_sigtstp_struct->sa_handler = &smash_sigtstp_handler;
    struct sigaction *smash_sigint_struct;
    smash_sigint_struct->sa_handler = &smash_sigint_handler;
    sigaction(SIGINT,smash_sigint_struct,nullptr);
    sigaction(SIGTSTP,smash_sigtstp_struct,nullptr);
    /************************************/

    /************************************/
    while (true)
    {
        printf("smash > ");
        update_jobs(jobs);
        fgets(lineSize, MAX_LINE_SIZE, stdin);
        strcpy(cmdString, lineSize);
        cmdString[strlen(lineSize) - 1] = '\0';
        // perform a complicated Command
        if (!ExeComp(lineSize)) continue;
        // background command
        if (!BgCmd(lineSize, jobs)) continue;
        // built in commands
        ExeCmd(jobs, lineSize, cmdString, prv_dir, history_commands);

        /* initialize for next line read*/
        lineSize[0] = '\0';
        cmdString[0] = '\0';
    }
    return 0;
}
