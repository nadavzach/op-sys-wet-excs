#ifndef _COMMANDS_H
#define _COMMANDS_H
#include <unistd.h>
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <sys/sendfile.h>
#include <fcntl.h>
#include <string.h>
#include <iostream>
#include <map>
#include <vector>
#include <list>
#include <iterator>
using namespace std;
#define MAX_LINE_SIZE 80
#define MAX_ARG 20
#define MAX_HISTORY 50

class job {
public:
    //const
    job(string title_of_job, int pid_num,clock_t time_of_exc);


//static vars
    static int org_num;
    //struct fields
    int ;
    string title_of_job;
    int pid_num;
    clock_t time_of_exc;
    int stopped = 0;

    //const&dest
    job();
    //cpy const
    job(const job &old_job){
        org_num=old_job.org_num;
        title_of_job = old_job.title_of_job;
        pid_num = old_job.pid_num;
        time_of_exc = old_job.time_of_exc;
        stopped = old_job.stopped;
    }

    //operators asasasas
    job& operator=(job& old_job);
    bool operator==(const job& rhs);

    //methods


};



void print_jobs( const std::list <job*>& jobs);

int ExeComp(char* lineSize);
int BgCmd(char* lineSize, list<job*>& jobs);
int ExeCmd(list<job&>& jobs, char* lineSize, char* cmdString, char* prv_dir, list<string>& history_commands);
void ExeExternal(char* args[MAX_ARG], char* cmdString,std::list <job*>& jobs);
#endif
