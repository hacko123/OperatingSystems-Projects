#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
 	int *ptr = NULL;

 	printf(1,"STARTING TEST: null pointer deferencing.\nderefencing null pointer\n");
 	printf(1,"EXPECTED: null pointer deferencing shoud cause segmentation fault. Process should be killed.\n");
	printf(1,"0x%x\n", *ptr);
	printf(1,"GOT: null pointer deferencing worked.\n");
	printf(1,"TEST FAILED\n");

  	exit();
}
