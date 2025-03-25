#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdarg.h>

void console_init(void){

}


/**
 * Print a text on the console
 */
void console_print(const char *format, ...) {
    char buf[128];
    va_list argptr;
    va_start(argptr, format);
    vsprintf(buf, format, argptr);
    va_end(argptr);
  
    // console_write(&cdcacm_console_tx, (uint8_t*)buf, strlen(buf));
}

// static void console_write(){

// }