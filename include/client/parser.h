/**
 * @file parser.h
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

#ifndef PARSER_H
#define PARSER_H

#include "task.h"

/**
 * @brief Processes the input arguments and creates a task accordingly.
 *
 * @param argc The number of command-line arguments.
 * @param data An array of strings containing the input data.
 * @return Task The task created based on the input data.
 */
Task processInput(int argc, char* data[]);


#endif
