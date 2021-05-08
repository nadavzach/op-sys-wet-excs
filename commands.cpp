
//		commands.cpp
//********************************************
#include <fstream>
#include "commands.h"
#include <sys/sendfile.h>
#include "signals.h"
//********************************************
// function name: ExeCmd
// Description: interperts and executes built-in commands
// Parameters: pointer to jobs, command string
// Returns: 0 - success,1 - failure
//**************************************************************************************
#define SIGTEM_TIMEOUT 5
//TODO list:
//TODO check for invalid args in each command/more args than needed?. exmp1 - exit killer,exmp2 - exit kill ohNo
//TODO write and check an external prog that gets more than 1 arg
//TODO check clang tidy errors
//%%%%%%%%%%%%%%%%%%%%%%%    DONE    %%%%%%%%%%%%%%%%%%%%%%%%%%%
//TODO add cout for each signal sent (after before every kill(..)
//TODO make sure when we print clock time we cast to double
//TODO check number of recived args is legal
//TODO if a stopped process is getting fg, unstop it
//%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%

struct sigaction *smash_sigtstp_struct;
struct sigaction *smash_sigint_struct;

job::job(string title_of_job, int pid_num, clock_t time_of_exc)
{
    /*if(title_of_job.empty()||time_of_exc == -1)
       return NULL;
   else*/
    //{

    this->time_of_exc = time_of_exc;
    this->title_of_job = title_of_job;
    this->pid_num = pid_num;
    this->proc_num = ++job::org_num;
    //job::org_num++;

    //}
}
bool job::operator==(const job& rhs) const
{
    return (this->pid_num == rhs.pid_num);
}
void send_signal(int signum,int pid,std::string const &signame)
{
    if(!kill(pid,signum))
        cout<<"smash > signal"<< signame <<"was sent to pid "<<pid<<endl;//format ok
    else
        perror("smash error: > kill job – cannot send signal");
}

void print_jobs(const std::list <job*>& jobs)
{
    //std::list<job*>::const_iterator it;

    clock_t curr_time;
    double job_running_time;
    for (auto job : jobs)
    {
        curr_time = clock();
        job_running_time = (double)(curr_time - job->time_of_exc) / CLOCKS_PER_SEC;
        std::cout << "[" << job->proc_num << "] " << job->title_of_job << " " << job->pid_num << " " << job_running_time << " secs";//format ok
        if (job->stopped)
        {
            std::cout << " (stopped)" << endl;//format ok
        }
        else
        {
            std::cout << endl;//format ok
        }

    }

}



