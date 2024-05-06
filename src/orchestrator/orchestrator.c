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
#include <sys/wait.h>
#include <sys/time.h>

void manager(int id, char* output_folder, Task task){

    if (task.command_flag == FLAG_U){
        exec_command_flag_u(task.exec_args, task.pid, output_folder);
    }
    else if (task.command_flag == FLAG_P){
        exec_command_flag_p(task.exec_args, task.pid, output_folder);
    }
    gettimeofday(&task.time_end, NULL);

    task.time_spent = calculateElapsedTime(task.time_start, task.time_end);
    task.manager_id = id;

    char log_file[50];
    sprintf(log_file, "../%s/log.txt", output_folder);
    int log_fd = open(log_file, O_WRONLY | O_APPEND, 0666);
    char log_message[100];

    char* program = extractProgram(task.exec_args, task.command_flag);
    snprintf(log_message, sizeof(log_message), "%d %s %d ms\n", task.pid, program, task.time_spent);

    int original_stout = dup(STDOUT_FILENO);

    if (dup2(log_fd, STDOUT_FILENO) == -1) {
        perror("Error redirecting stdout");
        exit(EXIT_FAILURE);
    }
    
    if (write(1, log_message, strlen(log_message)) < 0) {
        perror("Error writing to log file");
    }

    close(log_fd);
    dup2(original_stout,STDOUT_FILENO);
    close(original_stout);


    int fifo_write =  open(SERVER, O_WRONLY);
    int bytes_written = write(fifo_write, &task, sizeof(Task));
    while (bytes_written < 0){
        bytes_written = write(fifo_write,&task,sizeof(Task));
    }
}

int main(int argc, char* argv[]){
    int flag_sched_policy = 0;
    if(argc != 4){
        printMessageError(0);
    }
    else {
        toUpperCase(argv[3]);
        if(strcmp(argv[3], "FCFS") == 0){
            flag_sched_policy = 0;
            char message[500];
            int length;
            length = snprintf(message, sizeof(message), "\033[91m[The orchestrator is running with the first come first served (FCFS) scheduling policy.]\033[0m\n");
            length = write(1, message, length);
        }
        else if(strcmp(argv[3], "SJF") == 0){
            char message[500];
            int length;
            length = snprintf(message, sizeof(message), "\033[1;91m[The orchestrator is running with the shortest job first (SJF) scheduling policy.]\033[0m\n");
            length = write(1, message, length);
            flag_sched_policy = 1;
        }
        else {
            printMessageError(0);
            return -1;
        }
    }

    if(mkfifo(SERVER, 0666) < 0){
        perror("Error on creating fifo");
        return -1;
    }

    int NUM_TASKS_MAX = atoi(argv[2]);
    int num_tasks_running = 0;
    int manager_free[NUM_TASKS_MAX]; // 0 - livre, 1 - ocupado
    int manager_fifo[NUM_TASKS_MAX];


    char* output_folder = argv[1];

    char log_file[50];
    sprintf(log_file, "../%s/log.txt", output_folder);
    int log_fd = open(log_file, O_WRONLY | O_CREAT | O_APPEND, 0666);
    if (log_fd == -1) {
        perror("Error opening log file");
        return -1;
    }
    close(log_fd);

    int fifo_read = open(SERVER, O_RDONLY);
    int fifo_write_Open = open(SERVER, O_WRONLY);

    for(int i = 0; i < NUM_TASKS_MAX; i++){
        char* name_fifo = myConcat("manager_",i);
        if (mkfifo(name_fifo,0666) < 0){
            perror("Error on creating fifo");
            free(name_fifo);
            return -1;
        }
        free(name_fifo);
        manager_free[i] = 0;
    }
    Queue* queue = createTaskQueue(255);

    int read_bytes = 0;
    Task task;

    while(1){

        if ((read_bytes = read(fifo_read, &task, sizeof(Task))) > 0) {
            if (task.manager_id == -1){
                processTaskFromServer(task, queue, output_folder, flag_sched_policy);
            }
            else {
                int free = task.manager_id;
                changeTaskInQueue(queue, task);
                char message[100];
                int length;
                length = snprintf(message, sizeof(message), "\033[0;93m[Task %d done]\033[0m\n", task.pid);
                length = write(1, message, length);
                if(num_tasks_running > 0) num_tasks_running--;
                manager_free[free] = 0;
            }
        }

        if (num_tasks_running < NUM_TASKS_MAX){
            int free = freeManager(manager_free,NUM_TASKS_MAX);
            if (free != -1){
                if (queue->qpointer < queue->last_added) {
                    Task new_task = *(queue->tasks[queue->qpointer]);
                    queue->qpointer++;
                    manager_free[free] = 1;
                    if (fork() == 0){
                        char message[100];
                        int length;
                        length = snprintf(message, sizeof(message), "\033[0;32m[Task %d ready to send to the manager]\033[0m\n", new_task.pid);
                        length = write(1, message, length);
                        manager(free, output_folder, new_task);
                        _exit(0);
                    }
                    else num_tasks_running++;
                }
            }
        }
    }

    freeQueue(queue);
    close(fifo_read);

    return 0;
}
