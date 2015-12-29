#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <dirent.h>

//globals
int Return_Val, i = 0, j = 0, k = 0, m = 0, Count, New_Count, Temp;
char *Argv[10], *Argv2[10], *Return_Val_2;
char Input_Command[256];
char *Temp_Char, *Temp_Char_1, *Temp_Char_2, *Temp_Char_3, *Save_Ptr;
char Error_Message[30] = "An error has occurred\n";
char *Output_File = "/no/such/file";
char *Batch_File = "/no/such/file";
unsigned int Command_Error = 0; 
unsigned int Redirection = 0;
unsigned int Batch_Mode = 0;
int Batch_File_Desp; 
DIR *Dir;


void Error_Command()
{
	write(STDERR_FILENO, Error_Message, strlen(Error_Message));
	Command_Error = 1; 
	exit(0);
}

void Check_Batch_File()
{
	Batch_File_Desp = open(Batch_File, O_RDONLY);
	if (Batch_File_Desp < 0) 
	{
		fprintf(stderr,"Error: Cannot open file %s\n", Batch_File);
		Error_Command();
	}
}
unsigned int Is_Redirection()
{
	//printf("Inside Redirection\n");
	for(i = 0; i < Count; i++)
	{
		if(strcmp(Argv[i],">") == 0)
		{
			Output_File = strdup(Argv[i+1]);
			Argv[i] = NULL; 
			New_Count = i;
			return 1;
		}

	}

	return 0;
  	
}


unsigned int Split_Args_Redirection()
{
	j = 0;
	for(i = 0; i < Count; i++)
	{
		if(strcmp(Argv[i],">") == 0)
		{
			Output_File = strdup(Argv[i+1]);
			Argv[i] = NULL; 
			New_Count = i;
			return 1;
		}

		else
		{
			j = i;
			Temp_Char_1 = strstr (Argv[i],">");
			if(Temp_Char_1 != NULL)
			{
				printf("redirection operator is found\n");
				Temp_Char = strtok_r(Argv[i], ">", &Save_Ptr);
				while (Temp_Char != NULL) 
				{
					printf ("Temp_Char is %s Save_Ptr = %s\n",Temp_Char, Save_Ptr);
				    Temp_Char = strtok_r(NULL, " > ", &Save_Ptr);
				}
			}

		}

	}

	return 0;
}

void Parse_Arguments()
{
	Redirection = 0;
	//printf ("Splitting string "%s" into words:\n",Input_Command);
	Temp_Char = strtok_r(Input_Command, " ", &Save_Ptr);
	i = 0;
	Temp_Char_1 = strstr (Temp_Char,">");
	//printf ("Temp_Char is %s Temp_Char_1 is %s Save_Ptr = %s\n",Temp_Char, Temp_Char_1, Save_Ptr);
	if((Temp_Char_1 != NULL) && (strcmp(Temp_Char_1,">") == 0))
	{
		//printf("redirection operator is found\n");
		Redirection = 1;
		//printf("Save_Ptr is %s\n", Save_Ptr);
		Temp_Char_3 = strdup(Save_Ptr);
		Temp_Char_2 = strtok_r(Input_Command, ">", &Save_Ptr);
		//printf("Save_Ptr is %s\n", Save_Ptr);
		Argv[i] = strdup(Temp_Char_2);
		Argv[i+1] = '\0';
		Output_File = strdup(Temp_Char_3);
		Count = 2; 
		return;

	}
	if(Temp_Char_1 != NULL)
	{
		//printf("redirection operator is found\n");
		Redirection = 1;
		//printf("Save_Ptr is %s\n", Save_Ptr);
		Temp_Char_2 = strtok_r(Input_Command, ">", &Save_Ptr);
		//printf("Save_Ptr is %s\n", Save_Ptr);
		Argv[i] = strdup(Temp_Char_2);
		Argv[i+1] = '\0';
		Output_File = strdup(Save_Ptr);
		Count = 2; 
		return;
	}
	while (Temp_Char != NULL) 
	{
		Argv[i] = strdup(Temp_Char);
		//printf ("Argv[%d] is %s Save_Ptr = %s\n",i, Argv[i], Save_Ptr);
	    Temp_Char_1 = strstr (Save_Ptr,">");
		if(Temp_Char_1 != NULL)
		{
			//printf("redirection operator is found\n");
			Redirection = 1;
		}
		Temp_Char = strtok_r(NULL, " > ", &Save_Ptr);
	    i++;
	}

	if(Redirection == 1)
	{
		Output_File = strdup(Argv[i-1]);
		Argv[i-1] = '\0';
	}

	Argv[i] = '\0';
	Count = i;
}

int My_Shell()
{
	Command_Error = 0;
	Redirection = 0;
	memset(&Input_Command, 0, sizeof(Input_Command));

	if(Batch_Mode == 0)
	{
		printf("mysh > ");
	}

	else if(Batch_Mode == 1)
	{
		Return_Val = dup2(Batch_File_Desp,0);
		if(Return_Val == -1)
		{
			Error_Command();
			//printf("dup failed\n");
			exit(0);
		}
	}
    
    Return_Val_2 = fgets(Input_Command, 256, stdin);
    if(Return_Val_2 == NULL)
    {
    	//printf("End of batch file \n");
    	exit(0);
    }
    strtok(Input_Command, "\n");

    if(Batch_Mode == 1)
    {
    	printf("%s\n",Input_Command);	
    }
    
	Parse_Arguments(Input_Command,Argv);
	
	// printf("After split\n");
	// for(i=0;i<Count + 1;i++)
	// 	printf("Argv[%d] is %s\n", i, Argv[i]);

	// if(Redirection == 1)
	// 	printf("Output file is %s\n",Output_File);

	if((strcmp(Input_Command, "\n") == 0))
		return 0;
	
	if((strcmp(Argv[0],"cd") == 0))
	{
		Dir = opendir (Argv[1]);
        if (Dir == NULL) 
        {
            Error_Command();
        }
        else
        {
			chdir(Argv[1]);
		}
		return 0;
	}

    if((strcmp(Input_Command, "exit") == 0) || (strcmp(Argv[0], "exit") == 0))
	{
		exit(0);
	}
	
	Return_Val = fork();

	//Check for return value
	if(Return_Val == 0)
	{
		//open(STDOUT_FILENO);
		if(Redirection == 1)
		{
			close(STDOUT_FILENO);
			open(Output_File, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
		}

		execvp(Argv[0],Argv);
		Error_Command();
	}

	else if(Return_Val > 0)
	{
		if(strcmp(Input_Command, "exit") == 0 || Command_Error == 1)
		{
			//printf("exit works\n");
			exit(0);
		}
	}

	else 
	{
		//printf("fork failed\n");
		Error_Command();
	}

	return 0;

}


int main(int argc, char *argv[])
{
	if(argc > 2)
	{
		Error_Command();
	}

	if(argc == 2)
	{
		Batch_File = strdup(argv[1]);
		Check_Batch_File();
		Batch_Mode = 1;

	}

	while(1)
	{
		My_Shell();
		(void) wait(NULL);
	}
	
	return 0;
	
}
