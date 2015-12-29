#include "types.h"
#include "stat.h"
#include "user.h"



int main(int argc, char *argv[])
{

	char *ptr = NULL;

 	printf(1,"STARTING TEST: null pointer deferencing in syscall.\nnull pointer is dereferenced inside a syscall.\n");
 	printf(1,"EXPECTED: null pointer deferencing shoud not cause segmentation fault in kernel.\n");
	nullsyscall(ptr);
	printf(1,"GOT: null pointer deferencing didn't cause segmentation fault in kernel.\n");
	printf(1,"TEST PASSED\n");

  	exit();
}
