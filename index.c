#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>


int numberOfInstructions=0;
char lines[1024][100];
int instructions[1024];
int data[2048];
int pc = 0;
int opcode;
int R1;
int R2AddressImmediate;
int instruction=0;

void printBinary(int num) {
    for (int i = sizeof(short int) * 8 - 1; i >= 0; i--) {
        int bit = (num >> i) & 1;
       printf("%d", bit);
    }
    printf("\n");
}


void read(){
    char *file = "input.txt";
    FILE *fptr = fopen(file, "r");
    if (fptr == NULL)
    {
        printf("Error! Could not open file\n");
        return;
    }
    // Convert the assembly code to machine code
    while (fgets(lines[numberOfInstructions], 100, fptr) != NULL)
        numberOfInstructions++;
    fclose(fptr);
}

void convertAssemblyToMachineCode()
{
    // Read the assembly code from a file
    read();
    // Store the machine code in the instructions array
    for (int i = 0; i < numberOfInstructions; i++)
    {
        char *line = lines[i];
        char *operation = strtok(line, " ");
        char *firstReg = strtok(NULL, " ");
        char *secondArg = strtok(NULL, " ");
        short int opcode = 0;
        short int instruction = 0;
        char* operations[]={"add","sub","mul","movi","beqz","andi","eor","br","sal","sar","ldr","str"};
        for (; opcode < 12; opcode++)
            if(strcasecmp(operation,operations[opcode])==0)
                break;
        opcode <<=12;
        instruction |= opcode;
        char* regNum=&firstReg[1];
        instruction |= (atoi(regNum)<<6);
        if (secondArg[0]!='R')
            instruction |= atoi(secondArg);
        else
        {
            regNum=&secondArg[1];
            instruction |= atoi(regNum);
        }
        instructions[i] = instruction;
}
}
void fetch()
{
    // Fetch the instruction at the address in the program counter
    // and store it in the instruction register
    instruction = instructions[pc++];
}

void decode()
{
    opcode= (instruction >> 12);
    R1= (instruction >> 6) & 63;
    R2AddressImmediate = instruction & 63;

}



// void execute()
// {
//     int instruction = instructions[pc];
//     int opcode = instruction >> 24;
//     int operand1 = (instruction >> 16) & 0xFF;
//     int operand2 = (instruction >> 8) & 0xFF;
//     int operand3 = instruction & 0xFF;

//     switch (opcode)
//     {
//     case 0:
//         data[operand1] = data[operand2] + data[operand3];
//         break;
//     case 1:
//         data[operand1] = data[operand2] - data[operand3];
//         break;
//     case 2:
//         data[operand1] = data[operand2] * data[operand3];
//         break;
//     case 3:
//         data[operand1] = data[operand2] / data[operand3];
//         break;
//     case 4:
//         data[operand1] = data[operand2] % data[operand3];
//         break;
//     case 5:
//         data[operand1] = data[operand2] & data[operand3];
//         break;
//     case 6:
//         data[operand1] = data[operand2] | data[operand3];
//         break;
//     case 7:
//         data[operand1] = data[operand2] ^ data[operand3];
//         break;
//     case 8:
//         data[operand1] = data[operand2] << data[operand3];
//         break;
//     case 9:
//         data[operand1] = data[operand2] >> data[operand3];
//         break;
//     case 10:
//         data[operand1] = data[operand2] == data[operand3];
//         break;
//     case 11:
//         data[operand1] = data[operand2] != data[operand3];
//         break;
//     case 12:
//         data[operand1] = data[operand2] < data[operand3];
//         break;
//     case 13:
//         data[operand1] = data[operand2] <= data[operand3];
//         break;
//     case 14:
//         data[operand1] = data[operand2] > data[operand3];
//         break;
//     case 15:
//         data[operand1] = data[operand2] >= data[operand3];
//         break;
//     case 16:
//         data[operand1] = data[ operand2];
//         break;
//     }
// }



int main()
{

    convertAssemblyToMachineCode();

    for (int i = 0; i < 3; i++)
    {
        fetch();
        decode();
        printf("Instruction %d\n", i);
        printf("%d\n", instructions[i]);
        printBinary(instructions[i]);
        printf("Opcode: %d\n", opcode);
        printf("R1: %d\n", R1);
        printf("R2AddressImmediate: %d\n", R2AddressImmediate);
    }




    return 0;
}
