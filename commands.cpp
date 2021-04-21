//		commands.c
//********************************************
#include "commands.h"
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
static int org_num = 0;
struct job {
    int org_number;
    string title_of_job;
    int pid_num;
    clock_t time_of_exc;
    int stopped = 0;
    /*inline job operator=(job old_job){
        org_number=old_job.org_number;
        title_of_job = old_job.title_of_job;
        pid_num = old_job.pid_num;
        time_of_exc = old_job.time_of_exc;
        stopped = old_job.stopped;
    }*/
};

/*ostream& operator<<(ostream& os, job & job_to_print) {
    return os << "["<<job_to_print.org_number<<"] "<<job_to_print.title_of_job
              << " : " << job_to_print.pid_num <<job_to_print.time_of_exc<<" secs "
              << ((job_to_print.stopped == 1)?"stopped":"")<< endl;
}*/
int ExeCmd(list <int>& jobs, char* lineSize, char* cmdString, char* prv_dir, list <string>& history_commands)
{
    char* cmd;
    char* args[MAX_ARG];
    char* hist_com[MAX_ARG];
    char pwd[MAX_LINE_SIZE];
    char* delimiters = " \t\n";
    int i = 0, num_arg = 0;
    bool illegal_cmd = false; // illegal command
    cmd = strtok(lineSize, delimiters);
    if (cmd == NULL)
        return 0;
    args[0] = cmd;
    for (i = 1; i < MAX_ARG; i++)
    {
        args[i] = strtok(NULL, delimiters);
        if (args[i] != NULL)
            num_arg++;

    }
    /*
    job new_job;
    new_job.org_number = org_num;
    new_job.pid_num = getppid();
    new_job.title_of_job = string(args[0]);
    new_job.time_of_exc = clock();
    jobs.push_back(new_job);
    org_num++;*/
    /*************************************************/
    // Built in Commands PLEASE NOTE NOT ALL REQUIRED
    // ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
    // MORE IF STATEMENTS AS REQUIRED
    /*************************************************/
    if (!strcmp(cmd, "cd"))
    {
        char* cur_dir = getcwd(pwd, MAX_LINE_SIZE);
        if (!(strcmp(args[1], "-"))) { //going back a folder
            if (!(strcmp(prv_dir, ""))) {
                printf("cd: OLDPWD not set\n");
            }
            else {
                if (chdir(prv_dir) == -1)
                    illegal_cmd = true;
                if (illegal_cmd == false)
                    strcpy(prv_dir, cur_dir);
            }
        }
        // got an actual destination
        else {
            if (chdir(args[1]) == -1)
                illegal_cmd = true;
            if (illegal_cmd == false)
                strcpy(prv_dir, cur_dir);
        }
    }

    /*************************************************/
    else if (!strcmp(cmd, "pwd"))
    {
        char* path = getcwd(pwd, MAX_LINE_SIZE);
        if (path != NULL) {
            printf("%s\n", path);
        }
        else {
            perror("getcwd() error");
        }
    }

    /*************************************************/
    else if (!strcmp(cmd, "mkdir"))
    {

    }
    /*************************************************/

    else if (!strcmp(cmd, "jobs"))
    {
        //list <job> :: iterator it;
        //for(it = jobs.begin(); it != jobs.end() ; ++it) {
        //    cout << *it << '\n';
        //}
    }
    /*************************************************/
    else if (!strcmp(cmd, "showpid"))
    {
        pid_t dad_pid = getppid();
        cout << "smash pid is " << dad_pid << '\n';
    }
    /*************************************************/
    else if (!strcmp(cmd, "fg"))
    {

    }
    /*************************************************/
    else if (!strcmp(cmd, "cp"))
    {
        //create new file
        int fd_out = open(args[2], O_WRONLY | O_CREAT);
        //open old file
        int fd_in = open(args[1], O_RDONLY | O_CREAT);
        //int fd_cl = close(fd_in);
        //copy file
        int suc = sendfile(fd_out, fd_in, 0, 5000);
    }
    /*************************************************/
    else if (!strcmp(cmd, "bg"))
    {

    }
    /*************************************************/
    else if (!strcmp(cmd, "quit"))
    {
        exit(1);
    }
    /*************************************************/
    else if (!strcmp(cmd, "history"))
    {
        list <string> ::iterator it;
        for (it = history_commands.begin(); it != history_commands.end(); ++it) {
            cout << '\t' << *it << '\n';
        }
    }
    /*************************************************/

    else // external command
    {
        ExeExternal(args, cmdString);
        return 0;
    }
    if (illegal_cmd == true)
    {
        printf("smash error: > \"%s\"– No such file or directory\n", cmdString);
        return 1;
    }
    /*************************************************/
// inserting history commands into list
    if (history_commands.size() == 50)
        history_commands.pop_front();
    history_commands.push_back(cmdString);
    /*************************************************/
    return 0;
}
//**************************************************************************************
// function name: ExeExternal
// Description: executes external command
// Parameters: external command arguments, external command string
// Returns: void
//**************************************************************************************
void ExeExternal(char* args[MAX_ARG], char* cmdString)
{
    int pID;
    switch (pID = fork())
    {
    case -1:
        // Add your code here (error)

        /*
        your code
        */
    case 0:
        // Child Process
        setpgrp();

        // Add your code here (execute an external command)

        /*
        your code
        */

        //default:
        // Add your code here

        /*
        your code
        */
    }
}
//**************************************************************************************
// function name: ExeComp
// Description: executes complicated command
// Parameters: command string
// Returns: 0- if complicated -1- if not
//**************************************************************************************
int ExeComp(char* lineSize)
{
    char ExtCmd[MAX_LINE_SIZE + 2];
    char* args[MAX_ARG];
    if ((strstr(lineSize, "|")) || (strstr(lineSize, "<")) || (strstr(lineSize, ">")) || (strstr(lineSize, "*")) || (strstr(lineSize, "?")) || (strstr(lineSize, ">>")) || (strstr(lineSize, "|&")))
    {
        // Add your code here (execute a complicated command)

        /*
        your code
        */
    }
    return -1;
}
//**************************************************************************************
// function name: BgCmd
// Description: if command is in background, insert the command to jobs
// Parameters: command string, pointer to jobs
// Returns: 0- BG command -1- if not
//**************************************************************************************
int BgCmd(char* lineSize, list<int>& jobs)
{

    char* Command;
    char* delimiters = " \t\n";
    char* args[MAX_ARG];
    if (lineSize[strlen(lineSize) - 2] == '&')
    {
        lineSize[strlen(lineSize) - 2] = '\0';
        // Add your code here (execute a in the background)

        /*
        your code
        */

    }
    return -1;
}
