//  ____     _ __  _ __ ___   ___ ___  ___ ___ 
// |  _ \   | '_ \| '__/ _ \ / __/ _ \/ __/ __|
// | |_) |  | |_) | | | (_) | (_|  __/\__ \__ \
// | .__/   | .__/|_|  \___/ \___\___||___/___/
// |_|      |_|                                
//

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


int bag = 50;
int annoyTime = 1;
//Creating a structure for Children which we want to fork
struct Child { 
    pid_t pid;
    int name;
    char team[5];
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
struct Child child;
struct shmseg *shmp;

void handle_sigusr2(int sig) {
if(child.name == 23 || child.name == 22) {
   if(shmp->defend_from_p14 == 0) {
    if(child.name == 22){//If this proccess is stealing and P14 is preventing it
        shmp->defend_from_p14 = 1;
        kill(shmp->P23, SIGUSR2);// P23 defend the stealter P22 from P14
        usleep(1000);
    }
    if(child.name == 23){//If this proccess is stealing and P14 is preventing it
        shmp->defend_from_p14 = 1;
        kill(shmp->P22, SIGUSR2);// P23 defend the stealter P22 from P14
        usleep(1000);
    }
   } else if(shmp->defend_from_p14 == 1) {
       int ran1; //Ran1 due to P14
       shmp->ran1 = randNum(1, 2);
       ran1 = shmp->ran1;
       if(ran1 == 1) {
           shmp->bag1 -= 10;//P14 Fall and stealers take a cup = 10 drops of water
       }
       else if(ran1 == 2) {
           shmp->bag1 = 50;//P22 or P23 Fail
       }
   }
}

}
void handle_sigusr1(int sig) {
    if(child.name == 21)
        kill(shmp->P24, 2); //Call P14 to prevent the stealing
}

void handle_sigint(int sig) {
  int ran2;
  if(sig == 2)
    if(child.name == 24){ //If process is P14 and not fall @todo
        shmp->defend_from_p24 = 0;
        kill(shmp->who_annoy_P21, SIGUSR2);
        usleep(100000);
        ran2 = shmp->ran2;
        if(ran2 == 1){
            printf("P24 fell down");
            shmp->ran2 = 0;
        }else if(ran2 == 2) {
            printf("P24 defended from stealing :)");
        }
    }
}

int randNum(int lower, int upper)
{
    int s, numb;
    FILE *f;
    f = fopen("/dev/urandom", "rb");
    fread(&s, sizeof(int), 1, f);
    fclose(f);
    srand(s);
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;

}
void test() {
    printf("Child process\n");
    exit(0);
}


int main(int argc, char *argv[]) {
//    _____ _    _ __  __ ______ __  __  ____  _______     __
//   / ____| |  | |  \/  |  ____|  \/  |/ __ \|  __ \ \   / /
//  | (___ | |__| | \  / | |__  | \  / | |  | | |__) \ \_/ / 
//   \___ \|  __  | |\/| |  __| | |\/| | |  | |  _  / \   /  
//   ____) | |  | | |  | | |____| |  | | |__| | | \ \  | |   
//  |_____/|_|  |_|_|  |_|______|_|  |_|\____/|_|  \_\ |_|   
                                                          

//Creating Tanks which are Shared memories...
int shmid;
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



char Name[5];
strcpy(Name, argv[1]);
child.name = atoi(Name);
strcpy(child.team, argv[2]);
usleep(randNum(1000, 10000));
if(child.name == 21) child.pid = shmp->P21;
if(child.name == 22) child.pid = shmp->P22;
if(child.name == 23) child.pid = shmp->P23;
if(child.name == 24) child.pid = shmp->P24;

int distance = 0;
int bag2;
// pause();
//@todo Signal to overcome the pause
while(1){
//Team 2
if(!strcmp(child.team, "team2")) {
    if(child.name == 21) {
        printf("P21 Got Water from Tank: C");
        shmp->bag2 = 50;
        bag2 = shmp->bag2;
        (shmp->C) -= bag2;
        distance++;
        //Decrementing the bag now.... @todo
        signal(SIGUSR1, &handle_sigusr1);
        pause();
        usleep(1000000);
        bag2 = shmp->bag2;
        (shmp->D) += bag2;
        if(shmp-> C <= 0) {
            printf("C now is empty!!");
            system("pkill P2");
            system("pkill P1");
            shmdt(shmp);
            exit(0);

        }
        //Accidently drop the water bag @todo
        
    }else if(child.name == 22 || child.name == 23) { //If processes !P21 and !P24
        //This portion for P22 & P23 to annoy P11...
        //@todo Sand will slow them, so increase time to annoy
        sleep(randNum(2,3));//@todo Sleep with random number
        if(shmp->reached2 == 0){
            shmp->reached2 = 1;
            printf("Annoying P11");
            if(child.name == 22)
                shmp->who_annoy_P11 = getpid();//@todo one process annoy at the same time...
            if(child.name == 23)
                shmp->who_annoy_P11 = getpid();
            kill(shmp->P11, SIGUSR1); 
        }
        signal(SIGUSR2, &handle_sigusr2);
        pause();
        shmp->reached2 = 0;
        
    }else if(child.name == 24) {// P14 to help P11 by putting sand on P22 P23 bags
        //Random Number...
        printf("Defending P21");
        signal(SIGINT, &handle_sigint);
        pause();
    }


}
if(strcmp(child.team, "team2")){
    printf("Team Error");
    exit(0);
}
}
exit(0);
}