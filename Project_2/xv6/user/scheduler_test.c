#include "types.h"
#include "stat.h"
#include "user.h"
#include "fs.h"
#include "fcntl.h"
#include "syscall.h"
#include "traps.h"

int stdout = 1;

int main(int argc, char *argv[])
{
  int i = 0;
  int x = 0;
  int fd;

  if(argc != 2)
  	exit();

  fd = open("iotest.txt", O_CREATE|O_RDWR);
  if(fd >= 0)
  {
    printf(stdout, "create iotest succeeded; ok\n");
  } 
  else 
  {
    printf(stdout, "error: iotest create failed!\n");
    exit();
  }

  for (i = 0; i < atoi(argv[1]); i++)
  {
	x = x + 1; //printf(1, "%d \n", i);
	if( x%100 == 0 )
	{
		if(write(fd, "iotest", 6) != 6);
		{
      		printf(stdout, "error: write failed\n",i);
      		close(fd);
      		exit();
	    }
	}

  }

  close(fd);
  exit();
}
