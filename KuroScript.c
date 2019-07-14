/*************************************************************************
 CLASS INFORMATION //Idk why this is still here
 -----------------
   Program Name: Up to your interpretation
   Programmer:   Nathan Campbell
   Instructor:   Myself, StackOverflow, and Me
   Date Due:     Someday, maybe.

 DOCUMENTATION
 -------------
    Interprets a stupid language.
 ************************************************************************/
#include <stdio.h>
#include <stdlib.h>

#define FILENAME "Fibonacci.txt"
#define NEXTCHAR (testChar = fgetc(fileIn))
#define DEFAULT_OUTPUT stdout
#define DEFAULT_INPUT stdin
#define DEFAULT_ERROR stderr
#define DEFAULT_MEMORY_SIZE 5
#define NUM_OF_ELEMENTS (int)(sizeof(memory)/sizeof(int))
//#define DEBUGGER debug(DEFAULT_OUTPUT, fileIn, memory, testChar, 1);

    

int* allocateResources (FILE *output, FILE *fileIn, int *testChar, int *memorySize);
void debug             (FILE *output, FILE *fileIn, int *memory, int testChar, int display);

//Memory Changing
int increment          (FILE *output, FILE *fileIn, int *memory, int *testChar);
int decrement          (FILE *output, FILE *fileIn, int *memory, int *testChar);
int setValue           (FILE *output, FILE *fileIn, int *memory, int *testChar);

//Memory Evaluation
int evaluateCell       (FILE *output, FILE *fileIn, int *memory, int *testChar);

//Logic
int evaluateCondition  (FILE *output, FILE *fileIn, int *memory, int *testChar);

//Flow Changing
int conditionTrue      (FILE *output, FILE *fileIn, int *memory, int *testChar);
int jumpTo             (FILE *output, FILE *fileIn, int *memory, int *testChar);
int loop               (FILE *output, FILE *fileIn, int *memory, int *testChar);

//IO
int outputASCII        (FILE *output, FILE *fileIn, int *memory, int *testChar);
int comment            (FILE *output, FILE *fileIn, int *memory, int *testChar);
int input              (FILE *output, FILE *fileIn, int *memory, int *testChar);

int recursiveScan      (FILE *output, FILE *fileIn, int *memory, int *testChar, char until);

void safeExit          (FILE *fileIn, int *memory);

int main(void)
{

    int memorySize = 5,
        testChar = 0,
        *memory;
    
    FILE *fileIn = fopen(FILENAME, "r");
    //fseek(fileIn, SEEK_CUR, sizeof(int)*2);
    testChar = fgetc(fileIn);
    if((memory = allocateResources(DEFAULT_OUTPUT, fileIn, &testChar, &memorySize)) != NULL)
    {
        fseek(fileIn, 0, SEEK_SET);
        recursiveScan(DEFAULT_OUTPUT, fileIn, memory, &testChar, EOF);
    }
    else
    {
        fprintf(DEFAULT_OUTPUT, "\nMemory Allocation Failure.\n");
    }
    free(memory);
    fclose(fileIn);
    return 0;
}


