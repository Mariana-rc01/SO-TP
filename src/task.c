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
#include <sys/wait.h>

Queue* createTaskQueue(int size){
    Queue* new = malloc(sizeof(struct task_queue));
    new->size = size;
    new->qpointer = 0;
    new->last_added = 0;
    new->tasks = malloc(sizeof(Task*) * size);

    return new;
}

void insertTask(Task* task, Queue* queue){
    if(queue->last_added == queue->size){
        queue->size *= 2;
        queue->tasks = realloc(queue->tasks, sizeof(Task*) * queue->size);
        if(queue->tasks == NULL){
            perror("Failed realloc");
            exit(EXIT_FAILURE);
        }
    }

    queue->tasks[queue->last_added] = task;
    char message[100];
    int length;
    length = snprintf(message, sizeof(message), "\033[95m[Task %d added to queue]\033[0m\n", queue->tasks[queue->last_added]->pid);
    length = write(1, message, length);
    queue->last_added++;
}

void insertTaskSorted(Task* task, Queue* queue){
    if(queue->last_added == queue->size){
        queue->size *= 2;
        queue->tasks = realloc(queue->tasks, sizeof(Task*) * queue->size);
        if(queue->tasks == NULL){
            perror("Failed realloc");
            exit(EXIT_FAILURE);
        }
    }

    int insert_index = queue->qpointer;

    while (insert_index < queue->last_added && queue->tasks[insert_index]->time_expected <= task->time_expected) {
        insert_index++;
    }

    for (int i = queue->last_added; i > insert_index; i--) {
        queue->tasks[i] = queue->tasks[i - 1];
    }

    queue->tasks[insert_index] = task;
    char message[100];
    int length;
    length = snprintf(message, sizeof(message), "\033[95m[Task %d added to queue]\033[0m\n", queue->tasks[insert_index]->pid);
    length = write(1, message, length);
    queue->last_added++;
}

void freeQueue(Queue* queue){
    for(int i = 0; i < queue->qpointer; i++){
        free(queue->tasks[i]);
    }
    free(queue);
}

int processTaskFromClient(Task task){
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

    return 0;
}

void processStatus(char* name, Queue* queue, char* pathOutput) {

    int bytes_written;
    int fifo_status = open(name, O_WRONLY);
    if (fifo_status == -1) {
        perror("Error opening status FIFO for writing");
        return;
    }

    int original_stout = dup(STDOUT_FILENO);
    if (dup2(fifo_status, STDOUT_FILENO) == -1 || dup2(fifo_status, STDERR_FILENO) == -1) {
        perror("Error redirecting stdout/stderr");
        close(fifo_status);
        return;
    }
    close(fifo_status);

    // Print tasks scheduled
    bytes_written = write(1, "\033[91mTasks Scheduled:\n", 22);
    bytes_written = write(1, "\033[0m", 4);
    for (int i = queue->qpointer + 1; i < queue->last_added; i++) {
        int pid = queue->tasks[i]->pid;
        char* command = extractProgram(queue->tasks[i]->exec_args, queue->tasks[i]->command_flag);
        int len = snprintf(NULL, 0, "%d %s\n", pid, command);
        char* result = (char*)malloc(len + 1);
        snprintf(result, len + 1, "%d %s\n", pid, command);
        bytes_written = write(1, result, len + 1);
        free(command);
        free(result);
    }

    // Print tasks executing
    bytes_written = write(1, "\033[94mTasks Executing:\n", 22);
    bytes_written = write(1, "\033[0m", 4);
    for (int i = 0; i < queue->qpointer; i++) {
        if (queue->tasks[i]->manager_id == -1) {
            int pid = queue->tasks[i]->pid;
            char* command = extractProgram(queue->tasks[i]->exec_args, queue->tasks[i]->command_flag);
            int len = snprintf(NULL, 0, "%d %s\n", pid, command);
            char* result = (char*)malloc(len + 1);
            snprintf(result, len + 1, "%d %s\n", pid, command);
            bytes_written = write(1, result, len + 1);
            free(command);
            free(result);
        }
    }

    // Print tasks completed
    bytes_written = write(1, "\033[93mTasks Completed:\n", 22);
    bytes_written = write(1, "\033[0m", 4);
    char log_file[50];
    sprintf(log_file, "../%s/log.txt", pathOutput);
    int log_fd = open(log_file, O_RDONLY);
    if (log_fd == -1) {
        perror("Error opening log file for reading");
        close(fifo_status);
        return;
    }

    char buffer[1024];
    int bytes_read;
    while ((bytes_read = read(log_fd, buffer, sizeof(buffer))) > 0) {
        bytes_written = write(1, buffer, bytes_read);
        if (bytes_written < 0) {
            perror("Error writing to FIFO");
            break;
        }
    }

    close(log_fd);

    dup2(original_stout, STDOUT_FILENO);
    close(original_stout);

    (void) bytes_written;
}


void processTaskFromServer(Task task, Queue* queue, char* pathOutput, int flag_sched_policy){
    if(task.command_flag == STATUS){
        if (fork() == 0){
            char* name = myConcat("fifo_",task.pid);
            processStatus(name, queue, pathOutput);
            _exit(1);
        }
        wait(NULL);
    }
    else{
        Task* task_copy = malloc(sizeof(Task)); // Aloca memória para uma nova cópia da tarefa
        if (task_copy == NULL) {
            perror("Failed to allocate memory");
            exit(EXIT_FAILURE);
        }
        memcpy(task_copy, &task, sizeof(Task)); // Copia os dados da tarefa original para a cópia
        if (flag_sched_policy == 0){
            insertTask(task_copy, queue); // Adiciona a cópia da tarefa à fila
        }

        if (flag_sched_policy == 1){
            insertTaskSorted(task_copy,queue);
        }
    }
}

