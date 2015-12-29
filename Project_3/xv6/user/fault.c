#include "types.h"
#include "stat.h"
#include "user.h"

#define PGSIZE 4096


int main()
{
	//setting address that is not in stack or heap
	char *ptr = (640*1024 + PGSIZE);

 	printf(1,"STARTING TEST: fault address deferencing.\ndereferencing fault pointer: 0x%x that is not in stack or heap.\n", ptr);
 	printf(1,"EXPECTED: fault address deferencing shoud cause segmentation fault in kernel. Process should be killed.\n");
	printf(1,"ptr: %d\n address: %d", *ptr, ptr);
	printf(1,"GOT: fault address deferencing didn't cause segmentation fault in kernel. Process is not killed.\n");
	printf(1,"TEST FAILED\n");

    exit();
}
