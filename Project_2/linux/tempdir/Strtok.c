#include <stdio.h>
#include <string.h>

int main ()
{
	char *str;
  char *str1 = "nothing";
	//char *saveptr;
	//char *foo, *bar;
  //char key[] = { ' ', '\n', '\t', 0 };
  char key[] = { '>' };
  char key1[] = { ' ' };
 	// char str[] ="-ls>out.txt";
  // 	char * pch;
  // 	printf ("Splitting string \"%s\" into tokens:\n",str);
  // 	pch = strtok (str,"> ");

  // 	while (pch != NULL) {
  //   	printf ("%s\n",pch);
  //   	pch = strtok (NULL, "> ");
  // 	}




	// foo = strtok_r(str, " > ", &saveptr);
	// printf("saveptr = %s \n",saveptr);
	// bar = strtok_r(NULL, " > ", &saveptr);

	// printf("foo is %s bar is %s saveptr = %s \n", foo, bar, saveptr);


    
    //printf("%s\n", strpbrk (" -l > out.txt out1.txt", key));
    str = strpbrk (" -l > out.txt out1.txt", key);
    printf("%s\n", str);

    str1 = strpbrk ("> out.txt out1.txt", key1);
    printf("%s\n", str1);
  

  	return 0;
}