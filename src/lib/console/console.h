#pragma once

#include <board.h>

#define MAX_COMMANDS 10
#define MAX_INPUT_LENGTH 128
#define MAX_ARGS 8

typedef void (*command_func_t)(int argc, char** argv);

typedef struct {
    const char*    name;
    const char*    help;
    command_func_t func;
} console_cmd_t;

void console_init(void);
void console_run(void);
void console_print(const char* format, ...);
void console_register_command(const char* name, const char* help, command_func_t func);
void console_process_input(const char* input_line);