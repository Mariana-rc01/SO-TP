/**
 * @file parser.c
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

#include "client/parser.h"

#include <stdio.h>
#include <stdlib.h>

void process_input(char* data, requested_function func){
    
    int res = -1;
    char* exec_args[MAXN];
    int i = 0;
    char* comando = strdup(data);
    char* token;

    while((token = strsep(&comando, " ")) != NULL){
        exec_args[i] = strdup(token);
        i++;
    }
    // a partir daqui tem que ir para tratamento de comando do orquestrador

}