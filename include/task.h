/**
 * @file task.h
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

#ifndef TASK_H
#define TASK_H
#define SERVER "fifo_server"
#define STATUS 0
#define FLAG_U 1
#define FLAG_P 2
#define MAX_COMMANDS 20

#include <sys/time.h>

typedef struct task{
    int pid; // identificador de cada processo
    struct timeval time_start; // tempo em que a tarefa comecou
    struct timeval time_end; // tempo em que a tarefa terminou
    int time_spent;
    int time_expected; // tempo que o utilizador pensa que a tarefa irá demorar
    int command_flag; // flag para determinar a que comando corresponde a task (status, execute -u ou execute -p)
    char exec_args[500]; // argumentos da task para a sua execução
    int manager_id;
} Task;

typedef struct task_queue{
    int size;
    int qpointer;
    int last_added; // Posição do último valor adicionado
    Task** tasks;
} Queue;

Queue* createTaskQueue(int size);

void insertTask(Task* task, Queue* queue);

void insertTaskSorted(Task* task, Queue* queue);

void freeQueue(Queue* queue);

int processTaskFromClient(Task task);

void processStatus(char* name, Queue* queue, char* pathOutput);

void processTaskFromServer(Task task, Queue* queue, char* pathOutput, int flag_sched_policy);

int exec_command_flag_u(char* arg, int id, char* pathOutput);

int exec_command(char* arg);

int exec_command_flag_p(char* arg, int id, char* pathOutput);

void changeTaskInQueue(Queue* queue, Task task);

#endif
