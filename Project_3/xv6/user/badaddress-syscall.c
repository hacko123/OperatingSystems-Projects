#include "types.h"
#include "stat.h"
#include "user.h"

#define PGSIZE 4096


int main()
{
	//setting address between stack and heap
	char *ptr = (PGSIZE * 25);

 	printf(1,"STARTING TEST: bad address deferencing.\ndereferencing bad pointer: 0x%x.\n", ptr);
 	printf(1,"EXPECTED: bad address deferencing shoud not cause segmentation fault in kernel.\n");
	nullsyscall(ptr);
	printf(1,"GOT: bad address deferencing didn't cause segmentation fault in kernel.\n");
	printf(1,"TEST PASSED\n");

    exit();
}
