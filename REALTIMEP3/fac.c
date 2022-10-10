#include "local.h"
//   ______                               ____  
//  |  ____|             /\       ___    |  _ \ 
//  | |__ __ _  ___     /  \     ( _ )   | |_) |
//  |  __/ _` |/ __|   / /\ \    / _ \/\ |  _ < 
//  | | | (_| | (__   / ____ \  | (_>  < | |_) |
//  |_|  \__,_|\___| /_/    \_\  \___/\/ |____/ 
                                             
                                             


/*
This is Factory A, which will make the first raw 
 material, and transform it to the detergent factory
 using Trucks.
*/


/*
Also this works with Factory B, which will make the second 
raw material, and transform it to the detergent factory 
using Trucks
*/
struct shmseg *shmp;
int main(int argc, char *argv[]) {
int static CA = 10;
int static CB = 12;

sem_t *sem = sem_open(SNAME, 0); /* Open a preexisting semaphore. */
int shmid = atoi(argv[2]);
// Attach to the segment to get a pointer to it.
shmp = shmat(shmid, NULL, 0);
if (shmp == (void *) -1) {
    perror("Shared memory attach");
    return 1;
}
int name = atoi(argv[1]);
//name = 1 for A
//name = 2 for B

//Creating material A
int materialA = 0;
int materialB = 0;
char quantity[10];
while(1) {
    usleep(200000);
    if(name == 1)
        materialA++;//Creating one Material
    if(name == 2)
        materialB++;

    //Checking if there is enough material to send them with a truck
    if(materialA == 10 && name == 1) {//Factory A Code
         //Aquire Lock
        sem_wait(&(shmp->mutex));
        if(shmp->raw_material_A <= 0) {
            shmp->raw_material_A += CA;
             printf(BLU "Truck A is full, moving to detergent factory\n" RESET); 
        }
       
        sem_post(&(shmp->mutex));
        //Releasing Lock
        usleep(20000);
        materialA = 0;
        
    }
    if(materialB == 10 && name == 2) {//Factory B Code
         //Aquire Lock
        sem_wait(&(shmp->mutex));
    
        if(shmp->raw_material_B <= 0 && (shmp->stop == 0)) {
                shmp->raw_material_B += CB;
                 printf(GRN "Truck B is full, moving to detergent factory\n" RESET); 
            
        }
       
        sem_post(&(shmp->mutex));
        //Releasing Lock
        usleep(20000);
        materialB = 0;
    }

}

    
    
    return 0;
}
                            
