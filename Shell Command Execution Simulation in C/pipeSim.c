// Melisa Yilmaz 26777
// Date: 22.10.2021
// Programming Assignment 1
// Shell Command Execution Simulation in C
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/wait.h>
#include <sys/fcntl.h>


int main(int argc, char *argv[]) {
    
    int fd[2];
    printf("I'm SHELL process with PID: %d - Main command is: <man grep | grep -A 1 \\-U> \n",getpid());
    
    if (pipe(fd) < 0)
    {
        printf("The pipe is failed\n");
        _exit(0);
    }
    
    int id1 = fork();
    if (id1 < 0)
    {
        printf("The fork 1 is failed %d", (int) getpid());
        _exit(0);
        
    }
    
    else if ( id1 == 0)
    {
        //CHILD PROCESS (MAN)
        printf("I'm MAN process with PID: %d - My command is: <man grep> \n",(int) getpid());
        
        dup2(fd[1],STDOUT_FILENO); // make output go to the pipe
        close(fd[0]);
        close(fd[1]);
        
        char* manCommand = "man";
        char* argument_list[] = {"man", "grep", NULL};
        execvp(manCommand, argument_list);
    }
    
    int id2 = fork();
    if (id2 < 0)
    {
        printf("The fork 2 is failed %d", (int) getpid());
        _exit(0);
    }
    
    else if (id2 == 0)
    {
        //CHILD PROCESS  (GREP)
        printf("I'm GREP process with PID: %d - My command is: <grep -A 1 \\-U>\n",(int) getpid());
        dup2(fd[0],STDIN_FILENO); // get input from the pipe
        close(fd[1]);
        close(fd[0]);
        
        int new_fd = open("output.txt", O_WRONLY | O_CREAT, S_IRUSR | S_IWUSR);
        if (dup2(new_fd,STDOUT_FILENO) < 0)
        {
            perror("dup2");
            _exit(1);
        }

        char* command2 = "grep";
        char* argument_list2[] = {"grep", "-A", "1","\\-U",NULL};
        execvp(command2, argument_list2);
        
        
    }
    close(fd[0]);
    close(fd[1]);
    waitpid(id1,NULL,0);
    waitpid(id2,NULL,0);

    printf("I’m SHELL process, with PID: %d - execution is completed, you can find the results in output.txt\n", (int) getpid() );
   
    
    return 0;
}