int* allocateResources(FILE *output, FILE *fileIn, int *testChar, int *memorySize)
{
    int *memory;

    if(*testChar == '_')
    {
        //DEBUGGER
        if(fscanf(fileIn, "%d", memorySize))
        {
            if(*memorySize > 0)
            {
                if((memory = (int*) malloc(sizeof(int) * *memorySize)) == NULL)
                {
                    //DEBUGGER
                    
                    fprintf(DEFAULT_OUTPUT, "\nAllocation failure at %ld\n", ftell(fileIn));
                }
                else
                {
                   // DEBUGGER
                  // printf("\nAllocation Successfull?\n");
                }
            }
            else
            {
                fprintf(DEFAULT_OUTPUT, "\nAllocation failure at %ld. memorySize is invalid\n", ftell(fileIn));
            }
        }
        else
        {
            fprintf(DEFAULT_OUTPUT, "\nElement after allocation indicator is not an integer at %ld", ftell(fileIn));
            *memorySize = DEFAULT_MEMORY_SIZE;
            if((memory = (int*) malloc(sizeof(int) * *memorySize)) == NULL)
            {
               // DEBUGGER
                *testChar = '.';
                fprintf(DEFAULT_OUTPUT, "\nAllocation failure at %ld\n", ftell(fileIn));
            }
        }
    }
    else
    {
        if((memory = (int*) malloc(sizeof(int) * *memorySize)) == NULL)
        {
          //  DEBUGGER
            fprintf(DEFAULT_OUTPUT, "\nAllocation failure at %ld\n", ftell(fileIn));
        }
        else
        {
           // DEBUGGER
//           printf("%d memory cells allocated", *memorySize);
        }
    }
    
    //DEBUGGER
    if(*memorySize > 0)
    {
        for(int i = 0; i < *memorySize; ++i)
        {
            memory[i] = i;
        }
    }
    
    return memory;
}

/*************************************************************************
    Debug function, used for testing.
 ************************************************************************/
void debug(FILE *output, FILE *fileIn, int *memory, int testChar, int display)
{
    if(display)
    {
        fprintf(output, "\n-----------------------\n");
        fprintf(output, "\nftell result :%ld\n", ftell(fileIn));
        fprintf(output, "testChar Char:%c\n", testChar);
        fprintf(output, "testChar int :%d\n", testChar);
        fprintf(output, "memorySize    :%d\n", NUM_OF_ELEMENTS);
        for(int i = 0; i < NUM_OF_ELEMENTS; ++i)
        {
            fprintf(output, "memory cell %d~ int:%d ~char:%c\n", i, memory[i], 
        memory[i]);
        }
        fprintf(output, "\n-----------------------\n");
    }
    
    return;
}


/*************************************************************************
    Increments a defined element in memory.
 ************************************************************************/
int increment(FILE *output, FILE *fileIn, int *memory, int *testChar)
{   
    int errorLevel = 0;

    if(fscanf(fileIn, "%d", testChar))
    {
        if(*testChar <= (NUM_OF_ELEMENTS + 1) && *testChar >= 0)
        {
            ++memory[*testChar];
        }
        else
        {
            printf("\n%d\n%d", *testChar, NUM_OF_ELEMENTS);
            fprintf(DEFAULT_OUTPUT, "Invalid target of incrementation at %ld", ftell(fileIn));
            safeExit(fileIn, memory);
        }
    }
    else
    {
        fprintf(DEFAULT_OUTPUT, "Invalid target of incrementation at %ld", ftell(fileIn));
        safeExit(fileIn, memory);
    }

    return errorLevel;
}

/*************************************************************************
    Decrements a defined element in memory.
 ************************************************************************/
int decrement(FILE *output, FILE *fileIn, int *memory, int *testChar)
{   
    int errorLevel = 0;

    if(fscanf(fileIn, "%d", testChar))
    {
        if(*testChar <= (NUM_OF_ELEMENTS - 1) && *testChar >= 0)
        {
            --memory[*testChar];
        }
        else
        {
            fprintf(DEFAULT_OUTPUT, "Invalid target of decrementation at %ld", ftell(fileIn));
            safeExit(fileIn, memory);
        }
    }
    else
    {
        fprintf(DEFAULT_OUTPUT, "Invalid target of decrementation at %ld", ftell(fileIn));
        safeExit(fileIn, memory);
    }

    return errorLevel;
}

/*************************************************************************
    Outputs a string of characters and integers if defined in memory.
    
    Syntax: @Hello, World) outputs "Hello, world"
            @[0)           outputs whatever is in memory cell 0 as char
            @#0)           outputs whatever is in memory cell 0 as int
            
    Future:
            @ [1,6)        outputs whatever is in cells 1 to 6 as char
            @ #1,6)        outputs whatever is in cells 1 to 6 as int
 ************************************************************************/
