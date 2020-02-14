/********************************************************************
* Lab E
********************************************************************/
/*
Branching switch

*/
extern "C"
__attribute__((visibility("default")))
const char *getBgColor(int a)
{
	//branching switch
	switch(a)
	{

	case 0:	   return "fg_black";
	case 1000: return "fg_white";
	case 2000: return "fg_red";
	case 3000: return "fg_blue";
	case 4000: return "fg_green";
	case 5000: return "fg_orange";
	case 6000: return "fg_purple";
	case 7000: return "fg_cyan";
	case 8000: return "fg_pink";
	default:
		return NULL;
	}
}

extern "C"
__attribute__((visibility("default")))
void ReLabE()
{
	printf("?? to exit\n");
	do {
		int a = promptForNumber();
		if( a==99 )
        {
			return;
        }
		const char *p = getBgColor( a  );
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
