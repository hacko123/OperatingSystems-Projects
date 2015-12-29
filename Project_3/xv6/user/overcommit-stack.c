#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"

void grow_stack(int j)
{
	printf(1,"j: %d address: %d\n",j, &j);
	j++;
	grow_stack(j);
	return;
}

int main(int argc, char *argv[])
{
	int i = 0;
	

	printf(1,"STARTING TEST: over commit memory on stack.\nallocating bunch of memory on stack until its full\n");
    printf(1,"EXPECTED: over commiting memory on stack should cause error.\n");
	grow_stack(i);
	printf(1,"GOT: over commiting memory on stack should didn't cause error.\n");
    printf(1,"TEST FAILED\n");

  	exit();
}