/* 
Madhav Bist 
1001731179
*/

#define _GNU_SOURCE

#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

#define WHITESPACE " \t\n" // We want to split our command line up into tokens 
                           // so we need to define what delimits our tokens.   
                           // In this case  white space                        
                           // will separate the tokens on our command line

#define MAX_COMMAND_SIZE 255 // The maximum command-line size

#define MAX_NUM_ARGUMENTS 11 // Mav shell only supports 10 +1(itself) arguments
//Display the all the command entered by use Max dis 15.
//paramater: char double array and int.
void history_dis_function(char history_array[15][255], int hist_index)
{
    int i = 0;

    for (i = 0; i < hist_index; i++)
    {
        printf("%s", history_array[i]);
    }
}

int main()
{
    char history_array[15][255] = {{0}};                        // Assigning null value to 2D char array
    char *cmd_str = (char *)malloc(MAX_COMMAND_SIZE);           //Assigning memory to store command line
    char *cmd_str_collector = (char *)malloc(MAX_COMMAND_SIZE); //Assigning space which holds cmd_str lines
    int hist_index = 0;                                         // Counts the number of command
    int showpid_array[15];                                      //Stores the max pid of 15
    int index;                                                  // Count number of pids
    while (1)
    {
        // Print out the msh prompt
        printf("msh> ");

        // Read the command from the commandline.  The
        // maximum command that will be read is MAX_COMMAND_SIZE
        // This while command will wait here until the user
        // inputs something since fgets returns NULL when there
        // is no input
        while (!fgets(cmd_str, MAX_COMMAND_SIZE, stdin))
            ;
        if (cmd_str[0] == '\n')
            continue; // Built in function
        cmd_str_collector = cmd_str;
        strcpy(history_array[hist_index], cmd_str_collector);
        hist_index++;
        //Assigning 0 to any counter if it reach max of 15
        if (hist_index == 15)
        {
            hist_index = 0;
        }

        /* Parse input */
        char *token[MAX_NUM_ARGUMENTS];

        int token_count = 0;

        // Pointer to point to the token
        // parsed by strsep
        char *argument_ptr;

        char *working_str = strdup(cmd_str);

        // we are going to move the working_str pointer so
        // keep track of its original value so we can deallocate
        // the correct amount at the end
        char *working_root = working_str;

        // Tokenize the input stringswith whitespace used as the delimiter
        while (((argument_ptr = strsep(&working_str, WHITESPACE)) != NULL) &&
               (token_count < MAX_NUM_ARGUMENTS))
        {
            token[token_count] = strndup(argument_ptr, MAX_COMMAND_SIZE);
            if (strlen(token[token_count]) == 0)
            {
                token[token_count] = NULL;
            }
            token_count++;
        }

        // Now print the tokenized input as a debug check
        // \TODO Remove this code and replace with your shell functionality

        //Exits function if exit or quit prompt is entered
        if ((strcmp(token[0], "exit") == 0) || strcmp(token[0], "quit") == 0)
        {
            exit(0);
        }
        //Display all the command typed by user max diplay 15
        else if (strcmp(token[0], "history") == 0)
        {
            history_dis_function(history_array, hist_index);
        }
        // Helps execute cd command
        else if (strcmp(token[0], "cd") == 0)
        {
            chdir(token[1]);
        }
        // Prints the showpids max of 15
        else if (strcmp(token[0], "showpids") == 0)
        {
            for (int i = 0; i < index; i++)
            {
                printf("%d\n", showpid_array[i]);
            }
        }
        // Create fork and helps store the pids value
        else
        {
            pid_t pid = fork();

            if (pid == 0)
            {
                int pid1 = execvp(token[0], &token[0]);
                if (pid1 == -1)
                {
                    printf("%s command not found\n", token[0]);
                }
            }
            else if (pid < 0)
            {
                printf("process not created");
            }
            else
            {
                showpid_array[index] = pid;
                index++;
                if (index > 14)
                {
                    index = 0;
                }
                int status;
                wait(&status);
            }
        }

        free(working_root);
    }
    return 0;
}