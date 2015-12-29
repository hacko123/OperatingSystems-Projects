#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>
#include <assert.h>
#include <sys/types.h>
#include <dirent.h>

struct History
{
	int num;
	char hist_cmd[256];
}hist[20];

//globals
int Return_Val, i = 0, j = 0, k = 0, m = 0,n=0;
int h=0,p=0, Count, New_Count, Temp,Space_Count=0;
int Hist_flag=0;
char *Argv[10], *Argv2[10], *Return_Val_2;
char Input_Command[256];
char *Temp_Char, *Temp_Char_1, *Temp_Char_2, *Temp_Char_3, *Save_Ptr, *Temp_Char_4;
char Error_Message[30] = "An error has occurred\n";
char *Output_File = "/no/such/file";
char *Batch_File = "/no/such/file";
unsigned int Batch_Mode = 0;
int Batch_File_Desp; 
DIR *Dir;
unsigned int Command_Error = 0; 
unsigned int Redirection = 0;


void Error_Command()
{
	write(STDERR_FILENO, Error_Message, strlen(Error_Message));
	Command_Error = 1; 
	//exit(0);
}
void Check_Redirection_Error()
{
	Space_Count = 0;
	//printf("Inside Check_Redirection_Error %s\n", Temp_Char_4);
	if(Temp_Char_4 != NULL)
	{
		Temp_Char_4 = strpbrk(Temp_Char_4,">");
		if(Temp_Char_4 != NULL)
		{
			//printf("%s\n",Temp_Char_4);
			for (n = 0; n < strlen(Temp_Char_4); n++) 
			{
		    	if(Temp_Char_4[n] == ' ')
		    	{
		    		if(Temp_Char_4[n - 1] == '>')
		    			continue;
		    		else
		    			Space_Count++;
		    	}
			}

			if(Space_Count > 0)
			{
				//printf("Spce count %d\n", Space_Count);
				Error_Command();
			}
		}
	}

	Temp_Char_4 = NULL;
		
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

char *Str_Replace(char *orig, char *rep, char *with) 
{
    char *result; // the return string
    char *ins;    // the next insert point
    char *tmp;    // varies
    int len_rep;  // length of rep
    int len_with; // length of with
    int len_front; // distance between rep and end of last rep
    int count;    // number of replacements

    if (!orig)
        return NULL;
    if (!rep)
        rep = "";
    len_rep = strlen(rep);
    if (!with)
        with = "";
    len_with = strlen(with);

    ins = orig;
    for (count = 0; tmp = strstr(ins, rep); ++count) {
        ins = tmp + len_rep;
    }

    // first time through the loop, all the variable are set correctly
    // from here on,
    //    tmp points to the end of the result string
    //    ins points to the next occurrence of rep in orig
    //    orig points to the remainder of orig after "end of rep"
    tmp = result = malloc(strlen(orig) + (len_with - len_rep) * count + 1);

    if (!result)
        return NULL;

    while (count--) {
        ins = strstr(orig, rep);
        len_front = ins - orig;
        tmp = strncpy(tmp, orig, len_front) + len_front;
        tmp = strcpy(tmp, with) + len_with;
        orig += len_front + len_rep; // move to next "end of rep"
    }
    strcpy(tmp, orig);
    return result;
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

// void Print_Last_History()
// {
// 	if(h<20) {
// 			printf("%d %s",hist[h].num,hist[h].hist_cmd);
// 	}
// 	else {
// 			printf("%d %s",hist[19].num,hist[19].hist_cmd);
// 	}
// }

void Print_History()
{
	if(h<=20) {
		for(i=0;i<h;i++)
			printf("%d %s",hist[i].num,hist[i].hist_cmd);
	}
	else {
		for(i=0;i<20;i++)
			printf("%d %s",hist[i].num,hist[i].hist_cmd);
	}

}


void Parse_Arguments()
{
	Redirection = 0;
	Temp_Char_4 = strdup(Input_Command);
	Check_Redirection_Error();
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

unsigned int Check_History()
{
	int u=0,value=0;
	char counter[10];
	//char Old_Input_Command[256];
	char *Old_Input_Command, *New_Input_Command, *Temp_Char_Local, *To_Replacement_Str;
	//printf("inside check history %s strlen of input command = %d \n",Input_Command, (int) strlen(Input_Command));


	 for(i=0;i<strlen(Input_Command);i++)
	 {
    	if(Input_Command[i]=='!')
    	{

    		Temp_Char_Local = strstr (Input_Command,">");
    		if(Temp_Char_Local != NULL)
    		{
    			Old_Input_Command = strdup(Input_Command);
    			strtok(Old_Input_Command,"\n");
    			strtok(Input_Command, ">");
    			To_Replacement_Str = strdup(Input_Command);
				strcat(Input_Command,"\n");
				//printf("Old_Input_Command = %s To_Replacement_Str = %s Input_Command = %s\n",Old_Input_Command, To_Replacement_Str, Input_Command);
    		}

    		
			
    		
    		if((Input_Command[i+1]==' ') && (Input_Command[i+2]>='0' && Input_Command[i+2]<='9'))
  			{
  				if(Input_Command[i+3] ==' ' && Input_Command[i+4]!='\n')
	    		{
	    			//printf("here 1\n");
	    			write(STDERR_FILENO, Error_Message, strlen(Error_Message));
    			 	return 0;
    			}
    			if(Input_Command[strlen(Input_Command)-2]==' ')
    			{
		    		for(j=i+2;j<strlen(Input_Command)-2;j++)
		    			counter[u++]=Input_Command[j];
		    		counter[u]='\0';
		    		if(counter[0]!='\0')
		    			value=atoi(counter);
		    		break;
	    		}
	    		else if(Input_Command[strlen(Input_Command)-2]!=' ')
	    		{
		    		for(j=i+2;j<strlen(Input_Command)-1;j++)
		    			counter[u++]=Input_Command[j];
		    		counter[u]='\0';
		    		if(counter[0]!='\0')
		    			value=atoi(counter);
		    		break;
	    		
	    		}
	    	}
	    	else if(Input_Command[i+1]>='0' && Input_Command[i+1]<='9')
	    	{
	    		//printf("here 1.1\n");
	    		if(Input_Command[i+2]==' ' && Input_Command[i+3]!='\n')
	    		{
	    			printf("here 2\n");
	    			write(STDERR_FILENO, Error_Message, strlen(Error_Message));
    			 	return 0;
    			}
	    		for(j=i+1;j<strlen(Input_Command)-1;j++)
	    		{

	    			counter[u++]=Input_Command[j];
	    		}
	    		counter[u]='\0';
	    		//printf("counter = %s\n", counter);
	    		if(counter[0]!='\0')
	    			value=atoi(counter);
	    		break;
	    	}
	    	// if((Input_Command[i+1]==' ') && (Input_Command[i+2]>='0' && Input_Command[i+2]<='9') && (Input_Command[i+3]==' ') && ((Input_Command[i+4]!=' ') || (Input_Command[i+1]=='\0')))
    		// 	write(STDERR_FILENO, Error_Message, strlen(Error_Message));
    	}
	}
	//printf("value = %d\n",value);
    if(value!=0)
    {
    	for(i=0;i<19;i++)
    	{
    		if(value==hist[i].num)
    		{


    			strcpy(Input_Command,hist[i].hist_cmd);
    			//printf("Input_Command = %s\n", Input_Command);
    			
    			if(Temp_Char_Local != NULL)
    			{
    				strtok(Input_Command,"\n");
    				New_Input_Command = Str_Replace(Old_Input_Command, To_Replacement_Str, Input_Command);
    				//printf("New_Input_Command = %s\n", New_Input_Command);
    				strcpy(Input_Command, New_Input_Command);
    				//printf("Input_Command = %s\n", Input_Command);	
    			}
    			break;
    		}
    	}
    }

    // printf("Input_Command = %s\n", Input_Command);
    // printf("exiting check history\n");
    return 1;
}

void Add_History()
{
	//printf("inside history\n");

	if((strcmp(Input_Command,"!\n") != 0) && (strcmp(Input_Command," !\n") != 0) && (strcmp(Input_Command," ! \n") != 0) && (strcmp(Input_Command,"! \n") !=0) ) 
	   {
	    	h++;
	    	if(h<=20)
	    	{
	    		hist[h-1].num=h;
	    		strcpy(hist[h-1].hist_cmd,Input_Command);
	    	}
		    else if(h>20)
		    {
		    	for(i=1;i<20;i++)
		    	{
		    		strcpy(hist[i-1].hist_cmd,hist[i].hist_cmd);
		    		hist[i-1].num=hist[i].num;
		    	}
		    	hist[19].num = h;
		    	strcpy(hist[19].hist_cmd,Input_Command);
		    }
		}
	    else
		{
				if(h<20 && h>0)
				{
					strcpy(Input_Command,hist[h-1].hist_cmd);
					strcpy(hist[h].hist_cmd,hist[h-1].hist_cmd);
					hist[h].num=h+1;
					h++;
				}
				else if (h>19)
				{
					strcpy(Input_Command,hist[19].hist_cmd);
					strcpy(hist[19].hist_cmd,hist[19].hist_cmd);
					hist[19].num=h;
				}
				else {
					write(STDERR_FILENO, Error_Message, strlen(Error_Message));
					//return 0; 
				}
		}
}

void Print_Args()
{
	printf("After split\n");
	for(i=0;i<Count + 1;i++)
		printf("Argv[%d] is %s\n", i, Argv[i]);

	if(Redirection == 1)
		printf("Output file is %s\n",Output_File);
}

int My_Shell()
{

	Command_Error = 0;
	Redirection = 0;
	//printf("mysh # ");
	memset(&Input_Command, 0, sizeof(Input_Command));
	if(Batch_Mode == 0)
	{
		printf("mysh # ");
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

    if(strcmp(Input_Command,"\n")==0)
    {
    	return 0;
    }
    //strtok(Input_Command, "\n");
    if(Check_History() == 0)
    	return 0;
    
    Add_History();

    strtok(Input_Command, "\n");

    if(Batch_Mode == 1)
    {
    	printf("%s\n",Input_Command);	
    }

    Parse_Arguments(Input_Command,Argv);

    //Print_Args();

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

	if(Command_Error == 1)
		return 0;
	
	Return_Val = fork();

	//Check for return value
	if(Return_Val == 0)
	{
		//open(STDOUT_FILENO);
		if((strcmp(Argv[0],"history")==0  || strcmp(Input_Command," history")==0 || strcmp(Input_Command," history ")== 0) && Argv[1]==NULL)
		{
			if(Redirection == 1)
			{
				close(STDOUT_FILENO);
				open(Output_File, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
			}
			Print_History();
			exit(0);	
		}	
		if(Redirection == 1)
		{
			close(STDOUT_FILENO);
			open(Output_File, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
		}

		execvp(Argv[0],Argv);
		Error_Command();
		exit(0);
	}

	else if(Return_Val > 0)
	{
		if(strcmp(Input_Command, "exit") == 0 )//|| Command_Error == 1)
		{
			printf("exit works\n");
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
