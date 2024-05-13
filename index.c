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
int GPRS[64];
int SREG;


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

    // Sign extend the immediate value
    if (R2AddressImmediate&(1<<6))
        R2AddressImmediate |= 0xFFC0;

}

void evalFlags(int op1,int op2,int res){
    // Zero flag
    if (res==0)
        SREG |= 1;
    else
        SREG &= ~(1);

    // Negative flag
    if (res<0)
        SREG |= 1<<2;
    else
        SREG &= ~(1<<2);


    // Overflow flag
    if ((op1>0 && op2>0 && res<0) || (op1<0 && op2<0 && res>0))
        SREG |= 1<<3;
    else
        SREG &= ~(1<<3);

    // Carry flag
    if (res & 1<<8)
        SREG |= 1<<4;
    else
        SREG &= ~(1<<4);

    // Sign Flag
    int N=SREG & (1<<2);
    int V=SREG & (1<<3);
    if (N^V)
        SREG |= 1<<1;
    else
        SREG &= ~(1<<1);

}

void execute()
{

    switch (opcode)
    {
    case 0: // add
        GPRS[R1] = GPRS[R1] + GPRS[R2AddressImmediate];   // R1=R1+R2
        evalFlags(GPRS[R1], GPRS[R1], GPRS[R2AddressImmediate]);
        break;
    case 1: // sub
        GPRS[R1] = GPRS[R1] - GPRS[R2AddressImmediate];   // R1=R1-R2
        evalFlags(GPRS[R1], GPRS[R1], GPRS[R2AddressImmediate]);
        break;
    case 2: // MUL
        GPRS[R1] = GPRS[R1] * GPRS[R2AddressImmediate];   // R1=R1*R2
        evalFlags(GPRS[R1], GPRS[R1], GPRS[R2AddressImmediate]);
        break;
    case 3: // MOVI
        GPRS[R1] = R2AddressImmediate;   // R1=immediate
        break;
    case 4: // BEQZ
        if (GPRS[R1] == 0)
            pc +=1+ R2AddressImmediate;
        break;
    case 5: // ANDI
        GPRS[R1] = GPRS[R1] & R2AddressImmediate;   // R1=R1&R2
        evalFlags(GPRS[R1], GPRS[R1], R2AddressImmediate);
        break;
    case 6: // EOR
        GPRS[R1] = GPRS[R1] ^ GPRS[R2AddressImmediate];   // R1=R1^R2
        evalFlags(GPRS[R1], GPRS[R1], GPRS[R2AddressImmediate]);
        break;
    case 7: // BR
        GPRS[R1] <<= 6;
        pc = GPRS[R1] | GPRS[R2AddressImmediate];   // pc=R1 || R2
        break;
    case 8: // SAL
        GPRS[R1] = GPRS[R1] << R2AddressImmediate;   // R1=R1<<IMM
        evalFlags(GPRS[R1], GPRS[R1], R2AddressImmediate);
        break;
    case 9: // SAR
        GPRS[R1] = GPRS[R1] >> R2AddressImmediate;   // R1=R1>>IMM
        evalFlags(GPRS[R1], GPRS[R1], R2AddressImmediate);
        break;
    case 10: // LDR
        GPRS[R1] = data[R2AddressImmediate];   // R1=MEM[R2]
        break;
    case 11: // STR
        data[R2AddressImmediate] = GPRS[R1];   // MEM[R2]=R1
        break;
    default:
        break;
    }
}



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
