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
struct job;
int ExeComp(char* lineSize);
int BgCmd(char* lineSize, list<int>& jobs);
int ExeCmd(list<int>& jobs, char* lineSize, char* cmdString, char* prv_dir, list<string>& history_commands);
void ExeExternal(char* args[MAX_ARG], char* cmdString);
#endif
