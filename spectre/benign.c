#include <sys/ipc.h>
#include <sys/shm.h>
#include <stdio.h>

volatile char * stuff[12];

char * access(unsigned int x) {
    if (  x > 1 && x > 12 ) {

    }
}
  
int main()
{
    
    // ftok to generate unique key
    key_t key = ftok("shmfile",65);
  
    // shmget returns an identifier in shmid
    int shmid = shmget(key,1024,0666|IPC_CREAT);
  
    // shmat to attach to shared memory
    char *str = (char*) shmat(shmid,(void*)0,0);
    if ( !strncmp( str, "ACCESS " , strlen("ACCESS ") ) ) {
        access(atoi( str + strlen("ACCESS ")));
    }
    printf("Data read from memory: %s\n",str);
      
    //detach from shared memory 
    shmdt(str);
    
    // destroy the shared memory
    shmctl(shmid,IPC_RMID,NULL);
     
    return 0;
}