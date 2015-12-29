#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char *argv[])
{
  int i = 0;
  int x = 0;

  if(argc != 2)
  	exit();

  for (i = 0; i < atoi(argv[1]); i++)
  {
	x = x + 1; //printf(1, "%d \n", i);
  }

  exit();
}
