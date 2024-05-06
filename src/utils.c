/**
 * @file utils.c
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
#include "utils.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <sys/time.h>
#include <ctype.h>

char* myConcat(const char* s1, int num){
    int len = snprintf(NULL, 0, "%s%d", s1, num);

    char* result = (char*)malloc(len+1);

    snprintf(result, len + 1, "%s%d", s1, num);

    return result;
}

void toUpperCase(char* str) {
    while (* str) {
        *str = toupper(*str);
        str++;
    }
}

char* extractProgramU(char* string){
    char* copy = strdup(string);
    if (copy == NULL) {
        perror("Error duplicating string");
        exit(EXIT_FAILURE);
    }

    char* token = strtok(copy, " ");

    if (token == NULL) {
        free(copy);
        return strdup(string);
    }

    return strdup(token);
}


char* extractProgram(char* string, int command_flag){
    char* stringFinal = NULL;
    if (command_flag == 1){
        return extractProgramU(string);
    }
    else{
        int commandsN = 0;
        char** commands = splitString(string, "|",&commandsN);
        int i = 0;
        for (i = 0; i < commandsN; i++){
            char* firstWord = extractProgramU(commands[i]);
            if (stringFinal == NULL) {
                stringFinal = strdup(firstWord);
            } else {
                stringFinal = realloc(stringFinal, strlen(stringFinal) + strlen(firstWord) + 3);
                if (stringFinal == NULL) {
                    perror("Error allocating memory");
                    exit(EXIT_FAILURE);
                }
                strcat(stringFinal, " | ");
                strcat(stringFinal, firstWord);
            }
        }
    }
    return stringFinal;
}


char** splitString(char* string, char* delimiter, int* num_commands) {
    char** args = malloc(300 * sizeof(char*));
    if (args == NULL) {
        perror("Error allocating memory");
        exit(EXIT_FAILURE);
    }

    int i = 0;

    char* copy = strdup(string);
    if (copy == NULL) {
        perror("Error duplicating string");
        exit(EXIT_FAILURE);
    }

    char* token = strtok(copy, delimiter);

    while (token != NULL) {
        args[i] = strdup(token);
        if (args[i] == NULL) {
            perror("Error allocating memory");
            exit(EXIT_FAILURE);
        }

        i++;

        token = strtok(NULL, delimiter);
    }

    free(copy);

    *num_commands = i;

    return args;
}

int freeManager(int managers[], int N){
    for (int i = 0; i < N; i++){
        if (managers[i] == 0) return i;
    }
    return -1;
}

int calculateElapsedTime(struct timeval start, struct timeval end) {
    long seconds = end.tv_sec - start.tv_sec;
    long micros = end.tv_usec - start.tv_usec;

    // Converta os segundos e microssegundos para milissegundos
    int time_spent_ms = (seconds * 1000) + (micros / 1000);
    return time_spent_ms;
}

void printMessageError(int flag) {
    char message[1000];
    int length = 0;
    
    if (flag == 0) {
        length = snprintf(message, sizeof(message), "\033[91m[Error in input arguments]\033[0m\n");
        length += snprintf(message + length, sizeof(message) - length, "\033[96m[Usage]\033[0m\n");
        length += snprintf(message + length, sizeof(message) - length, "$ \033[1;32m./orchestrator\033[0m \033[1;33moutput_folder parallel-tasks sched-policy\033[0m\n");
        length += snprintf(message + length, sizeof(message) - length, "\033[1;33m[Output folder:\033[0m \033[0;36mdirectory to save task output files.\033[1;33m]\033[0m\n");
        length += snprintf(message + length, sizeof(message) - length, "\033[1;33m[Parallel tasks:\033[0m \033[0;36mmaximum number of tasks that can run simultaneously.\033[1;33m]\033[0m\n");
        length += snprintf(message + length, sizeof(message) - length, "\033[1;33m[Scheduling policy:\033[0m \033[0;36midentifier of the scheduling policy (e.g., FCFS or SJF).\033[1;33m]\033[0m\n");
    }
    else if (flag == 1) {
        length = snprintf(message, sizeof(message), "\033[91mError in input arguments\033[0m\n");
        length += snprintf(message + length, sizeof(message) - length, "\033[96m[Usage:]\033[0m\n");
        length += snprintf(message + length, sizeof(message) - length, "$ \033[1;32m./client <command> [options]\033[0m\n");
        length += snprintf(message + length, sizeof(message) - length, "Commands:\n");
        length += snprintf(message + length, sizeof(message) - length, "\033[1;33m1. execute -u \"prog-a [args]\":\033[0m \033[0;36mexecute a single task\033[0m\n");
        length += snprintf(message + length, sizeof(message) - length, "\033[1;33m2. execute -p \"prog-a [args] | prog-b [args] | prog-c [args]\":\033[0m \033[0;36mexecute a pipeline of tasks\033[0m\n");
        length += snprintf(message + length, sizeof(message) - length, "\033[1;33m3. status:\033[0m \033[0;36mcheck status of tasks\033[0m\n");
        length += snprintf(message + length, sizeof(message) - length, "Options:\n");
        length += snprintf(message + length, sizeof(message) - length, "\033[1;33m- time:\033[0m \033[0;36mindication of the estimated time, in milliseconds, for task execution.\033[0m\n");
        length += snprintf(message + length, sizeof(message) - length, "\033[1;33m- prog-a:\033[0m \033[0;36mthe name/path of the program to execute.\033[0m\n");
        length += snprintf(message + length, sizeof(message) - length, "\033[1;33m- [args]:\033[0m \033[0;36mthe arguments of the program, if any.\033[0m\n");
    }

    length = write(1, message, length);
}

