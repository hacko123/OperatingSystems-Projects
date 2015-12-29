#include <stdio.h>
#include <stdlib.h>
#include <string.h>

char *inputString(FILE* fp, size_t size){
//The size is extended by the input with the value of the provisional
    char *str;
    int ch;
    size_t len = 0;
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

    return realloc(str, sizeof(char)*len);
}

int main(void)
{
    char buffer[10];
    char *input = 0;
    size_t cur_len = 0;

    char *m;

    printf("input string : ");
    m = inputString(stdin, 10);
    printf("%s\n", m);

    free(m);

    // while (fgets(buffer, sizeof(buffer), stdin) != 0)
    // {
    //     size_t buf_len = strlen(buffer);
    //     printf("buffer = %s length = %d\n",buffer, (int)buf_len);
    //     char *extra = realloc(input, buf_len + cur_len + 1);
    //     if (extra == 0)
    //         break;
    //     input = extra;
    //     strcpy(input + cur_len, buffer);
    //     printf("input = %s length = %d\n",input, (int)strlen(input));
    //     cur_len += buf_len;
    // }
    // printf("%s [%d]", input, (int)strlen(input));
    // free(input);
    return 0;
}