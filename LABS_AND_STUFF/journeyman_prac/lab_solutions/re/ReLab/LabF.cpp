/********************************************************************
* Lab F
********************************************************************/
int my_strlen(const char *sz)
{
	int i;
	for(i=0; sz[i]; i++);
	return i;
}

//returns length
int trimNewline(char *sz)
{
	//trim any newline, return length of string
	int cb;

	if( !sz)
	{
		printf("Invalid parameter!\n");
		return -1;
	}

	cb = my_strlen(sz);

	if( cb>1 && sz[cb-1] == '\n' )
    {
		sz[--cb] = '\0';
    }

	return cb;
}

extern "C"
__attribute__((visibility("default")))
void ReLabF()
{

    volatile unsigned long secretPhrase[] = {0x60562f10, 0xf98ba986, 0x6ac8e0ad, 0xacc8a9ad, 0x00000000};

    secretPhrase[0] ^= 0x12345678;
    secretPhrase[1] ^= 0x90ABCDEF;
    secretPhrase[2] ^= 0x0BADC0DE;
    secretPhrase[3] ^= 0xDEADC0DE;

	do
	{
		if(!fgets(g_linebuf, sizeof(g_linebuf), stdin))
        {
			return;
        }
		trimNewline(g_linebuf);

		if(0 == strcasecmp(g_linebuf, (char *)&secretPhrase))
        {
			return;
        }

	}while(1);
}
