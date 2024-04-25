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

#include <stdlib.h>
#include <stdio.h>

char* myConcat(const char* s1, int num){
    int len = snprintf(NULL, 0, "%s%d", s1, num);

    char* result = (char*)malloc(len+1);

    snprintf(result, len + 1, "%s%d", s1, num);

    return result;
}
