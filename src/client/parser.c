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

Task process_input(char* data[], int argc){
    int i;
    struct timeval tv;

    Task task = {
        .pid = getpid(),
        .time_start = gettimeofday(&tv, NULL),
        .time_expected = atoi(data[2]),
    };
    
    for(i = 0; i < argc - 4; i++){
        //strcpy(task.exec_args[i], strdup(data[i+4]));
        task.exec_args[i] = data[i+4];
    }
    task.exec_args[i] = NULL;

    return task;
}
