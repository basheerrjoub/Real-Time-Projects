#include "local.h"
#include<semaphore.h>

void handle_sigusr1(int handle_sigusr1){
    
    system("pkill detergent");
    system("pkill fac");
    printf(CYN "The detergent Factory Exceeds 200 item\n" RESET);
    printf("Terminating");
    for(int i=0; i<5; i++){
        printf(".");
        usleep(700000);
    
    }
    printf("\n");
    exit(0);
    
}

void handle_sigusr2(int handle_sigusr2){
    
    system("pkill detergent");
    system("pkill fac");
    printf(CYN "The detergent Factory Exceeds Working Time\n" RESET);
    printf("Terminating");
    for(int i=0; i<5; i++){
        printf(".");
        usleep(700000);
    
    }
    printf("\n");
    exit(0);
    
}

int main(void) {
                                                          
//Creating Tanks which are Shared memories...
int shmid, numtimes;
struct shmseg *shmp;



shmid = shmget(SHM_KEY, sizeof(struct shmseg), 0666|IPC_CREAT);
if (shmid == -1) {
    perror("Shared memory");
    return 1;
}
char strShmid[15];
sprintf(strShmid, "%d", shmid);
// Attach to the segment to get a pointer to it.
shmp = shmat(shmid, NULL, 0);
if (shmp == (void *) -1) {
    perror("Shared memory attach");
    return 1;
}
sem_init(&(shmp->mutex), 1, 1);
shmp->raw_material_A = 0;
shmp->raw_material_B = 0;
shmp->S_1_A = 0;
shmp->S_1_B = 0;
shmp->stop = 0;
system("clear");
pid_t pid = getpid();


//Create detergent factory as a process
pid = fork();
        
if(pid>0){//Parent
    
}
//Executing detergent
if(pid == 0) { //Child
    sleep(1);
    const char* args[] = {"detergent", strShmid, NULL};
    int ret;
    ret = execv("./detergent", args);
    if(ret == -1) {
        perror("Error in Execv!");
        exit(EXIT_FAILURE);
    }
    return 0;
}
//Create Factory A  as a process
pid = fork();
        
if(pid>0){//Parent
    
}
//Executing Factory
if(pid == 0) { //Child 
    sleep(1);
    const char* args[] = {"fac", "1", strShmid,  NULL};
    int ret;
    ret = execv("./fac", args);
    if(ret == -1) {
        perror("Error in Execv!");
        exit(EXIT_FAILURE);
    }
    return 0;
}

//Create Factory B  as a process
pid = fork();
        
if(pid>0){//Parent
    
}
//Executing Factory
if(pid == 0) { //Child
    sleep(1);
    const char* args[] = {"fac","2",strShmid,  NULL};
    int ret;
    ret = execv("./fac", args);
    if(ret == -1) {
        perror("Error in Execv!");
        exit(EXIT_FAILURE);
    }
    return 0;
}

pid_t wpid;
int status = 0;
signal(SIGUSR1, &handle_sigusr1);   
signal(SIGUSR2, &handle_sigusr2); 
while ((wpid = wait(&status)) > 0);
sem_destroy(&(shmp->mutex));
    


return 0;
}
