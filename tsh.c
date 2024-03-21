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
    int list_size = 100;
    int index = 0;

    char** argument_list = malloc(list_size * sizeof(char*));
    char* argument;

    argument = strtok(input_string, " ");
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
    argument_list[index] = NULL;
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
        
        if(strcmp(argument_list[0], "exit")==0)
        {
            exit(0);
        }
        
        //use execlp
        if(exec_type == 0)
        {
            abort();
        }
        //use execvp
        else{
            abort();
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
    } while (!flag);

    printf("\n** Exiting tsh  **\n");
    return 0;
}