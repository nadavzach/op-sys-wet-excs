//		commands.cpp
//********************************************
#include <fstream>
#include "commands.h"

//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
#define SIGTEM_TIMEOUT 5
//TODO list:
//TODO add cout for each signal sent (after before every kill(..)
//TODO check for invalid args in each command/more args than needed?. exmp1 - exit killer,exmp2 - exit kill ohNo
//TODO make sure when we print clock time we cast to double
//TODO every place we access jobs, check that its not empty with the following code:
//if (*(jobs.begin()) == nullptr)
//            cout << "no processes running!" << endl;
//        else {
job::job(string title_of_job, int pid_num,clock_t time_of_exc)
{
    /*if(title_of_job.empty()||time_of_exc == -1)
       return NULL;
   else*/
    //{

    this->time_of_exc = time_of_exc;
    this->title_of_job = title_of_job;
    this->pid_num = pid_num;
    this->proc_num = job::org_num;
    job::org_num++;

    //}
}
bool job::operator==(const job& rhs) const
{
    return (this->pid_num == rhs.pid_num);
}


void print_jobs( const std::list <job*>& jobs)
{
    //std::list<job*>::const_iterator it;

    clock_t job_running_time,curr_time;
    for(auto job : jobs)
    {
        curr_time = clock();
        job_running_time =  curr_time - job->time_of_exc;
        std::cout<<"["<<job->proc_num<<"] "<<job->title_of_job<<" "<<job->pid_num<<" "<<job_running_time<<" secs";
        if(job->stopped)
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
    this->proc_num=old_job.proc_num;
    this->title_of_job = old_job.title_of_job;
    this->pid_num = old_job.pid_num;
    this->time_of_exc = old_job.time_of_exc;
    this->stopped = old_job.stopped;
}





int ExeCmd(list <job&>& jobs, char* lineSize, char* cmdString, char* prv_dir, list <string>& history_commands)
{
    //TODO
}
static int org_num = 0;


int ExeCmd(list <job*>& jobs, char* lineSize, char* cmdString, char* prv_dir, list <string>& history_commands)
{
    char* cmd;
    char* args[MAX_ARG];
    char* hist_com[MAX_ARG];
    char pwd[MAX_LINE_SIZE];
    char* delimiters = " \t\n";
    int i = 0, num_arg = 0;
    bool found;
    bool illegal_cmd = false; // illegal command
    cmd = strtok(lineSize, delimiters);
    if (cmd == nullptr)
        return 0;
    args[0] = cmd;
    for (i = 1; i < MAX_ARG; i++)
    {
        args[i] = strtok(nullptr, delimiters);
        if (args[i] != nullptr)
            num_arg++;

    }
    args[i+1]= nullptr;

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
                if (!illegal_cmd)
                    strcpy(prv_dir, cur_dir);
            }
        }
            // got an actual destination
        else {
            if (chdir(args[1]) == -1)
                illegal_cmd = true;
            if (!illegal_cmd)
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

        /*************************************************/

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
        int status;
        //without noticing if anumber has bin given or not
        //key operations:
        //print process's name\
        //call wait with right arguments
        auto cur_job = jobs.end();
        if(args[1] == nullptr)
        {
           cout << (*cur_job)->title_of_job<<endl;
           waitpid((*cur_job)->pid_num, &status,0 );

        }
        else
        {
            found = false;
            long int proccess_num = strtol(args[1], nullptr, 10);
            if (!proccess_num)
                perror("arguments coudlnt be cast to int");
            else {
                for (cur_job = jobs.begin(); cur_job != jobs.end();cur_job++) {
                    {
                        if ((*cur_job)->proc_num == proccess_num) {
                            found = true;
                            waitpid((*cur_job)->pid_num, &status, 0);
                        }
                    }
                }
            }
        }
            if (!found)
                printf("smash error: > \"%s\"� No such process\n", cmdString);//TODO is this err msg ok?
        if (WIFEXITED(status)) {
            printf("exited, status=%d\n", WEXITSTATUS(status));
        } else if (WIFSIGNALED(status))
        {
            delete(*cur_job);
            cur_job = jobs.erase(cur_job);
            printf("killed by signal %d\n", WTERMSIG(status));
        } else if (WIFSTOPPED(status))
        {
            (*cur_job)->stopped = 1;
            printf("stopped by signal %d\n", WSTOPSIG(status));
        } else if (WIFCONTINUED(status))
        {
            printf("continued\n");

        }
    }

        /*************************************************/
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
            while (true) {
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
            if (!diff) cout << "1" << endl;
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
            int suc = sendfile(fd_out, fd_in, 0, byte_size);//TODO fix this MF
            if (suc == -1)
                perror("cp error");
            else
                cout << args[1] << " has been copied to " << args[2] << endl;
        }
    }
        /*************************************************/
    else if (!strcmp(cmd, "bg")) {
        if (*(jobs.begin()) == nullptr)
            cout << "no processes running!" << endl;
        else {
            if (args[1] == nullptr) {//bging newest proccess
                auto cur_job = jobs.begin();
                kill((*cur_job)->pid_num,SIGCONT);

            } else {//bging specific proccess
                long int proccess_num = strtol(args[1], nullptr, 10);
                for (auto cur_job : jobs) {
                    if ((*cur_job).proc_num == proccess_num) {
                        if((*cur_job).stopped)
                        {
                            kill((*cur_job).pid_num,SIGCONT);
                        }else
                        {
                            cout<<"this process hasnt been stopped!"<<endl;
                        }
                    } else {
                        cout<<"couldnt find a process with this process id"<<endl;
                    }

                }
            }
        }
    }
        /*************************************************/
    else if (!strcmp(cmd, "kill"))
    {
        found = false;
        long int proccess_num = strtol(args[2], nullptr,10);
        long int signal = strtol((args[1]+1), nullptr,10);
        for(auto job : jobs)
        {

            if(!proccess_num || !signal)
                perror("arguments coudlnt be cast to int");
            else if((*job).proc_num == proccess_num)
            {
                found = true;
                kill( (*job).pid_num, (int)signal);
            }
        }
        if(!found)
            printf("smash error: > \"%s\"� No such file or directory\n", cmdString);
    }
        /*************************************************/
    else if (!strcmp(cmd, "quit"))
    {
        int status;
        bool sigterm_succeed = false;
        clock_t time_first_sig_sent;
        if(args[1] == nullptr || ((*jobs.begin()) == nullptr))
            exit(1);
        else
        {
            std::list<job*>::iterator job_it;
            for(job_it = jobs.begin();job_it != jobs.end();job_it++)
            {
                kill((*job_it)->pid_num,SIGTERM);
                time_first_sig_sent = clock();
                while((double)(clock()-time_first_sig_sent)<SIGTEM_TIMEOUT)
                {
                    int wait_ret_Val = waitpid((*job_it)->pid_num, &status, WNOHANG);
                    if (wait_ret_Val == -1) {
                        perror("waitpid failed from quit kill ");//TODO
                    } else
                    {

                       if (WIFSIGNALED(status) && WTERMSIG(status) == SIGTERM)
                        {
                            delete((*job_it));
                            jobs.erase(job_it);
                            cout<<(*job_it)->pid_num<<"killed by signal "<<SIGTERM<<endl;//TODO is this line needed?
                            sigterm_succeed = true;
                        }
                    }
                }
                if(!sigterm_succeed) {
                    kill((*job_it)->pid_num, SIGKILL);
                    delete((*job_it));
                    jobs.erase(job_it);
                    cout<<(*job_it)->pid_num<<"killed by signal "<<SIGKILL<<endl;//TODO is this line needed?
                }
            }

        }
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
        ExeExternal(args, cmdString,jobs);
        return 0;
    }
    if (illegal_cmd)
    {
        printf("smash error: > \"%s\"� No such file or directory\n", cmdString);//TODO shouldn't this be some different error?
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
void ExeExternal(char* args[MAX_ARG], char* cmdString,std::list <job*>& jobs)
{
    int pID,execv_ret_val;
    char pwd[MAX_LINE_SIZE];
    char* path;
    switch (pID = fork())
    {
        case -1:
            perror("fork fail at ExeExternal");
        case 0:
            // Child Process
            setpgrp();


            path = getcwd(pwd, MAX_LINE_SIZE);

            execv_ret_val = execl(args[0],args[1]);
            if(execv_ret_val==-1)// execv failed
                perror("command execution failed");

        default://parent process
            job* new_job = new job(args[0],pID,clock());
            jobs.push_back(new_job);


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

void update_jobs(list<job*>& jobs) {
    int status;

    if (jobs.cbegin() != jobs.cend()) {
        for (auto job_it = jobs.begin(); job_it != jobs.end();job_it++) {
            if ( (*job_it)->pid_num > 0) {
                int wait_ret_Val = waitpid((*job_it)->pid_num, &status, WNOHANG);
                if (wait_ret_Val == -1) {
                    perror("update_jobs failed from waitpid");
                } else
                {

                    if (WIFEXITED(status)) {
                        cout<<(*job_it)->pid_num<<"exited, status="<<WEXITSTATUS(status)<<endl;
                    } else if (WIFSIGNALED(status))
                    {
                        delete(*job_it);//TODO fix this MF
                        jobs.erase(job_it);
                        cout<<(*job_it)->pid_num<<"killed by signal "<<WTERMSIG(status)<<endl;

                    } else if (WIFSTOPPED(status))
                    {
                        (*job_it)->stopped = 1;
                        cout<<(*job_it)->pid_num<<"stopped by signal "<<WSTOPSIG(status)<<endl;
                    } else if (WIFCONTINUED(status))
                    {
                        cout<<(*job_it)->pid_num<<"continued"<<endl;
                    }
                }

            }

        }
    }
}


