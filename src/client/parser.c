/**
 * @file parser.c
 * @brief 
 */

/*
 *   Copyright 2023 Ana Pires, Hugo Abelheira, Mariana Rocha
 *
 *   Licensed under the Apache License, Version 2.0 (the "License");
 *   you may not use this file except in compliance with the License.
 *   You may obtain a copy of the License at
 *
 *       http://www.apache.org/licenses/LICENSE-2.0
 *
 *   Unless required by applicable law or agreed to in writing, software
 *   distributed under the License is distributed on an "AS IS" BASIS,
 *   WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 *   See the License for the specific language governing permissions and
 *   limitations under the License.
 */

#include "client/parser.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <string.h>
#include <unistd.h>

Task processInput(int argc, char* data[]){

    if(strcmp(data[1], "status") == 0){
        Task task;
        task.pid = getpid();
        task.command_flag = STATUS;
        task.manager_id = -1;
        return task;
    }
    
    int i;
    struct timeval tv;
    
    Task task = {
        .pid = getpid(),
        .time_spent = 0,
        .time_expected = atoi(data[2]),
        .command_flag = FLAG_U,
        .manager_id = -1,
    };

    gettimeofday(&task.time_start, NULL);
    gettimeofday(&task.time_end, NULL);
    for (int i = 4; i < argc; i++) {
        strcat(task.exec_args, data[i]);
        strcat(task.exec_args, " ");
    }

    if(strcmp(data[3], "-p") == 0){
        task.command_flag = FLAG_P;
    }
    else{
        task.command_flag = FLAG_U;
    }

    return task;
}
