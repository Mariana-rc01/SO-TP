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

/**
 * @brief Concatenates a string with an integer.
 *
 * @param s1 The string to concatenate.
 * @param num The integer to concatenate.
 * @return char* The concatenated string.
 */
char* myConcat(const char* s1, int num);

/**
 * @brief Converts a string to uppercase.
 *
 * @param str The string to convert.
 */
void toUpperCase(char* str);

/**
 * @brief Extracts the program name from a string.
 *
 * @param string The input string.
 * @return char* The extracted program name.
 */
char* extractProgramU(char* string);

/**
 * @brief Extracts the program name from a string containing multiple commands.
 *
 * @param string The input string.
 * @param command_flag Flag indicating whether it's a single command or multiple commands.
 * @return char* The extracted program name.
 */
char* extractProgram(char* string, int command_flag);

/**
 * @brief Splits a string into an array of strings using a delimiter.
 *
 * @param string The input string to split.
 * @param delimiter The delimiter character.
 * @param num_commands Pointer to store the number of commands.
 * @return char** An array of strings.
 */
char** splitString(char* string, char* delimiter, int* num_commands);

/**
 * @brief Finds and returns the index of the first free manager in an array.
 *
 * @param managers An array representing the managers.
 * @param N The size of the array.
 * @return int The index of the first free manager, or -1 if none is found.
 */
int freeManager(int managers[], int N);

/**
 * @brief Calculates the elapsed time between two timeval structs in milliseconds.
 *
 * @param start The start time.
 * @param end The end time.
 * @return int The elapsed time in milliseconds.
 */
int calculateElapsedTime(struct timeval start, struct timeval end);

/**
 * @brief Prints an error message.
 *
 * @param flag Flag indicating the type of error message to print.
 */
void printMessageError(int flag);

#endif
