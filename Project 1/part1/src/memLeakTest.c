// Include Statements
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>
#include <sys/wait.h>

// Function Prototypes
void noMemLeak(void);
void MemLeakInt(void);
void MemLeakStr(void);
void badFree(void);
void exitB4Free(void);
void childLeak(void);

/*
* Description: This program is used to test tools like gdb and valgrind to detect memory leaks.
* The user can choose to run 1 out of 5 test functions, each of which containing a different kind
* of memory leak. There is also a function with no memory leak, which can be used as a reference.
*
* Syntax: memLeakTest <test#>
* <test#> : integer between 1 and 6 (inclusive)
*     1 --> Allocates and frees an integer correctly
*     2 --> Allocates an integer, but doesn't free it (size = 4 bytes)
*     3 --> Allocates a string, but doesn't free it (size = 12 bytes)
*     4 --> Attempts to free an unallocated pointer
*     5 --> Allocates 10 bytes, but terminates before freeing (Hit Ctrl+DC when input is requested)
*     6 --> Child process allocates 10 bytes and exits without freeing. Parent reaps child.
*
* Assumptions: <test#> argument is an integer
*/
int main(int argc, char* argv[]){
    int testID; // Variable to hold test #

    // Check number of arguments
    if(argc != 2){
      // Print Usage Message
      printf("Invalid Arguments!\n");
      printf("  Usage:\n");
      printf("  memLeakTest <test#>\n");
      printf("  #1 :  Freed Correctly\n");
      printf("  #2 :  Integer Leak\n");
      printf("  #3 :  String Leak --> \"Hello World\"\n");
      printf("  #4 :  Free without Malloc\n");
      printf("  #5 :  Exit before free while scanning\n");
      printf("  #6 :  Leak in Child\n");
      return -1; // Return -1 on error
    }

    testID = atoi(argv[1]); // Get test # from argv

    // Switch on test # and call appropriate test function
    switch(testID){
        case 1:
            noMemLeak(); // Call test function #1
            break;
        case 2:
            MemLeakInt(); // Call test function #2
            break;
        case 3:
            MemLeakStr(); // Call test function #3
            break;
        case 4:
            badFree(); // Call test function #4
            break;
        case 5:
            exitB4Free(); // Call test function #5
            break;
        case 6:
            childLeak(); // Call test function #6
            break;
        default: // Any other integer is invalid..
            // Print Error Messge
            printf("%d is an invalid test number\n", test);
            printf("Valid tests are 1-6\n");
            return -1; // Return -1 on error
    }

    return 0; // Return 0 on success
}

// Allocates and frees an integer correctly
void noMemLeak(void){
    int *y = malloc(sizeof(int)); // Allocate an integer
    free(y); // Free the allocated memory
}

// Allocates an integer, but doesn't free it (size = 4 bytes)
void MemLeakInt(void){
  int *y = malloc(sizeof(int)); // Allocate an integer
}

// Allocates a string, but doesn't free it (size = 12 bytes)
void MemLeakStr(void){
    int *y = malloc(sizeof(char)*(strlen("Hello World")+1)); // Allocate a string of 12 characters
}

// Attempts to free an unallocated pointer
void badFree(void){
    int *y; // Unallocated pointer
    free(y); // Free the unallocated memory
}

// Allocates 10 bytes, but terminates before freeing
void exitB4Free(void){
    char *str = malloc(10*sizeof(char)); // Alocating 10 bytes
    scanf("Hit Ctrl+C/Ctrl+D to terminate %s",str); // Wait for user to terminate program with Ctrl-C
    free(str); // Free not done because program terminated prematurely
}

// Child process allocates 10 bytes and exits without freeing. Parent reaps child.
void childLeak(void){
    if(fork()==0){ // Fork a child process
        char *str = malloc(10*sizeof(char)); // Allocate 10 bytes
        exit(0); // Exit without freeing
    }
    wait(NULL); // Parent reaps child process
}