int outputASCII(FILE *output, FILE *fileIn, int *memory, int *testChar)
{
    int errorLevel = 0;
    
    *testChar = fgetc(fileIn);
    //DEBUGGER
    while(*testChar != EOF && *testChar != ')')
    {
        if(*testChar == '[')
        {
            if(fscanf(fileIn, "%d", testChar))
            {
                if(*testChar <= (NUM_OF_ELEMENTS - 1) || *testChar >= 0)
                {
              //      DEBUGGER
                    fprintf(DEFAULT_OUTPUT, "%c", memory[*testChar]);
                }
                else
                {
                //    DEBUGGER
                    *testChar = '.';
                    fprintf(DEFAULT_OUTPUT, "At character %ld, "
    "memory cell referenced does not exist", ftell(fileIn));
                    safeExit(fileIn, memory);
                    break;
                }
            }
            else
            {
               // DEBUGGER
                *testChar = '.';
                fprintf(DEFAULT_OUTPUT, "Element after memory " 
    "cell is not an integer at %ld", ftell(fileIn));
                safeExit(fileIn, memory);
            }
        }
        else if(*testChar == '#')
        {
            if(fscanf(fileIn, "%d", testChar))
            {
                if(*testChar <= (NUM_OF_ELEMENTS - 1) || *testChar >= 0)
                {
                //    DEBUGGER
                    fprintf(DEFAULT_OUTPUT, "%d", memory[*testChar]);
                }
                else
                {
                    //    DEBUGGER
                    fprintf(DEFAULT_OUTPUT, "At character %ld, "
    "memory cell referenced does not exist", ftell(fileIn));
                    safeExit(fileIn, memory);
                }
            }
            else
            {
                // DEBUGGER
                fprintf(DEFAULT_OUTPUT, "Element after memory " 
    "cell is not an integer at %ld", ftell(fileIn));
                safeExit(fileIn, memory);
            }
        }
        else
        {
            //DEBUGGER
            fprintf(DEFAULT_OUTPUT, "%c", *testChar);
        }
        *testChar = fgetc(fileIn);
    }

    return errorLevel;
    
}

/*************************************************************************
    Disreguards commented area. <comment>
 ************************************************************************/
int comment(FILE *output, FILE *fileIn, int *memory, int *testChar)
{
    int errorLevel = 0;

    while((*testChar = fgetc(fileIn)) != '>' && *testChar != EOF)
    {
        // Consider your past failures
        //DEBUGGER
    }
    *testChar = ' ';

    return errorLevel;   
}

/*************************************************************************
    Changes flow of logic depending on condition
    
    Syntax:
        ?<firstArgument><operator><secondArgument>
        <true path>
        }
        <false path>
        ~
    
 ************************************************************************/
int conditionTrue(FILE *output, FILE *fileIn, int *memory, int *testChar)
{
    int errorLevel = 0,
        endFalsePath = 1;

    if(evaluateCondition(DEFAULT_OUTPUT, fileIn, memory, testChar))
    {
        recursiveScan(DEFAULT_OUTPUT, fileIn, memory, testChar, '}');
        while(*testChar != '~')
        {
            *testChar = fgetc(fileIn);
        }
    }
    else
    {
        while(endFalsePath != 0)   //while(*testChar != '}')
        {
            *testChar = fgetc(fileIn);
            if(*testChar == '?')
            {
                printf("\ntestChar %d/%c, at %ld\n", *testChar, *testChar, ftell(fileIn));
                ++endFalsePath;
            }
            if(*testChar == '}')
            {
                --endFalsePath;
            }
        }
        recursiveScan(DEFAULT_OUTPUT, fileIn, memory, testChar, '~');
    }

    return errorLevel;
}

