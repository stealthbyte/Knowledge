// The following ifdef block is the standard way of creating macros which make exporting 
// from a DLL simpler. All files within this DLL are compiled with the SNEAKYCRYPTO_EXPORTS
// symbol defined on the command line. This symbol should not be defined on any project
// that uses this DLL. This way any other project whose source files include this file see 
// SNEAKYCRYPTO_API functions as being imported from a DLL, whereas this DLL sees symbols
// defined with this macro as being exported.
#ifdef SNEAKYCRYPTO_EXPORTS
#define SNEAKYCRYPTO_API  __declspec(dllexport) 
#else
#define SNEAKYCRYPTO_API  __declspec(dllimport) 
#endif


extern "C"
{
typedef struct
{
	DWORD bufferSize;
	char *input;
	char *output;
}
SneakyCryptoStruct, *PSneakyCryptoStruct;

SNEAKYCRYPTO_API int __stdcall encrypt1(char *input, char * output, DWORD bufferSize);
SNEAKYCRYPTO_API int __stdcall encrypt2(PSneakyCryptoStruct myStruct);
}