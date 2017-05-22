/* Includes */
#include <unistd.h>     /* Symbolic Constants */
#include <sys/types.h>  /* Primitive System Data Types */
#include <errno.h>      /* Errors */
#include <stdio.h>      /* Input/Output */
#include <stdlib.h>     /* General Utilities */
#include <pthread.h>    /* POSIX Threads */
#include <string.h>     /* String handling */
#include <stdio.h>

void* print_message ( void *threadid )
{
    printf("\tsecond thread here, my tid is %u but I'm still in process %d\n\n",(unsigned int)pthread_self(),getpid());
}

int main()
{
    pthread_t thread;
    printf("main thread here; pid is %d, tid is %u\n\n",getpid(), (unsigned int)pthread_self());
    if(pthread_create(&thread, NULL, print_message, NULL))
    {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }
    printf("main: successfully created a new thread with TID of %u\n\n", (unsigned int)thread);
    if(pthread_join(thread, NULL)) {
        fprintf(stderr, "Error joining thread\n");
        return 2;
    }
    printf("main: second thread has terminated, main will now exit.\n\nŝ");
    pthread_exit(0);
}