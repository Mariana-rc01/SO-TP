/**
 * @file orchestrator.c
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

#include "orchestrator/orchestrator.h"
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

    if(argc != 4){
        printf("Error in input arguments\n");
    }
    else if(strcmp(argv[3], "FCFS") == 0){
        printf("Chamar queue sem prioridade aqui\n");
    }
    else if(strcmp(argv[3], "SJF") == 0){
        printf("Chamar queue com prioridade aqui\n");
    }

    if(mkfifo(SERVER, 0666) < 0){
        perror("Error on creating fifo");
    }

    int fifo_read = open(SERVER, O_RDONLY); // fifo para o server ler os pedidos do cliente
    int fifo_write =  open(SERVER, O_WRONLY); // fifo para o server se manter aberto

    int read_bytes = 0;
    Task task;

    while((read_bytes = read(fifo_read, &task, sizeof(Task))) > 0){
        char* client = my_concat("fifo_", task.pid);

        printf("%s\n", client);

    }
    close(fifo_read);

    return 0;
}
