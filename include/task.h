/**
 * @file task.h
 * @brief
 */

/*
 *   Copyright 2024 Hugo Abelheira, Mariana Rocha
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

/**
 * @struct Task
 * @brief Represents a task with associated information.
 */
typedef struct task{
    int pid; /**< Identifier of each process */
    struct timeval time_start; /**< Time when the task started */
    struct timeval time_end; /**< Time when the task ended */
    int time_spent; /**< Time the task actually took to execute */
    int time_expected; /**< Expected time for the task to complete */
    int command_flag; /**< Flag to determine which command the task corresponds to (status, execute -u, or execute -p) */
    char exec_args[500]; /**< Arguments of the task for its execution */
    int manager_id; /**< Identifier of the manager */
} Task;

/**
 * @struct Queue
 * @brief Represents a queue of tasks.
 */
typedef struct task_queue{
    int size; /**< Size of the queue */
    int qpointer; /**< Queue pointer */
    int last_added; /**< Position of the last added value */
    Task** tasks; /**< Array of pointers to tasks */
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
