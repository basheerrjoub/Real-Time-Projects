////////////////////////////////////////////////////
///Title: Primitive War Simulation              ///
///Std1: Basheer Rjoub   1180291               ///
///Std2: Zahra'a Aqel    1171405              ///
///Std3: Tamara Daghlis  1171324             ///
///////////////////////////////////////////////
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<stdlib.h>
#include<signal.h>
#include<math.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/shm.h>
#include<errno.h>
#include <sys/sem.h>
#define SHM_KEY 0x123456




//Creating a structure for Children which we want to fork
struct Child { 
    pid_t pid;
    int name;
};

struct shmseg {
   int ran1;
   int ran2;
   int A; //Tank A
   int B; //Tank B
   int C; //Tank C
   int D; //Tank D
   pid_t P11;
   pid_t P21;
   pid_t P12;
   pid_t P22;
   pid_t P13;
   pid_t P23;
   pid_t P14;
   pid_t P24;
   pid_t who_annoy_P11;
   pid_t who_annoy_P21;
   int defend_from_p14;
   int defend_from_p24;
   int bag2;
   int bag1;
   int reached1;
   int reached2;
};

//Creating team one and team 2 as array of structure.....
struct Child P[8];

int main(void) {

//    _____ _    _ __  __ ______ __  __  ____  _______     __
//   / ____| |  | |  \/  |  ____|  \/  |/ __ \|  __ \ \   / /
//  | (___ | |__| | \  / | |__  | \  / | |  | | |__) \ \_/ / 
//   \___ \|  __  | |\/| |  __| | |\/| | |  | |  _  / \   /  
//   ____) | |  | | |  | | |____| |  | | |__| | | \ \  | |   
//  |_____/|_|  |_|_|  |_|______|_|  |_|\____/|_|  \_\ |_|   
                                                          
                                                          
//Creating Tanks which are Shared memories...
int shmid, numtimes;
struct shmseg *shmp;
int spaceavailable;


shmid = shmget(SHM_KEY, sizeof(struct shmseg), 0644|IPC_CREAT);
if (shmid == -1) {
    perror("Shared memory");
    return 1;
}
// Attach to the segment to get a pointer to it.
shmp = shmat(shmid, NULL, 0);
if (shmp == (void *) -1) {
    perror("Shared memory attach");
    return 1;
}
//First Initialize the tanks
//Here we assume there are 1000 Cup at the filled tank
shmp->A = 1000; //Filled tank A
shmp->B = 0;   //Empty tank B
shmp->C = 1000;//Filled tank C
shmp->D = 0;  //Empty tank D
shmp->reached1 = 0;
shmp->reached2 = 0;


system("clear");
system("gcc -o P2 P2.c");
system("gcc -o P1 P1.c");
char PName[5];
pid_t pid = getpid();



//Creating P1 team
for(int i=1; i<5; i++) {
    if(pid>0) {
        pid = fork();
        if(pid>0) {
            usleep(100000);
            printf("creating: %d\n", i+10);
            P[i-1].name = 10+i;
            sprintf(PName,"%d",P[i-1].name);
            P[i-1].pid = pid;                     
        }
    }
        
}
if(pid>0){//Parent
    shmp->P11 = P[0].pid;/////////////////////////////////////////////
    shmp->P12 = P[1].pid;
    shmp->P13 = P[2].pid;
    shmp->P14 = P[3].pid;
    
}
//Executing P1 team
if(pid == 0) { //Child
    sleep(1);
    const char* args[] = {"P1",PName, "team1", NULL};
    int ret;
    ret = execv("./P1", args);
    if(ret == -1) {
        perror("Error in Execv!");
        exit(EXIT_FAILURE);
    }
    return 0;
}

//Creating P2 team
for(int i=1; i<5; i++) {
        if(pid>0) {
        pid = fork();
        if(pid>0) {
            usleep(100000);
            printf("creating: %d\n", i+20);
            P[i+3].name = i+20;
            sprintf(PName,"%d",P[i+3].name);
            P[i+3].pid = pid;              
        }
        }
        
}

if(pid>0){//Parent
    shmp->P21 = P[4].pid;/////////////////////////////////////////////
    shmp->P22 = P[5].pid;
    shmp->P23 = P[6].pid;
    shmp->P24 = P[7].pid;
    
}

//Executing P2 team
if(pid == 0) { //Child    //arg0  arg1   arg2  
    sleep(1);
    const char* args[] = {"P2", PName,"team2", NULL};
    int ret;
    ret = execv("./P2", args);
    if(ret == -1) {
        perror("Error in Execv!");
        exit(EXIT_FAILURE);
    }
    return 0;
}


pid_t wpid;
int status = 0;
while ((wpid = wait(&status)) > 0);
    


return 0;
}
