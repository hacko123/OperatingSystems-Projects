#include "types.h"
#include "stat.h"
#include "user.h"



void D()
{
    char name[1024] = "funtion D()";
    int i = 0;
    char j[20];
    uint k = 97;
    
    for(i = 0; i < 20; i++)
    {
        j[i] = k++;
    }

    printf(1,"name: \"%s\" address: %d\n",name, &name);

    printf(1,"i: %d address: %d\n",i, &i);
    for(i = 19; i >= 0; i--)
    {
        printf(1,"j[%d]: %c address: %d\n",i, j[i], &j[i]);
    }
    

    return;
}


void C()
{
    char name[1024] = "funtion C()";
    int i = 0;
    char j[20];
    uint k = 97;
    
    for(i = 0; i < 20; i++)
    {
        j[i] = k++;
    }

    printf(1,"name: \"%s\" address: %d\n",name, &name);

    printf(1,"i: %d address: %d\n",i, &i);
    for(i = 19; i >= 0; i--)
    {
        printf(1,"j[%d]: %c address: %d\n",i, j[i], &j[i]);
    }
    

    D();

    return;
}


void B()
{
    char name[1024] = "funtion B()";
    int i = 0;
    char j[20];
    uint k = 97;
    
    for(i = 0; i < 20; i++)
    {
        j[i] = k++;
    }

    printf(1,"name: \"%s\" address: %d\n",name, &name);

    printf(1,"i: %d address: %d\n",i, &i);
    for(i = 19; i >= 0; i--)
    {
        printf(1,"j[%d]: %c address: %d\n",i, j[i], &j[i]);
    }
    

    C();

    return;
}



void A()
{
    char name[1024] = "funtion A()";
    int i = 0;
    char j[20];
    uint k = 97;
    

    for(i = 0; i < 20; i++)
    {
        j[i] = k++;
    }

    printf(1,"name: \"%s\" address: %d\n",name, &name);

    printf(1,"i: %d address: %d\n",i, &i);
    for(i = 19; i >= 0; i--)
    {
        printf(1,"j[%d]: %c address: %d\n",i, j[i], &j[i]);
    }
    

    B();

    return;
}

int main(int argc, char **argv)
{

    printf(1,"STARTING TEST: stack-growth.\nallocating 1 page less amount of varibles in a each function but in total more than 1 page in total (stack) and check the addresses). \n");
    printf(1,"EXPECTED: varibles are allocated on stack and addresses should look reasonable.\n");
    A();
    printf(1,"EXPECTED: varibles are allocated on stack and addresses look reasonable.\n");
    printf(1,"TEST PASSED\n");

    exit();
}
