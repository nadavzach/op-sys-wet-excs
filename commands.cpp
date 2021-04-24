//		commands.cpp
//********************************************
#include "commands.h"

//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************


job::job(string title_of_job, int pid_num,clock_t time_of_exc)
{
    /*if(title_of_job.empty()||time_of_exc == -1)
       return NULL;
   else*/
    //{

    this->time_of_exc = time_of_exc;
    this->org_num = job::org_num;
    this->title_of_job = title_of_job;
    this->pid_num = pid_num;
    job::org_num++;

    //}
}
bool job::operator==(const job& rhs)
{
    if(this->org_num == rhs.org_num)
        return true;
    else
        return false;
}


void print_jobs( const std::list <job*>& jobs)
{
    std::list<job*>::const_iterator it;

clock_t job_running_time,curr_time;
for(it = jobs.begin(); it != jobs.end(); ++it)
{
curr_time = clock();
job_running_time =  curr_time - *it.time_of_exc;
std::cout<<"["<<*it.org_number<<"] "<<*it.title_of_job<<" "<<*it.pid_num<<" "<<job_running_time<<" secs";
if(*it.stopped)
{
std::cout<<" stopped"<<endl;
}
else
{
std::cout<<endl;
}

}

}



job& job::operator=(job& old_job){
    this->org_num=old_job.org_num;
    this->title_of_job = old_job.title_of_job;
    this->pid_num = old_job.pid_num;
    this->time_of_exc = old_job.time_of_exc;
    this->stopped = old_job.stopped;
}





int ExeCmd(list <job&>& jobs, char* lineSize, char* cmdString, char* prv_dir, list <string>& history_commands)
=======
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
int ExeCmd(list <job*>& jobs, char* lineSize, char* cmdString, char* prv_dir, list <string>& history_commands)
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
    job new_job;
    new_job.org_number = org_num;
    new_job.pid_num = getppid();
    new_job.title_of_job = string(args[0]);
    new_job.time_of_exc = clock();
    //JJJ.push_back(new_job);
    org_num++;
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
=======

    /*************************************************/

    else if (!strcmp(cmd, "jobs"))
    {
        print_jobs(jobs);
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
=======
    /*************************************************/
    else if (!strcmp(cmd, "diff"))
    {
        bool diff = true;
        fstream f1, f2;
        char c1, c2;
        f1.open(args[1], ios::in);
        f2.open(args[2], ios::in);
        if ((f1.get() == EOF) || (f2.get() == EOF))
            perror("File can't be opened");
        else {
            while (1) {
                c1 = f1.get();
                c2 = f2.get();
                if (c1 != c2) {
                    diff = false;
                    break;
                }
                if ((c1 == EOF) || (c2 == EOF))
                    break;
            }
            f1.close();
            f2.close();
            if (diff == false) cout << "1" << endl;
            else cout << "0" << endl;
        }
    }
    /*************************************************/
    else if (!strcmp(cmd, "cp"))
    {
        //create new file
        int fd_out = open(args[2], O_RDWR | O_CREAT, 0666);
        //open old file
        int fd_in = open(args[1], O_RDONLY);
        if (fd_in == -1 || fd_out == -1)
            perror("cp error");
        else {
            //copy file
            struct stat st;
            int byte_size;
            if (stat(args[1], &st) == 0) {
                //Size of file, in bytes.
                byte_size = st.st_size;
            }
            int suc = sendfile(fd_out, fd_in, 0, byte_size);
            if (suc == -1)
                perror("cp error");
            else
                cout << args[1] << " has been copied to " << args[2] << endl;
        }
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
        printf("smash error: > \"%s\"� No such file or directory\n", cmdString);
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
int BgCmd(char* lineSize, list<job&>& jobs)
=======
int BgCmd(char* lineSize, list<job*>& jobs)
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
