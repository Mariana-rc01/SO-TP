/**
 * @file utils.h
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

#ifndef UTILS_H
#define UTILS_H

#include <sys/time.h>

char* myConcat(const char* s1, int num);

void toUpperCase(char* str);

char* extractProgramU(char* string);

char* extractProgram(char* string, int flag_command);

char** splitString(char* string, char* delimiter, int* num_commands);

int freeManager(int managers[], int N);

int calculateElapsedTime(struct timeval start, struct timeval end);

void printMessageError(int flag);

#endif
