
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
int Return_Val, i = 0, j = 0, k = 0, m = 0,n = 0, o = 0, q = 0, r = 0;
int h=0,p=0, Count, New_Count, Temp,Space_Count=0;
int Hist_flag=0;
char *Argv[100], *Argv2[100], *Return_Val_2;
char Input_Command[512];
char *Input_Command_Ptr,mysh[]="mysh # ";
char *Temp_Char, *Temp_Char_1, *Temp_Char_2, *Temp_Char_3, *Save_Ptr, *Temp_Char_4, *Temp_Char_5, *Temp_Char_6;
char Error_Message[30] = "An error has occurred\n";
char *Output_File = "/no/such/file";
char *Batch_File = "/no/such/file";
unsigned int Batch_Mode = 0;
int Batch_File_Desp, Redirection_File_Desp; 
DIR *Dir;
unsigned int Command_Error = 0; 
unsigned int Redirection = 0;
unsigned int Command_Executed = 0;
size_t len = 0;
unsigned int Spaces = 0;


void Error_Command()
{
	write(STDERR_FILENO, Error_Message, strlen(Error_Message));
	//printf("%s",Error_Message);
	Command_Error = 1; 
	//exit(0);
}
unsigned int Check_Redirection_Error()
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
		    		if(Temp_Char_4[n - 1] == '>' || Temp_Char_4[n - 1] == ' ')
		    			continue;
		    		else
		    			Space_Count++;
		    	}
			}

			if(Space_Count > 0)
			{
				//printf("Space count %d\n", Space_Count);
				
				return 1;
			}
		}
	}

	Temp_Char_4 = NULL;

	return 0;
		
}
void Check_Batch_File()
{
	struct stat st;

	Batch_File_Desp = open(Batch_File, O_RDONLY);
	if (Batch_File_Desp < 0) 
	{
		Error_Command();
		exit(1);
	}

	//Get the file size 
    if (stat(Batch_File, &st) != 0) 
    {
        //Error_Command();
        exit(0);
    }

    //Check if it is empty and print error and exit or execut a empty command and exit
    if(st.st_size == 0)
    {
    	//Error_Command();
        exit(0);
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


void Print_History()
{
	if(h<=20) {
		for(i=0;i<h;i++)
			printf("%d %s\n",hist[i].num,hist[i].hist_cmd);
	}
	else {
		for(i=0;i<20;i++)
			printf("%d %s\n",hist[i].num,hist[i].hist_cmd);
	}

}


void Parse_Arguments()
{
	Redirection = 0;
	Temp_Char_4 = strdup(Input_Command);
	if(Check_Redirection_Error() == 1)
	{
		Error_Command();
		return;
	}
	//printf ("Splitting string "%s" into words:\n",Input_Command);
	Temp_Char = strtok_r(Input_Command, " ", &Save_Ptr);
	i = 0;
	Temp_Char_1 = strstr (Temp_Char,">");
	//printf ("Temp_Char is %s Temp_Char_1 is %s Save_Ptr = %s strlen(Save_Ptr) = %d\n",Temp_Char, Temp_Char_1, Save_Ptr, (int)strlen(Save_Ptr));
	if(Temp_Char[0] == '>')
	{
		Error_Command();
		return; 
		//printf("Temp_Char is %s Save_Ptr = %s\n",Temp_Char, Save_Ptr);
		if((int)strlen(Save_Ptr) != 0)
		{
			Argv[i] = '\0';
			Save_Ptr = strtok(Save_Ptr," ");
			Output_File = strdup(Save_Ptr);
			Redirection = 1;
			Count = 1;
			int fp = open(Save_Ptr, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
			close(fp);
			Command_Executed = 1; 
			return;
		}
		else
		{
			Argv[i] = '\0';
			Temp_Char_5 = strtok(Temp_Char_1,">");
			//printf("Temp_Char_5 = %s\n", Temp_Char_5);
			Argv[i] = '\0';
			Output_File = strdup(Temp_Char_5);
			Redirection = 1;
			Count = 1;
			int fp = open(Temp_Char_5, O_WRONLY | O_CREAT | O_TRUNC, S_IRWXU);
			close(fp);
			Command_Executed = 1; 
			return;	
		}
		
	}
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
		//printf ("Argv[%d] is %s Save_Ptr = %s\n Temp_Char_1 is %s",i, Argv[i], Save_Ptr, Temp_Char_1);
		//printf ("Argv[%d] is %s Save_Ptr = %s\n",i, Argv[i], Save_Ptr);
	    i++;
	}

	if(Redirection == 1)
	{	
		Output_File = strdup(Argv[i-1]);
		Argv[i-1] = '\0';
		//printf("redirection occured\n");
		//printf ("Argv[%d] is %s Save_Ptr = %s\n",i, Argv[i], Output_File);
	}

	Argv[i] = '\0';
	Count = i;
}

unsigned int Check_History()
{
	int u=0,value=0;
	char counter[10];
	//char Old_Input_Command[256];
	char *Old_Input_Command, *New_Input_Command, *Temp_Char_Local, *Temp_Char_Local_2, *To_Replacement_Str;
	//printf("inside check history %s strlen of input command = %d \n",Input_Command, (int) strlen(Input_Command));
	
	Spaces = 0;

	 for(i=0;i<strlen(Input_Command);i++)
	 {
	 	if(Input_Command[i]=='!')
    	{
    		//check if !'\0'
    		if(Input_Command[i+1] == '\n')
    		{
    			value = h;
    			break;
    		}

    		//check !<space>'\0'
    		for(o=i; Input_Command[o] != '\n' ;o++)
    		{
    			if(Input_Command[o] == ' ')
    				Spaces++;

    		}

    		if((int)strlen(Input_Command) - (i+2) == Spaces)
			{
				value = h;
				break;
			}


    		Temp_Char_Local = strstr (Input_Command,">");
    		if(Temp_Char_Local != NULL)
    		{
			Error_Command();
			return;
    			//Old_Input_Command = strdup(Input_Command);
    			//strtok(Old_Input_Command,"\n");
    			//strtok(Input_Command, ">");
    			//To_Replacement_Str = strdup(Input_Command);
			//	strcat(Input_Command,"\n");
				//printf("Old_Input_Command = %s To_Replacement_Str = %s Input_Command = %s\n",Old_Input_Command, To_Replacement_Str, Input_Command);
			
    		}

    	
    		if((Input_Command[i+1]==' ') && (Input_Command[i+2]>='0' && Input_Command[i+2]<='9'))
  			{
  				if(Input_Command[i+3] ==' ' && Input_Command[i+4]!='\n')
	    		{
	    			Error_Command();
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
	    		if(Input_Command[i+2]==' ' && Input_Command[i+3]!='\n')
	    		{
	    			Error_Command();
    			 	return 0;
    			}
	    		for(j=i+1;j<strlen(Input_Command)-1;j++)
	    		{

	    			counter[u++]=Input_Command[j];
	    		}
	    		counter[u]='\0';
	    		if(counter[0]!='\0')
	    			value=atoi(counter);
	    		break;
	    	}
    	}
	}

	//printf("value = %d\n", value);
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
	//printf("Input_Command = %s\n",Input_Command);
	Temp_Char_4 = strdup(Input_Command);

	if(Check_Redirection_Error() == 1)
	{
		//printf("redirection error in Add_History\n");
		return;
	}

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
				else 
				{
					Error_Command();
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

char *inputString(FILE* fp, size_t size){
//The size is extended by the input with the value of the provisional
    char *str;
    int ch;
    len = 0;
    str = realloc(NULL, sizeof(char)*size);//size is start size
    if(!str)return str;
    while(EOF!=(ch=fgetc(fp)) && ch != '\n'){
        str[len++]=ch;
        if(len==size){
            str = realloc(str, sizeof(char)*(size+=16));
            if(!str)return str;
        }
    }
    str[len++]='\0';

    //printf("len = %d\n",(int)len);

    return realloc(str, sizeof(char)*len);
}


int My_Shell()
{

	int temp;
	Command_Error = 0;
	Redirection = 0;
	Command_Executed = 0;
	
	memset(&Input_Command, 0, sizeof(Input_Command));
	if(Batch_Mode == 0)
	{
		//printf("mysh # ");
		write(STDOUT_FILENO, mysh, strlen(mysh));
	}

	else if(Batch_Mode == 1)
	{
		Return_Val = dup2(Batch_File_Desp,0);
		if(Return_Val == -1)
		{
			Error_Command();
			exit(0);
		}
	}

	Input_Command_Ptr = inputString(stdin, 512);
    //printf("char size = %d", (int)sizeof(char));
    //printf("length of input %d",(int)len);

    if(len > 512)
	{
		//Input_Command[512] = '\0';
		//printf("length of input %d",(int)len);
		for(r=0, q=0; r < 512; r++)
			Input_Command[q++] = Input_Command_Ptr[r];
		Input_Command[q] = '\0';
		//exit(0);
	}
    else
	strcpy(Input_Command,Input_Command_Ptr);

    //printf("Input_Command = %s\n", Input_Command);
	

    strcat(Input_Command,"\n");

    Temp_Char_6 = strtok(Input_Command_Ptr," ");
    //printf("Temp_Char_6 = %s\n", Temp_Char_6);
    
    if(Temp_Char_6 == NULL)
    {
    	if(Batch_Mode == 1)
    		exit(0);
    	
    	else
    		return 0;
    }

    //Return_Val_2 = fgets(Input_Command, 512, stdin);
    
    //printf("size of Input_Command %lu\n",sizeof(Input_Command));
    if(len <= 1 )
    {
    	if(Batch_Mode == 1)
    	{
    		//Error_Command();
    		exit(0);
    	}
    	return 0;
    }

    if(strcmp(Input_Command,"\n")==0)
    {
    	return 0;
    }

    if(Batch_Mode == 1)
    {
	
    	write(STDOUT_FILENO, Input_Command, strlen(Input_Command));
		if(len == 513 && Input_Command[512] == '\n')
    	{
    		temp = 1;
    	}
    	else if(len > 513)
    	{
    		Error_Command();
    		return;
    	}
    }

    //strtok(Input_Command, "\n");
    if(Check_History() == 0)
    	return 0;
    
    strtok(Input_Command, "\n");

    Add_History();

    Parse_Arguments(Input_Command,Argv);

    //Print_Args();

    if(Command_Executed == 1 || Command_Error == 1)
    	return 0;

    if(Argv[0] != NULL)
    {
    	//Execute mysh commands
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


	    if((strcmp(Input_Command, "exit") == 0) || (strcmp(Argv[0], "exit")==0))
		{
			if(Argv[1] != NULL)
			{
				Error_Command();
				return;
			}
				exit(0);
		}
		

		if((strcmp(Argv[0],"history")==0  || strcmp(Input_Command," history")==0 || strcmp(Input_Command," history ")== 0) && Argv[1]==NULL)
		{
			if(Redirection == 1)
			{
				Error_Command();
				return;
			}
			Print_History();
			exit(0);	
		}

	}

	if(Command_Error == 1)
		return 0;


	Return_Val = fork();

	//Check for return value
	if(Return_Val == 0)
	{
		//open(STDOUT_FILENO);
		if(Redirection == 1)
		{
			close(STDOUT_FILENO);
			Redirection_File_Desp = open(Output_File, O_CREAT | O_WRONLY | O_TRUNC, S_IRWXU);
			if(Redirection_File_Desp < 0)
			{

				Error_Command();
				return 0;
			}

		}


		execvp(Argv[0],Argv);
		Error_Command();
		exit(0);
	}

	else if(Return_Val > 0)
	{
			//Error_Command();
			return 0;
	}

	else 
	{
		//printf("fork failed\n");
		exit(1);
	}

	return 0;

}


int main(int argc, char *argv[])
{
	if(argc > 2)
	{
		Error_Command();
		exit(1);
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
