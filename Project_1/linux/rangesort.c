#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <ctype.h>
#include <string.h>
#include "sort.h"
#include <sys/types.h>
#include <sys/stat.h>
#define MAX_HIGH 4294967296

unsigned int my_atoi(char *str)
{
	long res = 0,i; 
 
    for (i = 0; i<strlen(str); ++i) {
    	if(str[i]>='0' && str[i]<='9')
        	res = res*10 + str[i] - '0';
        else {
        	fprintf(stderr, "Error: Invalid range value\n" );
    		exit(1);
        }
    }
	if(res>MAX_HIGH)
	{
		fprintf(stderr, "Error: Invalid range value\n" );
		exit(1);
	}
    return res;
}
void quick_sort (rec_t *a, int n) {
    unsigned int i, j, p;
    rec_t t;
    if (n < 2)
        return;
    p = a[n / 2].key;
    for (i = 0, j = n - 1;; i++, j--) {
        while (a[i].key < p)
            i++;
        while (p < a[j].key)
            j--;
        if (i >= j)
            break;
        t = a[i];
        a[i] = a[j];
        a[j] = t;
    }
    quick_sort(a, i);
    quick_sort(a + i, n - i);
}

void usage() 
{
    fprintf(stderr, "Usage: rangesort -i inputfile -o outputfile -l lowvalue -h highvalue\n");
    exit(1);
}
int main(int argc,char *argv[])
{
	char *inputFile,*outputFile;
	unsigned int lkey,hkey;
	int i=0,j=0,count=0,key;
	int c,c1=0,c2=0,c3=0,c4=0;
    opterr = 0;
    while ((c = getopt(argc, argv, "i:o:l:h:")) != -1) {
	switch (c) {
	case 'i':
	    inputFile = strdup(optarg);
	    c1++;
	    break;
	case 'o':
	    outputFile = strdup(optarg);
	    c2++;
	    break;
	case 'l':
	    lkey = my_atoi(optarg);
	    c3++;
	    break;
	case 'h':
	    hkey = my_atoi(optarg);
	    c4++;
	    break;
	default:
	   usage();
	}
    }
    if(c1!=1 || c2!=1 || c3!=1 || c4!=1)
    	usage();
    // for(i=1;i<9;i++)
    // 	if(argv[i]==NULL)
    // 		usage();
    if(lkey>hkey) {
    	fprintf(stderr, "Error: Invalid range value\n" );
    	exit(1);
    }

	int fd=open(inputFile,O_RDONLY);
	if (fd < 0) {
	fprintf(stderr, "Error: Cannot open file %s\n", inputFile);
	exit(1);
    }
    rec_t r;
    int rc;
    while (1) {	
	rc = read(fd, &r, sizeof(rec_t));
	if (rc == 0) // 0 indicates EOF
	    break;
	if (rc < 0) {
	    fprintf(stderr, "Error: Cannot open file %s", inputFile);
	    exit(1);
	}
	if(r.key>=lkey && r.key<=hkey) 
		count++;
}
	//printf("\n%d\n",count);
	rec_t *array;
	array = (rec_t *)calloc(count,sizeof(rec_t)); 
	 if(array==NULL) {
		fprintf(stderr, "Error: Memory Allocation Failed");
		exit(1);
	}
	lseek(fd,0,SEEK_SET);
	i=0;
	while (1) {	
	rc = read(fd, &r, sizeof(rec_t));
	if (rc == 0) // 0 indicates EOF
	    break;
	if (rc < 0) {
	    fprintf(stderr, "Error: Cannot open file %s", inputFile);
	    free(array);
	    exit(1);
	}
	if(r.key>=lkey && r.key<=hkey) {
		array[i] = r;
		i++;
	}
}
quick_sort(array,count);
	// rec_t temp;
	// for (i = 0; i < count-1; i++)      
 //       for (j = 0; j < count-i-1; j++)   
 //           if (array[j].key > array[j+1].key) {
 //              	temp=array[j];
 //               	array[j]=array[j+1];
 //               	array[j+1]=temp;
 //               }
  
int od = open(outputFile, O_WRONLY|O_CREAT|O_TRUNC, S_IRWXU);
    if (od < 0) {
    fprintf(stderr, "Error: Cannot open file %s\n", outputFile);
    free(array);
	exit(1);
    }
    for(i=0;i<count;i++) {
	int rc = write(od, &array[i], sizeof(rec_t));
	if (rc != sizeof(rec_t)) {
	    fprintf(stderr, "Error: Cannot open file %s", outputFile);
	    free(array);
	    exit(1);	    
	}
    }

    close(fd);
    close(od);
}



 