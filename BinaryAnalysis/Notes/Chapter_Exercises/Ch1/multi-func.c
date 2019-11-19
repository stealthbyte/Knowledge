#include <stdio.h>


void addStuff()
{
    int a, b;
    a = 5;
    b = 10;
    
    printf("New Value %d\n", b*a);
}

int simpleCalc()
{
    int newValue;
    int dolla = 5;
    int bill = 20;

    newValue = dolla + bill;
    return newValue;
}

int main()
{
    int myMoney;
    addStuff();
    myMoney = simpleCalc();
    printf("All my money %d\n", myMoney);
}