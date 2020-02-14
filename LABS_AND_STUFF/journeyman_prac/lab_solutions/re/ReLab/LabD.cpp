/********************************************************************
* Lab D
********************************************************************/
/*
table switch
*/
extern "C"
__attribute__((visibility("default")))
const char *getColor(int a)
{
	//table switch
	switch(a)
	{

	case 0: return "black";
	case 1: return "white";
	case 2: return "red";
	case 3: return "blue";
	case 4: return "green";
	case 5: return "orange";
	case 6: return "purple";
	case 7: return "cyan";
	case 8: return "pink";
	default:
		return NULL;
	}
}

extern "C"
__attribute__((visibility("default")))
void ReLabD()
{
	printf("?? to exit\n");
	do {
		int a = promptForNumber();
		if( a==99 )
        {
			return;
        }
		const char *p = getColor( a  );
		if( !p )
        {
			printf("no\n");
        }
		else
        {
            printf("%d %s\n", a, p);
        }
	}while(1);
}
