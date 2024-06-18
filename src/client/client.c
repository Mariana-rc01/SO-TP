/**
 * @file client.c
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

/**
 * @brief The main function of the client program.
 *
 * @param argc The number of command-line arguments.
 * @param argv An array of strings containing the command-line arguments.
 * @return int The exit status of the program.
 */
int main(int argc, char* argv[]){
    Task task;

    if(argc == 2 && strcmp(argv[1], "status") == 0){
        task = processInput(argc, argv);
        char* name = myConcat("fifo_", task.pid);
        int flag = processTaskFromClient(task);

        int fifo_status = open(name, O_RDONLY);
        if (fifo_status < 0) {
            perror("Error on opening status fifo");
            exit(EXIT_FAILURE);
        }

        char buffer[1024]; // Buffer para ler as frases

        int bytes_read;
        int bytes_written;
        while ((bytes_read = read(fifo_status, buffer, sizeof(buffer))) > 0) {
            bytes_written = write(1, buffer, bytes_read);
            if (bytes_written < 0) {
                perror("Error writing to FIFO");
                break;
            }
        }

        if (bytes_read < 0) {
            perror("Error on reading from fifo_status");
        }

        close(fifo_status);

        return 0;
    }
    else if(argc > 4 && strcmp(argv[1], "execute") == 0){
        if(strcmp(argv[3], "-u") == 0){
            Task task1 = processInput(argc, argv);
            
            int fifo_server = open(SERVER, O_WRONLY);
            if(write(fifo_server, &task1, sizeof(Task)) == -1){
                perror("Error on writing in fifo");
                return -1;
            }
            close(fifo_server);

            fprintf(stdout, "\033[96m");
            fprintf(stdout,"Task %d\n", task1.pid);
            fprintf(stdout, "\033[96m");
        }
        else if(strcmp(argv[3], "-p") == 0){
            Task task2 = processInput(argc, argv);
            
            int fifo_server = open(SERVER, O_WRONLY);
            if(write(fifo_server, &task2, sizeof(Task)) == -1){
                perror("Error on writing in fifo");
                return -1;
            }
            close(fifo_server);

            fprintf(stdout, "\033[96m");
            fprintf(stdout,"Task %d\n", task2.pid);
            fprintf(stdout, "\033[96m");
        }
    }
    else{
        printMessageError(1);
        return -1;
    }
    
    return 0;
}

