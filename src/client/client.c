/**
 * @file client.c
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

#include "client/client.h"
#include "client/parser.h"
#include "utils.h"
#include "task.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

int main(int argc, char* argv[]){
    Task task;

    if(argc == 2 && strcmp(argv[1], "status") == 0){
        printf("Status a ser implementado no futuro\n");
        task = processInput(argc, argv);
    }
    else if(argc > 4 && strcmp(argv[1], "execute") == 0){
        if(strcmp(argv[3], "-u") == 0){
            printf("Flag -u\n");
            for(int i = 0; i < argc; i++){
                printf("%s\n",argv[i]);
            }
            task = processInput(argc, argv);
        }
        else if(strcmp(argv[3], "-p") == 0){
            // vai para o parser de pipeline
            printf("Flag -p\n");
        }
    }
    else{
        printf("Task not found !!\n");
        return -1;
    }
    
    processTaskFromClient(task);

    return 0;
}

