#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define tapeSize 5000

char memory[tapeSize];
char *instructions;
char *programCounter;
char *tapeHead;

//Initializes both the tape and the program instructions
void init(char *program)
{
    instructions = (char *)malloc(sizeof(char) * strlen(program));

    tapeHead = &memory[0];
    programCounter = &instructions[0];

    memset(memory, 0, tapeSize);
    memset(instructions, 0, strlen(program));
    strcpy(instructions, program);
}

//Corresponds to ">" instruction
void incrementPointer(void)
{
    if(tapeHead == (&memory[0] + tapeSize - 1))
        tapeHead = &memory[0];
    else
        tapeHead++;
}

//Corresponds to "<" instruction
void decrementPointer(void)
{
    if(tapeHead == &memory[0])
        tapeHead = &memory[0] + tapeSize - 1;
    else
        tapeHead--;
}

//Corresponds to "+" instruction
void increment(void)
{
    (*tapeHead)++;
}

//Corresponds to "-" instruction
void decrement(void)
{
    (*tapeHead)--;
}

//Corresponds to "." instruction
void input(void)
{
    scanf("%c", tapeHead);
}

//Corresponds to "," instruction
void output(void)
{
    printf("%c", *tapeHead);
}

//Interprets the BF program instructions
void interpret(void)
{
    int loopCount;

    loopCount = 0;

    while(programCounter <= instructions + strlen(instructions))
    {
        switch(*programCounter)
        {
        case '>':
            incrementPointer();
            break;
        case '<':
            decrementPointer();
            break;
        case '+':
            increment();
            break;
        case '-':
            decrement();
            break;
        case '.':
            output();
            break;
        case '[':
            if(!*tapeHead)
            {
                loopCount++;

                while(loopCount != 0 || *programCounter != ']')
                {
                    if(*(programCounter + 1) == '[')    //Found a nested loop
                        loopCount++;
                    else if(*(programCounter + 1) == ']')
                        loopCount--;

                    programCounter++;
                }
            }
            break;
        case ']':
            if(*tapeHead)
            {
                loopCount++;

                while(loopCount != 0 || *programCounter != '[')
                {
                    if(*(programCounter - 1) == ']')
                        loopCount++;
                    else if(*(programCounter - 1) == '[')
                        loopCount--;

                    programCounter--;
                }
            }
            break;
        }

        programCounter++;   //Always moving forward
    }
}

//Takes in a BF program as a command line argument. Anything else crashes. Bad BF programs will crash this too!
int main(int argc, char **argv)
{
    if(argc != 2)   //Not the right amount of arguments
        printf("Invalid input");
    else
    {
        init(argv[1]);

        interpret();
    }
}
