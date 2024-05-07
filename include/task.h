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
    int time_spent; // tempo que a task efetivamente demorou a executar
    int time_expected; // tempo que o utilizador pensa que a tarefa irá demorar
    int command_flag; // flag para determinar a que comando corresponde a task                                 (status, execute -u ou execute -p)
    char exec_args[500]; // argumentos da task para a sua execução
    int manager_id; // identificador do manager
} Task;

typedef struct task_queue{
    int size;
    int qpointer;
    int last_added; // Posição do último valor adicionado
    Task** tasks;
} Queue;

/**
 * @brief Creates a new task queue.
 *
 * @param size The initial size of the queue.
 * @return Queue* A pointer to the newly created queue.
 */
Queue* createTaskQueue(int size);

/**
 * @brief Inserts a task into the queue.
 *
 * @param task The task to insert.
 * @param queue The queue to insert into.
 */
void insertTask(Task* task, Queue* queue);

/**
 * @brief Inserts a task into the queue in a sorted manner based on expected time.
 *
 * @param task The task to insert.
 * @param queue The queue to insert into.
 */
void insertTaskSorted(Task* task, Queue* queue);

/**
 * @brief Frees the memory allocated for a task queue.
 *
 * @param queue The queue to free.
 */
void freeQueue(Queue* queue);

/**
 * @brief Processes a task sent from a client.
 *
 * @param task The task to process.
 * @return int 0 if successful, -1 otherwise.
 */
int processTaskFromClient(Task task);

/**
 * @brief Processes the status command.
 *
 * @param name The name of the FIFO.
 * @param queue The task queue.
 * @param pathOutput The path to the output directory.
 */
void processStatus(char* name, Queue* queue, char* pathOutput);

/**
 * @brief Processes a task received from the server.
 *
 * @param task The task to process.
 * @param queue The task queue.
 * @param pathOutput The path to the output directory.
 * @param flag_sched_policy Flag indicating the scheduling policy.
 */
void processTaskFromServer(Task task, Queue* queue, char* pathOutput, int flag_sched_policy);

/**
 * @brief Executes a command with flag '-u'.
 *
 * @param arg The argument string.
 * @param id The ID of the task.
 * @param pathOutput The path to the output directory.
 * @return int The exit status of the command.
 */
int exec_command_flag_u(char* arg, int id, char* pathOutput);

/**
 * @brief Executes a command without any flags.
 *
 * @param arg The argument string.
 * @return int The exit status of the command.
 */
int exec_command(char* arg);

/**
 * @brief Executes a command with flag '-p'.
 *
 * @param arg The argument string.
 * @param id The ID of the task.
 * @param pathOutput The path to the output directory.
 * @return int The exit status of the command.
 */
int exec_command_flag_p(char* arg, int id, char* pathOutput);

/**
 * @brief Changes the status of a task in the queue.
 *
 * @param queue The task queue.
 * @param task The task with updated information.
 */
void changeTaskInQueue(Queue* queue, Task task);


#endif
