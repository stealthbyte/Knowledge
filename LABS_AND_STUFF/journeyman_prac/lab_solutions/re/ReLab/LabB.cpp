/* ReLabs.cpp for ACTP Software Reverse Engineering course */

#include <stdio.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>
#include <stdint.h>
#include <sys/mman.h>

/* Used to grab user input from the console */
char g_linebuf[1024];

#define _fastcall __attribute__((fastcall))
#define _stdcall __attribute__((stdcall))
#define __forceinline __attribute__((always_inline))
#define INFINITE 0xffffffff
#define MAX_PATH 260
int trimNewline(char *sz);
char *getLine(const char *prompt="line?")
{
	printf("%s> ", prompt);
    fflush(NULL);
/********************************************************************
* Lab B
********************************************************************/
//fastcall
//uses lea to do subtracts
//an inefficient fibonacci calculator
unsigned long _fastcall slowfib(unsigned long a)
{
	if( a <= 1 )
    {
		return a;
    }
	return slowfib(a-1) + slowfib(a-2);
}

extern "C"
__attribute__((visibility("default")))
void ReLabB()
{
	unsigned long a = (unsigned long)promptForNumber();
	if(a == 0)
    {
		printf("no\n");
    }
	else if( a > 30 )
    {
		printf("Too big\n");
    }
	else
    {
		printf("%ld %ld\n", a, slowfib(a));
    }
}
