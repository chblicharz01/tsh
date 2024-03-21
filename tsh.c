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


/*
int run()
takes parameters int exec_type and a character string 
forks the process, and then uses a formatted string of arguments to call a command
*/
int run(int exec_type, char* input_string){
    printf("Entered command: %s", input_string);
    
    //build argument list
    char* string_copy = input_string;
    char* command = strtok(string_copy, " ");
    char* argument = strtok(NULL, " ");
    int length = 0;
    int index = 0;
    while(argument!=NULL)
    {
        length++;
        argument = strtok(NULL, " ");
    }

    printf("Argument list length: %d", length);
    char* argument_list[length];
    command = strtok(input_string, " ");
    argument = strtok(NULL, " ");
    while(argument!= NULL)
    {
        argument_list[index] = argument;
        index++;
        argument = strtok(NULL, " ");
    }

    for(int i = 0; i < length; i++)
    {
        printf("\nElement %d: %s", i, argument_list[i]);
    }
    
    
    /*
    char* argument_list[100];
    int index = 0;
    char* command = strtok(input_string, " ");
    char* argument = strtok(NULL, " ");
    while (argument!= NULL && index != 10)
    {
        argument_list[index] = argument;
        index++;
        argument= strtok(NULL, " ");
    }
    for (int i = 0; i < 3; i++)
    {
        printf("\nElement %d: %s", i+1, argument_list[i]);
    }
    */


    //fork the process
    pid_t pid;
    pid = fork();
    
    //parent process
    if(pid!= 0){
        wait(NULL);
        return pid;
    }
    
    //child process that runs command
    else if (pid ==0){
        //use execlp
        if(exec_type == 0)
        {
            
        }
        //use execvp
        else{

        }
    }
    else {
        perror("Fork failed.");
        return 2;
    }
    return 0;
}


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
        else {
            printf("**Based on your choice, execlp() will be used**");
        }
    }
    else {
        printf("**By default, execlp() will be used**");
    }


    //input setup
    char* input_string = NULL;
    size_t size = 0;
    ssize_t raw_chars;
    
    int flag = 0;
    do{
        printf("\ntsh >\t");
        raw_chars = getline(&input_string, &size, stdin);
        flag = run(exec_type, input_string);
        printf("The return code: %d",flag);
    } while (!flag);

    printf("\n** Hello world from the tsh! **\n");
    return 0;
}