job& job::operator=(job& old_job) {
    this->proc_num = old_job.proc_num;
    this->title_of_job = old_job.title_of_job;
    this->pid_num = old_job.pid_num;
    this->time_of_exc = old_job.time_of_exc;
    this->stopped = old_job.stopped;
}







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
    bool run_in_bg = false;
    if (cmdString[strlen(cmdString) - 1] == '&') { // run in background
        cmdString[strlen(cmdString) - 1] = '\0';
        run_in_bg = true;
    }
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
    args[i + 1] = nullptr;

    /*************************************************/
    // Built in Commands PLEASE NOTE NOT ALL REQUIRED
    // ARE IN THIS CHAIN OF IF COMMANDS. PLEASE ADD
    // MORE IF STATEMENTS AS REQUIRED
    /*************************************************/
    if (!strcmp(cmd, "cd"))
    {
        char* cur_dir = getcwd(pwd, MAX_LINE_SIZE);
        if(args[2] != nullptr)
            cout << "smash error: > too many arguments"<<endl;
        else {
            if (!(strcmp(args[1], "-"))) { //going back a folder
                if (!(strcmp(prv_dir, ""))) {//TODO is this compare ok?
                    cout << "smash error: > no previous directory" << endl;//format ok
                } else {
                    if (chdir(prv_dir) == -1)
                        perror("smash error: > No such file or directory");//format ok
                    else {
                        cout << prv_dir << endl;//format ok
                        strcpy(prv_dir, cur_dir);
                    }
                }
            }
                // got an actual destination
            else {
                if (chdir(args[1]) == -1)
                    cout << "smash error: > No such file or directory" << endl;//format ok
                else {
                    cout << prv_dir << endl;//format ok
                    strcpy(prv_dir, cur_dir);
                }
            }
        }
    }

        /*************************************************/
    else if (!strcmp(cmd, "pwd"))
    {
        if (args[1] != nullptr) {
            char *path = getcwd(pwd, MAX_LINE_SIZE);
            if (path != nullptr) {
                cout << path << endl;//format ok
            } else {
                perror("smash error: > getcwd() error");//format ok
            }
        }//format ok
        else cout << "smash error: > too many arguments" << endl;//format ok
    }

        /*************************************************/



    else if (!strcmp(cmd, "jobs"))
    {
        if(args[1] != nullptr)
            cout << "smash error: > too many arguments"<<endl;//format ok
        else {
            print_jobs(jobs);
        }
    }
        /*************************************************/
    else if (!strcmp(cmd, "showpid"))
    {
        if(args[1] != nullptr)
            cout << "smash error: > too many arguments"<<endl;//format ok
        else {
            pid_t dad_pid = getppid();
            cout << "smash pid is " << dad_pid << endl;//format ok
        }
    }
        /*************************************************/
    else if (!strcmp(cmd, "fg")) {
        if (args[2] != nullptr) {
            cout << "smash error: > too many arguments" << endl;//format ok
        }
        else {
            int status;
            job *Pjob;
            //without noticing if anumber has bin given or not
            //key operations:
            //print process's name\
        //call wait with right arguments
            if ((*jobs.begin()) != nullptr) {
                auto max_proc_num_job = jobs.begin();
                auto cur_job = jobs.begin();

                if (args[1] == nullptr) {//move last activated process (max process num)
                    for (cur_job = jobs.begin(); cur_job != jobs.end(); cur_job++) {
                        if ((*cur_job)->proc_num > (*max_proc_num_job)->proc_num) {
                            max_proc_num_job = cur_job;
                        }
                    }
                    cout << (*max_proc_num_job)->title_of_job << endl;
                    Pjob = *cur_job;
                    cur_job = jobs.erase(cur_job);
                    run_in_fg(*cur_job, jobs);

                } else {
                    found = false;
                    long int process_num = strtol(args[1], nullptr, 10);
                    if (!process_num)
                        perror("smash error: > arguments couldn't be cast to int");//format ok
                    else {
                        for (cur_job = jobs.begin(); cur_job != jobs.end(); cur_job++) {
                            {
                                if ((*cur_job)->proc_num == process_num) {
                                    found = true;
                                    Pjob = *cur_job;
                                    cur_job = jobs.erase(cur_job);
                                    run_in_fg(*cur_job, jobs);
                                    break;
                                }
                            }
                        }
                        if (!found)
                            cout << "smash error: > process not found" << endl;//format ok
                    }
                }
            }else{
                cout << "smash error: > no processes to run in fg" << endl;//format ok
            }
        }
    }


        /*************************************************/
        /*************************************************/
    else if (!strcmp(cmd, "diff"))
    {
        if(args[3] != nullptr)
            cout << "smash error: > too many arguments"<<endl;//format ok
        else {
            bool diff = true;
            fstream f1, f2;
            char c1, c2;
            f1.open(args[1], ios::in);
            f2.open(args[2], ios::in);
            if ((f1.get() == EOF) || (f2.get() == EOF))
                perror("smash error: > problem with files");//format ok
            else {
                while (true) {
                    c1 = (char)f1.get();
                    c2 = (char)f2.get();
                    if (c1 != c2) {
                        diff = false;
                        break;
                    }
                    if ((c1 == EOF) || (c2 == EOF))
                        break;
                }
                f1.close();
                f2.close();
                if (!diff) cout << "1" << endl;//format ok
                else cout << "0" << endl;//format ok
            }
        }
    }
        /*************************************************/
    else if (!strcmp(cmd, "cp"))
    {
        if(args[3] != nullptr)
            cout << "smash error: > too many arguments"<<endl;//format ok
        else {
            //create new file
            int fd_out = open(args[2], O_RDWR | O_CREAT, 0666);
            //open old file
            int fd_in = open(args[1], O_RDONLY);
            if (fd_in == -1 || fd_out == -1)
                perror("smash error: > cp error");//format ok
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
                    perror("smash error: > cp error");//format ok
                else
                    cout << args[1] << " has been copied to " << args[2] << endl;//format ok
            }
        }
    }
        /*************************************************/
    else if (!strcmp(cmd, "bg")) {
        if(args[2] != nullptr)
            cout << "smash error: > too many arguments"<<endl;//format ok
        else {
            if (*(jobs.begin()) == nullptr)
                cout << "smash error: > no processes running!" << endl;
            else {
                if (args[1] == nullptr) {//bging newest stopped proccess
                    found = false;
                    for (auto cur_job = (--jobs.end()); cur_job != (--jobs.begin()); cur_job--) {
                        {
                            if ((*cur_job)->stopped) {
                                found = true;
                                cout << (*cur_job)->title_of_job<<endl;//format ok
                                send_signal(SIGCONT, (*cur_job)->pid_num, "SIGCONT");
                                (*cur_job)->stopped = false;
                                break;
                            }
                        }
                    }
                    if (!found)
                        cout << "smash error: > there are not any stopped processes" << endl;
                } else {//bging specific process
                    long int process_num = strtol(args[1], nullptr, 10);
                    if (!process_num)
                        cout << "smash error: > prccess number is invalid!" << endl;
                    else {
                        found = false;
                        for (auto cur_job : jobs) {
                            if ((*cur_job).proc_num == process_num) {
                                found = true;
                                if ((*cur_job).stopped) {
                                    cout << (*cur_job).title_of_job << endl;//format ok
                                    send_signal(SIGCONT, (*cur_job).pid_num, "SIGCONT");
                                    (*cur_job).stopped = false;
                                } else {
                                    cout << "smash error: > this process hasnt been stopped!" << endl;
                                }

                            }
                        }
                        if(!found) {
                            cout << "smash error: > couldnt find a process with this process number" << endl;
                        }
                    }
                }
            }
        }
    }
        /*************************************************/
    else if (!strcmp(cmd, "kill"))
    {
        if(args[3] != nullptr)
            cout << "smash error: > too many arguments"<<endl;
        else {
            found = false;
            long int proccess_num = strtol(args[2], nullptr, 10);
            long int signal = strtol((args[1] + 1), nullptr, 10);
            if (!proccess_num || !signal)
                perror("smash error: > arguments could not be cast to int");
            for (auto job : jobs) {
                 if ((*job).proc_num == proccess_num) {
                    found = true;
                    send_signal((int) signal, (*job).pid_num, (string) (args[1] + 1));
                    if ((int) signal == SIGSTOP)
                        (*job).stopped = 1;
                    if ((int) signal == SIGCONT)
                        (*job).stopped = 0;
                    break;
                }
            }
            if (!found)
                cout << "smash error: > kill job – job does not exist"<<endl;//format ok
        }
    }
        /*************************************************/
    else if (!strcmp(cmd, "quit"))
    {
        if(args[2] != nullptr)
            cout << "smash error: > too many arguments"<<endl;
        else {
            int status;
            bool sigterm_succeed = false;
            clock_t time_first_sig_sent;
            if (args[1] == nullptr || ((*jobs.begin()) == nullptr))
                exit(1);
            else if (!strcmp(args[1], "kill")) {
                for (auto job_it = jobs.begin(); job_it != jobs.end(); job_it++) {
                    kill((*job_it)->pid_num,SIGTERM);
                    cout<<"["<<(*job_it)->proc_num<<"]"<<(*job_it)->title_of_job<<"– Sending SIGTERM...";
                    time_first_sig_sent = clock();
                    while ( ((double)(clock() - time_first_sig_sent) / CLOCKS_PER_SEC) < SIGTEM_TIMEOUT) {
                        int wait_ret_Val = waitpid((*job_it)->pid_num, &status, WNOHANG);
                        if (wait_ret_Val == -1) {
                            perror("smash error: > waitpid failed from quit kill ");//TODO
                        } else {

                            if (WIFSIGNALED(status) && WTERMSIG(status) == SIGTERM)//TODO check what is going on with WIFSIGNALED here and all other places we used ths stuff
                            {
                                delete ((*job_it));
                                job_it = jobs.erase(job_it);
                                sigterm_succeed = true;
                                cout<<" Done."<<endl;
                            }
                        }
                    }
                    if (!sigterm_succeed) {
                        cout<<" (5 sec passed) Sending SIGKILL…"<<endl;
                        if(!kill((*job_it)->pid_num,SIGKILL)) {
                            cout<<" Done."<<endl;
                            delete ((*job_it));
                            job_it = jobs.erase(job_it);
                        }else
                            perror("smash error: > kill job – cannot send signal");
                    }
                }
                exit(1);
            } else
                cout << "smash error: > invalid argument for quit!" << endl;
        }
    }
        /*************************************************/
    else if (!strcmp(cmd, "history"))
    {
        if(args[1] != nullptr)
            cout << "smash error: > too many arguments"<<endl;
        else {
            list<string>::iterator it;
            for (it = history_commands.begin(); it != history_commands.end(); ++it) {
                cout <<*it << endl;
            }
        }
    }
        /**************************************************/

    else // external command
    {
        ExeExternal(args, cmdString, jobs, run_in_bg);
        return 0;
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
void ExeExternal(char* args[MAX_ARG], char* cmdString, std::list <job*>& jobs, bool bg)
{
    int pID, execv_ret_val, i, last_arg;

    switch (pID = fork())
    {
        case -1:
            perror("smash error: > fork fail at ExeExternal");
        case 0:
            // Child Process
            setpgrp();

            execv_ret_val = execvp(args[0], reinterpret_cast<char* const*>(args[1]));
            if (execv_ret_val == -1)// execv failed
                perror("smash error: > command execution failed");

        default://parent process
            smash_sigtstp_struct->sa_handler = &smash_sigtstp_handler;
            smash_sigint_struct->sa_handler = &smash_sigint_handler;
            sigaction(SIGINT,smash_sigint_struct,nullptr);
            sigaction(SIGTSTP,smash_sigtstp_struct,nullptr);
            job* new_job = new job(args[0], pID, clock());
            if (bg)
            {
                jobs.push_back(new_job);
            }
            else//run in fg
                run_in_fg(new_job, jobs);
    }
}



void update_jobs(list<job*>& jobs) {
    int status;

    if (*jobs.begin() != nullptr) {
        for (auto job_it = jobs.begin(); job_it != jobs.end(); job_it++) {
            if ((*job_it)->pid_num > 0) {
                int wait_ret_Val = waitpid((*job_it)->pid_num, &status, WNOHANG);
                if (wait_ret_Val == -1) {
                    perror("smash error: > update_jobs failed from waitpid");
                }
                else
                {

                    if (WIFEXITED(status) || (WIFSIGNALED(status) && WTERMSIG(status) != 32)) {
                        delete(*job_it);
                        job_it = jobs.erase(job_it);
                    }
                    else if (WIFSTOPPED(status))
                    {
                        (*job_it)->stopped = true;
                        cout << (*job_it)->title_of_job << "stopped by signal " << WSTOPSIG(status) << endl;

                    }
                    else if (WIFCONTINUED(status))
                    {
                        (*job_it)->stopped = false;
                        cout << (*job_it)->title_of_job << "continued" << endl;
                    }
                }

            }

        }
    }
}
void run_in_fg(job* cur_job, list<job*>& jobs)
{
    //unstop if stopped
    if((*cur_job).stopped) {
        (*cur_job).stopped = false;
        send_signal(SIGCONT,(*cur_job).pid_num,"SIGCONT");
    }

    fg_process_pid = (*cur_job).pid_num;
    int status;
    waitpid((*cur_job).pid_num, &status, 0);//TODO which option? insted of zero..
    fg_process_pid = NO_PROCESS_IN_FG;
    if (WIFEXITED(status) || (WIFSIGNALED(status) && WTERMSIG(status) != 32)) {
        delete(cur_job);
    }
    else if (WIFSTOPPED(status))
    {
        (*cur_job).stopped = true;
        jobs.push_back(cur_job);
    }


}

