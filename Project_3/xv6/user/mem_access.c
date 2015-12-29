#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
 	char *ptr = (char *) ((640*1024) - 4096);
 	uint rc; 

	printf(1,"address of USERTOP: %x\n", ptr);

	*ptr = 'a';

	rc = fork();

	if(rc == 0)
	{
		printf(1,"from  child: %c\n", *ptr);		
	}

	else if(rc > 0)
	{
		(void) wait();
		printf(1,"from  parent : %c\n", *ptr);		
	}

	else
	{
		printf(1,"fork failed \n");		
	}



  	exit();
}
