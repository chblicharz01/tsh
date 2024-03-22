/*
Chris Blicharz
tsh.c
IT383 - 3/14/24
*/
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/wait.h>
#define COMMAND_LENGTH 100
#define MAX_HISTORY_SIZE 50

//history setup 
    char command_history [MAX_HISTORY_SIZE][COMMAND_LENGTH];
    int history_index = 0;

/*

void history()
takes no parameters
prints the history of previously ran commands
*/
void history()
{
    for (int i = 0; i < history_index; i++) {
        printf("%d: %s\n", (i+1),command_history[i]);
    }
}

/*
int run()
takes parameters int exec_type and a character string 
forks the process, and then uses a formatted string of arguments to call a command
*/
int run(int exec_type, char* input_string){
    //setup the initial size and index of the argument list
    int list_size = 100;
    int index = 0;

    //allocate resources to the argument list and the command
    char** argument_list = malloc(list_size * sizeof(char*));
    char* command;
    char* argument;

    //parse the input string into a command and argument list
    command = strtok(input_string, " ");
    argument = strtok(NULL, " ");
    while (argument != NULL)
    {
        argument_list[index] = argument;
        index++;
        if (index >= list_size)
        {
            list_size+= 100;
            argument_list = realloc(argument_list, list_size * sizeof(char*));
        }

        argument = strtok(NULL, " ");
    }
    //last element needs to be null
    argument_list[index] = NULL;

    //fork the process
    pid_t pid;
    pid = fork();
    
    //parent process
    if(pid!= 0){
        int status;
        if(waitpid(pid, &status, 0)==-1)
        {
            perror("Error in parent process.");
            exit(0);
        }
        

    }

    //child process that runs command
    else if (pid ==0){
        //exit if the command is exit
        if(strcmp(command, "exit")==0){
            return 1;
        }
        if (strcmp(command, "history")==0)
            {
                history();
            }
        //use execlp
        if(exec_type == 0){
            execlp(command, argument_list[0], argument_list[1], argument_list[2], argument_list[3], argument_list[4], NULL);
            perror("Error executing command");
            abort();
        }
        //use execvp

        else{
            execvp(command, argument_list);
            perror("Error executing command");
            abort();
        }
    }
    else {
        perror("Fork failed.");
        return 2;
    }
    return 0;
}

//main
int main(int argc, char* argv[]){
    //initial usage check
    if(argc > 2){
        perror("Usage: ./tsh [optional exec call] ");
        return 1;
    }

    //shell setup
    int exec_type = 0;
    if(argc == 2) {
        if (strcmp(argv[1], "-execvp")==0){
            exec_type = 1;
            printf("**Based on your choice, execvp() will be used**");
        }
        else if (strcmp(argv[1],"-execlp") ==0){
            printf("**Based on your choice, execlp() will be used**");
        }
        else {
            printf("Invalid exec type. execlp will be used by default.");
        }

    }
    else {
        printf("**By default, execlp() will be used**");
    }

    //input setup
    char* input_string = NULL;
    size_t size = 0;
    ssize_t retval;
    
    int flag = 0;
    do{
        printf("\ntsh >\t");
        retval = getline(&input_string, &size, stdin);
        input_string[strcspn(input_string, "\r\n")] = 0;
        flag = run(exec_type, input_string);
        if (history_index < MAX_HISTORY_SIZE) {
            strncpy(command_history[history_index], input_string, COMMAND_LENGTH - 1);
            command_history[history_index][COMMAND_LENGTH - 1] = '\0'; // Ensure null-termination
            history_index++;
        } 
        else {
            history_index = 0;
            strncpy(command_history[history_index], input_string, COMMAND_LENGTH - 1);
            command_history[history_index][COMMAND_LENGTH - 1] = '\0'; // Ensure null-termination
            history_index++;
        }
    } while (!flag);

    printf("\n** Exiting tsh  **\n");
    return 0;
}