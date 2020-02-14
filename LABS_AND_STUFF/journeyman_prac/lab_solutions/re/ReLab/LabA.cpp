/********************************************************************
* Lab A
********************************************************************/

int aplus2b(int a, int b, int *c)
{
	return *c = a+2*b;
}

int promptForNumber(const char *prompt="number")
{
	printf("%s> ", prompt); fflush(NULL);
	if(!fgets(g_linebuf, sizeof(g_linebuf), stdin))
		exit(0);
	return atoi(g_linebuf);
}

extern "C"
__attribute__((visibility("default"))) // This is like __declspec(dllexport)
void ReLabA()
{
	srand((unsigned int)time(NULL));
	int c;
	aplus2b(promptForNumber(), rand()&0xFF, &c);
	printf("%d\n", c);
}

