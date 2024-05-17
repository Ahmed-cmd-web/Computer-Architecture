
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void printBinary(int num) {
    for (int i = sizeof(short int) * 8 - 1; i >= 0; i--) {
        int bit = (num >> i) & 1;
       printf("%d", bit);
    }
    printf("\n");
}



int main()
{
   printf("%d\n",sizeof(short int) * 8);
   printf("\n");
   printf("%d\n",sizeof(int) * 8);

    return 0;
}



// -5
// 100101