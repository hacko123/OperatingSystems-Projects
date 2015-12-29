#include "types.h"
#include "stat.h"
#include "user.h"

void small_stack()
{
	char name[256] = "small-stack()";
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

int main(int argc, char **argv)
{

 	printf(1,"STARTING TEST: small-stack.\nallocating 1 page less amount of varibles in a function (stack) and check the addresses.\n");
 	printf(1,"EXPECTED: varibles are allocated on stack and addresses should look reasonable.\n");
	small_stack();
	printf(1,"EXPECTED: varibles are allocated on stack and addresses look reasonable.\n");
	printf(1,"TEST PASSED\n");

    exit();
}
