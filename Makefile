# START COPYRIGHT NOTICE

# 	Copyright 2023 Ana Pires, Hugo Abelheira, Mariana Rocha
#
#    Licensed under the Apache License, Version 2.0 (the "License");
#    you may not use this file except in compliance with the License.
#    You may obtain a copy of the License at
#
#       http://www.apache.org/licenses/LICENSE-2.0
#
#    Unless required by applicable law or agreed to in writing, software
#    distributed under the License is distributed on an "AS IS" BASIS,
#    WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
#    See the License for the specific language governing permissions and
#    limitations under the License.

# END COPYRIGHT NOTICE

# START CONFIGURATION

CC             := gcc
CFLAGS         := -Wall -Wextra -Werror=unused-result -Werror -pedantic -fexec-charset=UTF-8 -Iinclude -Wno-unused-variable
DEBUG_CFLAGS   := -g
RELEASE_CFLAGS := -O2

OBJDIR         := obj
DOCSDIR        := docs

define Doxyfile

	INPUT                  = include src README.md
	RECURSIVE              = YES
	EXTRACT_ALL            = YES
	FILE_PATTERNS 		   = *.h *.c

	PROJECT_NAME 		   = SO
	USE_MDFILE_AS_MAINPAGE = README.md

	OUTPUT_DIRECTORY	   = $(DOCSDIR)
	GENERATE_HTML          = YES
	GENERATE_LATEX         = YES

endef

# END CONFIGURATION

# START MAKEFILE RULES

export Doxyfile

ifeq ($(DEBUG), 1)
	CFLAGS += ${DEBUG_CFLAGS}
else
	CFLAGS += ${RELEASE_CFLAGS}
endif


all: folders server client

server: bin/orchestrator

client: bin/client

folders:
	@mkdir -p src include obj bin tmp

bin/orchestrator: obj/orchestrator/orchestrator.o obj/utils.o obj/task.o
	mkdir -p $(@D)
	$(CC) $(CFLAGS) $^ -o $@

bin/client: obj/client/client.o obj/task.o obj/client/parser.o obj/utils.o obj/task.o
	$(CC) $(CFLAGS) $^ -o $@

obj/%.o: src/%.c
	mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	find bin/ -maxdepth 1 \( -type f -not \( -name 'hello' -o -name 'void' \) \) -o \( -type p \) -delete
	rm -f bin/client
	rm -f bin/orchestrator
	rm -f obj/client/* obj/orchestrator/* obj/* tmp/*

# END MAKEFILE RULES

