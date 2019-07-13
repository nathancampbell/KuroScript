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

#define FILENAME "conditions.txt"
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

//Memory Evaluation
int evaluateCell       (FILE *output, FILE *fileIn, int *memory, int *testChar);

//Logic
int evaluateCondition  (FILE *output, FILE *fileIn, int *memory, int *testChar);

//Flow Changing
int conditionTrue      (FILE *output, FILE *fileIn, int *memory, int *testChar);

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
    
    if(*testChar == '+')
    {
        if(fscanf(fileIn, "%d", testChar))
        {
            if(*testChar <= NUM_OF_ELEMENTS && *testChar >= 0)
            {
                fprintf(DEFAULT_OUTPUT, "Invalid target of incrementation at %ld", ftell(fileIn));
                safeExit(fileIn, memory);
            }
            else
            {
                ++memory[*testChar];
            }
        }
        else
        {
            fprintf(DEFAULT_OUTPUT, "Invalid target of incrementation at %ld", ftell(fileIn));
            safeExit(fileIn, memory);
        }
    }
    
    return errorLevel;
}

/*************************************************************************
    Decrements a defined element in memory.
 ************************************************************************/
int decrement(FILE *output, FILE *fileIn, int *memory, int *testChar)
{   
    int errorLevel = 0;
    
    if(*testChar == '-')
    {
        if(fscanf(fileIn, "%d", testChar))
        {
            if(*testChar <= NUM_OF_ELEMENTS && *testChar >= 0)
            {
                fprintf(DEFAULT_OUTPUT, "Invalid target of decrementation at %ld", ftell(fileIn));
                safeExit(fileIn, memory);
            }
            else
            {
                --memory[*testChar];
            }
        }
        else
        {
            fprintf(DEFAULT_OUTPUT, "Invalid target of decrementation at %ld", ftell(fileIn));
            safeExit(fileIn, memory);
        }
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
    
    if(*testChar == '@')
    {
        *testChar = fgetc(fileIn);
        //DEBUGGER
        while(*testChar != ')')
        {
            comment(DEFAULT_OUTPUT, fileIn, memory, testChar);
            if(*testChar == '[')
            {
                if(fscanf(fileIn, "%d", testChar))
                {
                    if(*testChar <= NUM_OF_ELEMENTS || *testChar >= 0)
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
                    if(*testChar <= NUM_OF_ELEMENTS || *testChar >= 0)
                    {
                  //      DEBUGGER
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
    }
    
    return errorLevel;
    
}

/*************************************************************************
    Disreguards commented area.
 ************************************************************************/
int comment(FILE *output, FILE *fileIn, int *memory, int *testChar)
{
    int errorLevel = 0;
    
    if(*testChar == '<')
    {
        while((*testChar = fgetc(fileIn)) != '>')
        {
            // Consider your past failures
            //DEBUGGER
        }
        *testChar = ' ';
    }
    
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
    int errorLevel = 0;
    
    if(*testChar == '?')
    {
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
            while(*testChar != '}')
            {
                *testChar = fgetc(fileIn);
            }
            recursiveScan(DEFAULT_OUTPUT, fileIn, memory, testChar, '~');
        }
    }
    
    return errorLevel;
}

int recursiveScan(FILE *output, FILE *fileIn, int *memory, int *testChar, char until)
{
    int errorLevel = 1;
    
    while((*testChar = fgetc(fileIn)) != EOF && *testChar != until)
    {
        //DEBUGGER
        // Increment
        increment(DEFAULT_OUTPUT, fileIn, memory, testChar);
        
        // Decriment
        decrement(DEFAULT_OUTPUT, fileIn, memory, testChar);
        
        // Output as an ascii character
        outputASCII(DEFAULT_OUTPUT, fileIn, memory, testChar);

        // Commenting
        comment(DEFAULT_OUTPUT, fileIn, memory, testChar);
        
        // Getting Input
        input(DEFAULT_OUTPUT, fileIn, memory, testChar);
        
        // True condition flow change
        conditionTrue(DEFAULT_OUTPUT, fileIn, memory, testChar);
        
        
        // Terminate
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
        if(*testChar <= NUM_OF_ELEMENTS || *testChar >= 0)
        {
            cellValue = memory[*testChar];
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
    
    return;
}
/*************************************************************************
    Unfinished input function
 ************************************************************************/
int input(FILE *output, FILE *fileIn, int *memory, int *testChar)
{
    int errorLevel = 0;
    
    if(*testChar == '$')
    {
        if(fscanf(DEFAULT_INPUT, "%d", testChar))
        {
            //input is an integer
        }
        
        else
        {
            //input is a character
        }
    }

 
   return errorLevel;    
}