int recursiveScan(FILE *output, FILE *fileIn, int *memory, int *testChar, char until)
{
    int errorLevel = 1;

    while((*testChar = fgetc(fileIn)) != EOF && *testChar != until)
    {
        switch(*testChar)
        {
            case '+':
                increment(DEFAULT_OUTPUT, fileIn, memory, testChar);            
            break;
            
            case '-':
                decrement(DEFAULT_OUTPUT, fileIn, memory, testChar);            
            break;
            
            case ':':
                loop(DEFAULT_OUTPUT, fileIn, memory, testChar);
            break;
            
            case '=':
                setValue(DEFAULT_OUTPUT, fileIn, memory, testChar);
            break;
            
            case '@':
                outputASCII(DEFAULT_OUTPUT, fileIn, memory, testChar);
            break;
            
            case '<':
                comment(DEFAULT_OUTPUT, fileIn, memory, testChar);            
            break;
            
            case '?':
                conditionTrue(DEFAULT_OUTPUT, fileIn, memory, testChar);
            break;
            
            case '`':
                jumpTo(DEFAULT_OUTPUT, fileIn, memory, testChar);
            break;
            
        }
        
        if(*testChar == '.')
        {
            break;
        }
    }
    
    return errorLevel;
}

/*************************************************************************
    Takes in a cell or literal, then an operator, then a cell or literal
    
    Then they are evaluated to 1 or 0.
 ************************************************************************/
int evaluateCondition(FILE *output, FILE *fileIn, int *memory, int *testChar)
{
    int evaluation = 0,
        firstArgument,
        secondArgument;
    char operator = '0';
    
    //look for a valid first argument
    if(fscanf(fileIn, "%d", testChar))
    {
        firstArgument = *testChar;
    }
    else
    {
        *testChar = fgetc(fileIn);
        if(*testChar == '[')
        {
            firstArgument = evaluateCell(DEFAULT_OUTPUT, fileIn, memory, testChar);
        }
    }
    //look for a valid operator
    if(fscanf(fileIn, "%d", testChar))
    {
        fprintf(DEFAULT_OUTPUT, "\nInvalid operator at %ld\n", ftell(fileIn));
        safeExit(fileIn, memory);
    }
    else
    {
        *testChar = fgetc(fileIn);
        if(*testChar == '>' || *testChar == '<' || *testChar == '=')
        {

            operator = *testChar;
        }
    }
    
    //look for a valid second argument
    if(fscanf(fileIn, "%d", testChar))
    {
        secondArgument = *testChar;
    }
    else
    {
        *testChar = fgetc(fileIn);
        if(*testChar == '[')
        {
            secondArgument = evaluateCell(DEFAULT_OUTPUT, fileIn, memory, testChar);
        }
    }
    
    switch(operator)
    {
        case '<':
            if(firstArgument < secondArgument)
            {
                evaluation = 1;
            }
            else
            {
                evaluation = 0;
            }
        break;
       
        case '>':
            if(firstArgument > secondArgument)
            {
                evaluation = 1;
            }
            else
            {
                evaluation = 0;
            }
        break;
        
        case '=':
            if(firstArgument == secondArgument)
            {
                evaluation = 1;
            }
            else
            {
                evaluation = 0;
            }
        break;
        default:
        fprintf(DEFAULT_OUTPUT, "no valid operator at%ld", ftell(fileIn));
    }
    
    return evaluation;
}

/*************************************************************************
    Gets the value of the cell number that is next in the file.
 ************************************************************************/
int evaluateCell(FILE *output, FILE *fileIn, int *memory, int *testChar)
{
    int cellValue;

    if(fscanf(fileIn, "%d", testChar))
    {
        if(*testChar <= NUM_OF_ELEMENTS && *testChar >= 0)
        {
            cellValue = memory[*testChar];
        }
        else
        {
            printf("\ntestchar:%d or %c", *testChar, *testChar);
            fprintf(DEFAULT_OUTPUT, "At character %ld, memory cell "
    "referenced does not exist", ftell(fileIn));
            safeExit(fileIn, memory);
        }
    }
    else
    {
        fprintf(DEFAULT_OUTPUT, "Memory cell referenced is invalid at"
    " %ld", ftell(fileIn));
        safeExit(fileIn, memory);
    }
    
    return cellValue;
}
/*************************************************************************
    Safely exits the interpreter at any time.
 ************************************************************************/
