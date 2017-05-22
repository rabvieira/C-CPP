/* This program forks and and the prints whether the process is
 *   - the child (the return value of fork() is 0), or
 *   - the parent (the return value of fork() is not zero)
 *
 * When this was run 100 times on the computer the author is
 * on, only twice did the parent process execute before the
 * child process executed.
 *
 * Note, if you juxtapose two strings, the compiler automatically
 * concatenates the two, e.g., "Hello " "world!"
 */
//------------------------------------------------------------------------------------
/*
#include <stdio.h>
int main( void ) {
	int pid = fork();

	if ( pid == 0 ) {
		printf( "This is being printed from the child process\n" );
	} else {
		printf( "This is being printed in the parent process:\n"
		        " - the process identifier (pid) of the child is %d\n", pid );
	}

	return 0;
}
*/
//------------------------------------------------------------------------------------
/*
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
int main()
{
    pid_t pid;
    // fork a child process
    pid = fork();
    if (pid < 0) { // error occurred
        fprintf(stderr, "Fork Failed");
        return 1;
    }
    else if (pid == 0) { // child process
        execlp("/bin/ls","ls",NULL);
    }
    else { // parent process
    // parent will wait for the child to complete
        wait(NULL);
        printf("Child Complete");
    }
    return 0;
}
*/
//------------------------------------------------------------------------------------
/*
int main( void )
{
    int pid, i;
    for(i=0 ; i<2 ; i++)
    {
        pid = fork();
        if(pid)
            printf("I am child #%d, my pid is %d; my parent's pid is %d\n",i,getpid(),getppid());
        else
            printf("Parent, my pid is %d\n",getpid());
    }
    return 0;
}
*/
//------------------------------------------------------------------------------------

/*-----------------------------------------------------------------------------
 Name      : process_code.c
 Date	   : September 15, 2014
 Class     : C420, Operating Systems
 Author    : Renan A. B. Vieira
 Op. Syst. : Developed in Code::Blocks / Linux Ubuntu
 Descript. : This program creates one father process with three child process;
             The second child executes the execv command
-----------------------------------------------------------------------------
*/
#include <sys/types.h>
#include <stdio.h>
#include <unistd.h>
#define child 3  //number of child
int main()
{
    pid_t pid=1;
    int i;
    char *temp[] = {NULL};  //args for execv command
    for(i=1 ; i<child+1 ; i++)
    {
        if(pid>0)                           //execute at least once (pid=1) and avoid child call more child
            pid = fork();
        if(pid>0 && (pid-i == getpid()))    //if it is the parent process
        {
            printf("Parent, my pid is %d: Spawned child #%d whose pid is %d\n",getpid(),i,pid);
        }
        else if(!pid)
        {
            printf("\tI am child #%d, my pid is %d; my parent's pid is %d\n",i,getpid(),getppid());
            if(i==2)
                execv("child2Replacement", temp);
            break;                          //just one chid for loop
        }
    }
    return 0;
}

















