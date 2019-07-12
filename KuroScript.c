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

#define FILENAME "program.txt"
#define NEXTCHAR (testChar = fgetc(fileIn))
#define DEFAULT_OUTPUT stdout
#define DEFAULT_INPUT stdin
#define DEFAULT_ERROR stderr
#define DEFAULT_MEMORY_SIZE 5
#define NUM_OF_ELEMENTS (int)(sizeof(memory)/sizeof(int))
//#define DEBUGGER debug(DEFAULT_OUTPUT, fileIn, memory, testChar, 1);

    

int* allocateResources (FILE *output, FILE *fileIn, int *testChar, int *memorySize);
void debug             (FILE *output, FILE *fileIn, int *memory, int testChar, int display);

int increment          (FILE *output, FILE *fileIn, int *memory, int *testChar);
int decrement          (FILE *output, FILE *fileIn, int *memory, int *testChar);
int outputASCII        (FILE *output, FILE *fileIn, int *memory, int *testChar);
int comment            (FILE *output, FILE *fileIn, int *memory, int *testChar);

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
        while(NEXTCHAR != EOF)
        {
            //DEBUGGER
            // Increment
            increment(DEFAULT_OUTPUT, fileIn, memory, &testChar);
            
            // Decriment
            decrement(DEFAULT_OUTPUT, fileIn, memory, &testChar);
            
            // Output as an ascii character
            outputASCII(DEFAULT_OUTPUT, fileIn, memory, &testChar);
                        
            // Commenting
            comment(DEFAULT_OUTPUT, fileIn, memory, &testChar);
            
            // Getting Input
            if(testChar == '$')
            {
                if(fscanf(DEFAULT_INPUT, "%d", &testChar))
                {
                    //input is an integer
                }
                else
                {
                    //input is a character
                }
            }
            // Terminate
            if(testChar == '.')
            {
                break;
            }
        }
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
            }
            else
            {
                ++memory[*testChar];
            }
        }
        else
        {
            fprintf(DEFAULT_OUTPUT, "Invalid target of incrementation at %ld", ftell(fileIn));
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
            }
            else
            {
                --memory[*testChar];
            }
        }
        else
        {
            fprintf(DEFAULT_OUTPUT, "Invalid target of decrementation at %ld", ftell(fileIn));
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
                        break;
                    }
                }
                else
                {
                   // DEBUGGER
                    *testChar = '.';
                    fprintf(DEFAULT_OUTPUT, "Element after memory " 
    "cell is not an integer at %ld", ftell(fileIn));
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
                        *testChar = '.';
                        fprintf(DEFAULT_OUTPUT, "At character %ld, "
    "memory cell referenced does not exist", ftell(fileIn));
                        break;
                    }
                }
                else
                {
                   // DEBUGGER
                    *testChar = '.';
                    fprintf(DEFAULT_OUTPUT, "Element after memory " 
    "cell is not an integer at %ld", ftell(fileIn));
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
        ?
    
 ************************************************************************/
int condition(FILE *output, FILE *fileIn, int *memory, int *testChar)
{
    int errorLevel = 0,
        firstArgument,
        secondArgument;
    char firstOperator,
         secondOperator;
    
    // Get firstArgument
    if(*testChar == '?')
    {
        if(!(fscanf(fileIn, "%d", testChar)))
        {
            //Treat first as Character
            if(*testChar == '[')
            {
                //First Argument is a valid indicater of a Memory cell
                if(fscanf(fileIn, "%d", testChar))
                {
                    //First Argument is a
                    if(*testChar <= NUM_OF_ELEMENTS || *testChar >= 0)
                    {
                        firstArgument = memory[*testChar];
                    }
                    else
                    {
                        fprintf(output, "\nMemory cell not found at %ld\n", ftell(fileIn));
                    }
                }
                else
                {
                    fprintf(output, "\nInvalid symbol after memory cell indicator at %ld\n", ftell(fileIn));
                }
            }
            else
            {
                //First Argument is not a valid indicator of a Memory cell
                fprintf(output, "\nInvalid argument for conditional if at%ld\n", ftell(fileIn));
            }
        }
        else
        {
            //Treat first argument as constant
            firstArgument = *testChar;
        }
        
        
        // Get firstOperator
        if(fscanf(fileIn, "%d", testChar))
        {
            fprintf(output, "Invalid operator at %ld", ftell(fileIn));
        }
        else
        {
            if(*testChar != '<' && *testChar != '>' && *testChar != '=')
            {
                fprintf(output, "Invalid operator at %ld", ftell(fileIn));
            }
            else
            {
                firstOperator = *testChar;
            }
        }

        // Get secondOperator
        if(fscanf(fileIn, "%d", testChar))
        {
            fprintf(output, "Invalid operator at %ld", ftell(fileIn));
        }
        else
        {
            if(*testChar != '<' && *testChar != '>' && *testChar != '=')
            {
                fprintf(output, "Invalid operator at %ld", ftell(fileIn));
            }
            else
            {
                secondOperator = *testChar;
            }
        }

        if(!(fscanf(fileIn, "%d", testChar))) 
        {
            //Treat first as Character
            if(*testChar == '[')
            {
                //First Argument is a valid indicater of a Memory cell
                if(fscanf(fileIn, "%d", testChar))
                {
                    //First Argument is a
                    if(*testChar <= NUM_OF_ELEMENTS || *testChar >= 0)
                    {
                        secondArgument = memory[*testChar];
                    }
                    else
                    {
                        fprintf(output, "\nMemory cell not found at %ld\n", ftell(fileIn));
                    }
                }
                else
                {
                    fprintf(output, "\nInvalid symbol after memory cell indicator at %ld\n", ftell(fileIn));
                }
            }
            else
            {
                //First Argument is not a valid indicator of a Memory cell
                fprintf(output, "\nInvalid argument for conditional if at%ld\n", ftell(fileIn));
            }
        }
        else
        {
            //Treat first argument as constant
            secondArgument = *testChar;
        }
        
        switch(firstOperator)
        {
            case '<':
            if(firstArgument < secondArgument)
            {
                errorLevel = 1;
            }
            else
            {
                errorLevel = 0;
            }
            break;
            
            case '>':
            if(firstArgument > secondArgument)
            {
                
            }
            break;
            
            case '=':
            if(firstArgument == secondArgument)
            {
                
            }
            break;
            
            default:
            fprintf(output, "Something has gone horribly wrong at %ld", ftell(fileIn));
            break;
        }
    
    
        
    }
    
    return errorLevel;
}
