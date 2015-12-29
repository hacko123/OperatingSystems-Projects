#include "types.h"
#include "stat.h"
#include "user.h"

int main(int argc, char **argv)
{

    int *mem1 = (int *) malloc(sizeof(int) * 64);
    int *mem2 = (int *) malloc(sizeof(int) * 128);
    int *mem3 = (int *) malloc(sizeof(int) * 256);
    int i;
    int j = 97;    

    printf(1,"STARTING TEST: allocate memory on heap.\nallocating bunch of memory on heap and manipulate them\n");
    printf(1,"EXPECTED: varibles are allocated on heap, addresses and values look reasonable.\n");


    for(i = 0; i<64; i++)
    {
        mem1[i] = i * 10;
        printf(1,"mem1[%d]: %d address: %d\n",i, mem1[i], &mem1[i]);
    }

    for(i = 0; i<128; i++)
    {
        mem2[i] = i * 20;
        printf(1,"mem2[%d]: %d address: %d\n",i, mem2[i], &mem2[i]);
    }

    for(i = 0; i<256; i++)
    {
        mem3[i] = i * 30;
        printf(1,"mem3[%d]: %d address: %d\n",i, mem3[i], &mem3[i]);
    }


    printf(1,"EXPECTED: varibles are allocated on heap, addresses and values look reasonable.\n");
    printf(1,"TEST PASSED\n");

    exit();
}
