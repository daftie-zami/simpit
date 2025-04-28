#include <console.h>
#include <console_usart.h>
#include <utils.h>

#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int           command_count = 0;
static console_cmd_t console_cmd[MAX_COMMANDS];

static void console_help(int argc, char** argv);

void console_init(void) {
    command_count = 0;
    console_usart_init();
    console_register_command("help", "List available commands", console_help);
}

void console_register_command(const char* name, const char* help, command_func_t func) {
    if (command_count < MAX_COMMANDS) {
        console_cmd[command_count++] = (console_cmd_t){name, help, func};
    }
}

void console_process_input(const char* input_line) {
    char* argv[MAX_ARGS];
    char  buffer[MAX_INPUT_LENGTH];
    strncpy(buffer, input_line, MAX_INPUT_LENGTH);
    buffer[MAX_INPUT_LENGTH - 1] = '\0';

    int   argc  = 0;
    char* token = strtok(buffer, " \t\n");
    while (token != NULL && argc < MAX_ARGS) {
        argv[argc++] = token;
        token        = strtok(NULL, " \t\n");
    }

    if (argc == 0)
        return;

    for (int i = 0; i < command_count; ++i) {
        if (strcmp(argv[0], console_cmd[i].name) == 0) {
            console_cmd[i].func(argc, argv);
            return;
        }
    }

    console_print("Unknown command: %s\n", argv[0]);
}

void console_run(void) {
    static char line[128];
    if (console_usart_read_line(line, sizeof(line))) {
        console_process_input(line);
    }
}

void console_print(const char* format, ...) {
    char    buf[128];
    va_list argptr;
    va_start(argptr, format);
    vsprintf(buf, format, argptr);
    va_end(argptr);

    console_usart_write((uint8_t*) buf, strlen(buf));
}

static void console_help(int argc, char** argv) {
    UNUSED(argc);
    UNUSED(argv);
    console_print("Available commands:\n");
    for (int i = 0; i < command_count; ++i) {
        console_print("%s: %s\n", console_cmd[i].name, console_cmd[i].help);
    }
}