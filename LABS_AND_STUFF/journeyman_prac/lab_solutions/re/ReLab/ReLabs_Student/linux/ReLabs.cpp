/* ReLabs.cpp for ACTP Software Reverse Engineering course */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/* Used to grab user input from the console */
char g_linebuf[1024];


/********************************************************************
* Lab A
********************************************************************/


extern "C"
__attribute__((visibility("default")))
void ReLabA()
{

}

/********************************************************************
* Lab B
********************************************************************/

extern "C"
__attribute__((visibility("default")))
void ReLabB()
{

}

/********************************************************************
* Lab C
********************************************************************/


extern "C"
__attribute__((visibility("default")))
void ReLabC()
{

}

/********************************************************************
* Lab D
********************************************************************/


extern "C"
__attribute__((visibility("default")))
void ReLabD()
{

}

/********************************************************************
* Lab E
********************************************************************/

extern "C"
__attribute__((visibility("default")))
void ReLabE()
{

}

/********************************************************************
* Lab F
********************************************************************/

extern "C"
__attribute__((visibility("default")))
void ReLabF()
{

}

/********************************************************************
* Lab G
********************************************************************/

extern "C"
__attribute__((visibility("default")))
void ReLabG()
{

}

/********************************************************************
* Lab H
********************************************************************/

extern "C"
__attribute__((visibility("default")))
void ReLabH()
{
}

/********************************************************************
* Lab I
********************************************************************/

extern "C"
__attribute__((visibility("default")))
void ReLabI()
{

}

/********************************************************************
* Lab J
********************************************************************/

extern "C"
__attribute__((visibility("default")))
void ReLabJ()
{

}


/********************************************************************
* Lab K
********************************************************************/

extern "C"
__attribute__((visibility("default")))
void ReLabK()
{

}

/********************************************************************
* Lab L
********************************************************************/

extern "C"
__attribute__((visibility("default")))
void ReLabL()
{

}

/********************************************************************
* Lab M
********************************************************************/

extern "C"
__attribute__((visibility("default")))
void ReLabM()
{

}

int main(int argc, char* argv[])
{
	typedef void (*RELAB)();

	RELAB fp[]  = {
		ReLabA,
		ReLabB,
		ReLabC,
		ReLabD,
		ReLabE,
		ReLabF,
		ReLabG,
		ReLabH,
        ReLabI,
		ReLabJ,
		ReLabK,
		ReLabL,
        ReLabM,
		NULL
	};

	char *p = getLine("Letter?");

	char cc = toupper(p[0]);
	if(cc < 'A' || cc > 'M')
	{
		printf("nope\n");
		return -1;
	}


	fp[(cc-'A')]();

	printf("DONE\n");

    return 0;
}