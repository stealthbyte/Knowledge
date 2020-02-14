/********************************************************************
* Lab C
********************************************************************/
int my_max(int a, int b, int *c=0)
{
	int max = a;
	if( b > a )
    {
		max = b;
    }
	if( c )
    {
		*c = max;
    }
	return max;
}

extern "C"
__attribute__((visibility("default")))
void ReLabC()
{
	int res;
	res = my_max(promptForNumber(), promptForNumber());
	printf("%d\n", res);

	my_max(promptForNumber(), promptForNumber(), &res);
	printf("%d\n", res);
}

