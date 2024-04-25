/**
 * @file task.c
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

#include "task.h"
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

Queue createTaskQueue(int size){
    Queue new = malloc(sizeof(struct task_queue));
    new->size = size;
    new->qpointer = 0;
    new->tasks = malloc(sizeof(Task*) * size);

    return new;
}

void insertTask(Task* task, Queue queue){
    if(queue->qpointer == queue->size){
        queue->size *= 2;
        queue->tasks = realloc(queue->tasks, sizeof(Task*) * queue->size);
        if(queue->tasks == NULL){
            perror("Failed realloc");
            exit(EXIT_FAILURE);
        }
    }

    queue->tasks[queue->qpointer] = task;
    queue->qpointer++;
}

void freeQueue(Queue queue){
    for(int i = 0; i < queue->qpointer; i++){
        free(queue->tasks[i]);
    }

    free(queue); // possivel seg fault
}

void processTaskFromClient(Task task){
    char* name = myConcat("fifo_", task.pid);
    if(mkfifo(name,0666) < 0){
        perror("Error on creating fifo");
        return -1;
    }
    
    int fifo_server = open(SERVER, O_WRONLY);
    if(write(fifo_server, &task, sizeof(Task)) == -1){
        perror("Error on writing in fifo");
        return -1;
    }
    close(fifo_server);

    printf("Task %d received\n", task.pid);
}

void processStatus(Task task, Queue queue){
    int i = 0;
    char* name = myConcat("fifo_",task.pid);
    int fifo_server = open(SERVER, O_WRONLY);

    write(1, "Tasks Executing:\n", 17);
    int pid = queue->tasks[queue->qpointer]->pid;
    char* command = queue->tasks[queue->qpointer]->exec_args[0];
    int len = snprintf(NULL, 0, "%d %s\n", pid, command);
    char* result = (char*)malloc(len+1);
    snprintf(result, len + 1, "%d %s\n", pid, command);
    write(1, result, len+1);

    write(1, "Tasks Scheduled:\n", 17);
    for(i = queue->qpointer + 1; i < queue->size && queue->tasks[i] != NULL; i++){
        int pid = queue->tasks[i]->pid;
        char* command = queue->tasks[i]->exec_args[0];
        int len = snprintf(NULL, 0, "%d %s\n", pid, command);
        char* result = (char*)malloc(len+1);
        snprintf(result, len + 1, "%d %s\n", pid, command);
        write(1, result, len+1);
    }

    write(1, "Tasks Completed:\n", 17);
    for(i = 0; i < queue->qpointer; i++){
        int pid = queue->tasks[i]->pid;
        char* command = queue->tasks[i]->exec_args[0];
        int task_duration = queue->tasks[i]->time_end;
        int len = snprintf(NULL, 0, "%d %s %d\n", pid, command, task_duration);
        char* result = (char*)malloc(len+1);
        snprintf(result, len + 1, "%d %s %d\n", pid, command, task_duration);
        write(1, result, len+1);
    }
}

void processTaskFromServer(Task task, Queue queue){
    if(task.command_flag == STATUS){
        int pid;
        if((pid = fork()) == 0){
            processStatus(task, queue);
            exit(EXIT_SUCCESS);
        }
    }
    else{
        insertTask(&task, queue);
    }

}