int exec_command_flag_u(char* arg, int id, char* pathOutput) {

    char* exec_args[MAX_COMMANDS];

    char* string;
    int exec_ret = 0;
    int i = 0;

    char* command = strdup(arg);

    string = strtok(command, " ");

    while (string != NULL) {
        exec_args[i] = string;
        string = strtok(NULL, " ");
        i++;
    }

    exec_args[i++] = NULL;

    char output_file[50];
    sprintf(output_file, "../%s/output_%d.txt", pathOutput, id);

    int output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
    if (output_fd == -1) {
        perror("Error opening output file");
        exit(EXIT_FAILURE);
    }

    int original_stout = dup(STDOUT_FILENO);

    if (dup2(output_fd, STDOUT_FILENO) == -1) {
        perror("Error redirecting stdout");
        exit(EXIT_FAILURE);
    }

    if (dup2(output_fd, STDERR_FILENO) == -1) {
        perror("Error redirecting stderr");
        exit(EXIT_FAILURE);
    }

    close(output_fd);

    int child_pid;
    int res = -1;
	if ((child_pid = fork()) == 0){
		int return_value = execvp(exec_args[0],exec_args);
		_exit(return_value);
	}
	else if (child_pid < 0){
		return res;
	}
	else{
		int status;
		wait(&status);
		if (WIFEXITED(status)){
			res = WEXITSTATUS(status);
		}
	}

    dup2(original_stout,STDOUT_FILENO);
    close(original_stout);

    return res;
}

int exec_command(char* arg){

    //Estamos a assumir numero maximo de argumentos
    char* exec_args[MAX_COMMANDS];

    char* string;
    int exec_ret = 0;
    int i=0;

    char* command = strdup(arg);

    string = strtok(command," ");
    
    while(string != NULL){
        exec_args[i] = string;
        string = strtok(NULL," ");
        i++;
    }

    exec_args[i] = NULL;
    
    exec_ret = execvp(exec_args[0],exec_args);
    
    return exec_ret;
}

int exec_command_flag_p(char* arg, int id, char* pathOutput){

    int number_of_commands = 0;

    char** commands = splitString(arg, "|", &number_of_commands);

    int pipes[number_of_commands-1][2];
    int status[number_of_commands];

    for (int c = 0; c < number_of_commands; c++){
        if (c == 0){ // 1º
            if (pipe(pipes[c]) != 0){
                perror("pipe");
                return 1;
            }

            switch(fork()){
                case -1:
                    perror("fork");
                    return -1;

                case 0:

                    close(pipes[c][0]);

                    dup2(pipes[c][1],1);

                    exec_command(commands[c]);

                    _exit(0);

                default:
                    close(pipes[c][1]);
            }

        
        }
        else if (c == number_of_commands - 1){ // último

            switch(fork()){
                case -1:
                    perror("fork");
                    return -1;

                case 0:

                    close(pipes[c][0]);

                    dup2(pipes[c][1],1);
                    close(pipes[c][1]);

                    dup2(pipes[c-1][0],0);
                    close(pipes[c-1][0]);

                    char output_file[50];
                    sprintf(output_file, "../%s/output_%d.txt", pathOutput, id);
                    int output_fd = open(output_file, O_WRONLY | O_CREAT | O_TRUNC, 0666);
                    if (output_fd == -1) {
                        perror("Error opening output file");
                        exit(EXIT_FAILURE);
                    }
                    // Redirecionar stdout e stderr para o arquivo
                    dup2(output_fd, STDOUT_FILENO);
                    dup2(output_fd, STDERR_FILENO);

                    exec_command(commands[c]);
                    _exit(0);

                default:
                    close(pipes[c-1][0]);
            }
        }
        else{ // intermédios
            if (pipe(pipes[c]) != 0){
                perror("pipe");
                return 1;
            }

            switch(fork()){
                case -1:
                    perror("fork");
                    return -1;

                case 0:

                    close(pipes[c][0]);

                    dup2(pipes[c][1],1);
                    close(pipes[c][1]);

                    dup2(pipes[c-1][0],0);
                    close(pipes[c-1][0]);

                    exec_command(commands[c]);
                    _exit(0);

                default:
                    close(pipes[c][1]);
                    close(pipes[c-1][0]);
            }
        }
    }

    for (int i = 0; i < number_of_commands; i++){
        wait(&status[i]);
    }

    return 0;
}

void changeTaskInQueue(Queue* queue, Task task){
    // Procura pela tarefa na fila usando o PID
    int found_task_index = -1;
    for (int i = 0; i < queue->qpointer; i++) {
        if (queue->tasks[i]->pid == task.pid) {
            found_task_index = i;
            break;
        }
    }

    // Atualiza o tempo gasto e o ID
    if (found_task_index != -1) {
        queue->tasks[found_task_index]->time_spent = task.time_spent;
        queue->tasks[found_task_index]->manager_id = task.manager_id;
    }
}
