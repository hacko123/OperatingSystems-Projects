#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"

#define PGSIZE 4096


int main(int argc, char *argv[])
{
	//int *mem[200];
	int i = 1;
	

	printf(1,"STARTING TEST: over commit memory on heap.\nallocating bunch of memory on heap until its full\n");
  printf(1,"EXPECTED: over commiting memory on heap should cause error.\n");


	for(;;)
	{
		if(malloc(PGSIZE) == NULL)
		{
				printf(1,"GOT: over commiting memory on heap caused error.\n");
        printf(1,"TEST PASSED\n");
				exit();
		}
		
    printf(1,"page: %d address: %d\n",i, malloc(PGSIZE));
    i = i+2;

	}

  printf(1,"GOT: over commiting memory on heap should didn't cause error.\n");
  printf(1,"TEST FAILED\n");

  exit();
  
}