void safeExit(FILE *fileIn, int *memory)
{
    free(memory);
    fclose(fileIn);
    exit(0);
    return;
}

/*************************************************************************
    Sets the value of one memory cell to a literal or the value of another
 ************************************************************************/
int setValue(FILE *output, FILE *fileIn, int *memory, int *testChar)
{
    int errorLevel = 0,
        cellToChange,
        newValue;

    if(fscanf(fileIn, "%d", testChar))
    {
        if(*testChar <= (NUM_OF_ELEMENTS - 1) || *testChar >= 0)
        {
            cellToChange = *testChar;
        }
        else
        {
            fprintf(DEFAULT_OUTPUT, "At character %ld, memory cell "
    "referenced does not exist", ftell(fileIn));
            safeExit(fileIn, memory);
        }
    }
    else
    {
        if((*testChar = fgetc(fileIn)) == '[')
        {
            fprintf(DEFAULT_OUTPUT, "'[' is not needed at "
    "%ld", ftell(fileIn));
            safeExit(fileIn, memory);
        }
        else
        {
            fprintf(DEFAULT_OUTPUT, "Assignment is not to an integer at "
    "%ld", ftell(fileIn));
            safeExit(fileIn, memory);
        }
    }
    if(fscanf(fileIn, "%d", testChar))
    {
        fprintf(DEFAULT_OUTPUT, "'^' expected after cell to be "
        "asigned at %ld", ftell(fileIn));
        safeExit(fileIn, memory);
    }
    else
    {
        *testChar = fgetc(fileIn);
        if(*testChar != '^')
        {
            fprintf(DEFAULT_OUTPUT, "'^' expected after cell to be "
    "asigned at %ld", ftell(fileIn));
            safeExit(fileIn, memory);
        }
        else
        {
            if(fscanf(fileIn, "%d", testChar))
            {
                newValue = *testChar;
            }
            else
            {
                *testChar = fgetc(fileIn);
                if(*testChar == '[')
                {
                    newValue = evaluateCell(DEFAULT_OUTPUT, fileIn, memory, testChar);
                }
                else
                {
                    newValue = *testChar;
                }
            }
        }
        memory[cellToChange] = newValue;
    }
    
    return errorLevel;
}

/*************************************************************************
    Jumps to the specified point in the KuroScript file.
 ************************************************************************/
int jumpTo(FILE *output, FILE *fileIn, int *memory, int *testChar)
{
    int errorLevel = 0;

    if(fscanf(fileIn, "%d", testChar))
    {
        if(*testChar == EOF)
        {
            fprintf(DEFAULT_OUTPUT, "jump location not available at %ld", ftell(fileIn));
            safeExit(fileIn, memory);
        }
        else
        {
            fseek(fileIn, *testChar, SEEK_SET);
        }
    }
    else
    {
        *testChar = fgetc(fileIn);
        if(*testChar == '[')
        {
            fseek(fileIn, evaluateCell(DEFAULT_OUTPUT, fileIn, memory, testChar), SEEK_SET);
        }
        else
        {
            fprintf(DEFAULT_OUTPUT, "jump location not available at %ld", ftell(fileIn));
            safeExit(fileIn, memory);
        }
    }
        
    return errorLevel;
}

/*************************************************************************
    Loops until a condition is false
    
    :<condition> <instructions>;
    
 ************************************************************************/
int loop(FILE *output, FILE *fileIn, int *memory, int *testChar)
{
    int errorLevel = 0,
    loopLocation = ftell(fileIn) - 1;

    if(evaluateCondition(DEFAULT_OUTPUT, fileIn, memory, testChar))
    {
        recursiveScan(DEFAULT_OUTPUT, fileIn, memory, testChar, ';');
        fseek(fileIn, loopLocation, SEEK_SET);
    }
    else
    {
        while(*testChar != ';' && *testChar != EOF)
        {
            *testChar = fgetc(fileIn);
        }
    }

    return errorLevel;
}
