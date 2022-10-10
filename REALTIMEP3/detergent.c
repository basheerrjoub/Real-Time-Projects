#include "local.h"
//This factory has to produce 1000 items
struct Item itms[1000];
struct Item Box[1000];
struct Item Shipped[1000];
int Box_Count;
int storage2_Available;
struct Item Storage_2[1000];
sem_t threadSem;
sem_t Box_sem;
sem_t StopLines;
int stopLinesValue;

struct thread_args {
    int first;
    const char *second;
};
struct shmseg *shmp;

int numberOfItemsProduced = 0;

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

void* Line(void *a) {

   //Start Manufacuring steps
   int i = 0;
   while(i<1000){
   //getting the raw material

   if(i > 200)
     kill(getppid(), SIGUSR1);

     
   
   sem_wait(&threadSem);
   i = numberOfItemsProduced;
   numberOfItemsProduced++;
   sem_post(&threadSem);    


   sem_wait(&(shmp->mutex));
   while(shmp->S_1_A <= 0 || shmp->S_1_B <= 0){
    sem_post(&(shmp->mutex));
    usleep(1000);
    sem_wait(&(shmp->mutex));
   }

   
   while(stopLinesValue == 1){
       sem_post(&StopLines);
       sleep(1);
       sem_wait(&StopLines);
   }
   


   shmp->S_1_A -= 2;
   shmp->S_1_B -= 1;
   printf("A: %d, B: %d\n", shmp->S_1_A, shmp->S_1_B);
   sem_post(&(shmp->mutex));
    
   itms[i].id = i;
   Emp1(itms[i]);///////////////
                 //          ///
   Emp2(itms[i]);//          ///
                 //          ///
   Emp3(itms[i]);//          ///
                 //          ///
   Emp4(itms[i]);//          ///
                 //          ///
   Emp5(itms[i]);//          ///
                 //          ///
   Emp6(itms[i]);//          ///
                 //          ///
   Emp7(itms[i]);//          ///
                 //          ///
   Emp8(itms[i]);//          ///
                 //          ///
   Emp9(itms[i]);//          ///
                 ///////////////
                 
   Emp10(itms[i]);
   printf(RED);
   printf("Item %d is ready\n", i);
   printf(RESET);
   storage2_Available++;
   
   }
   
    return NULL;

}
//Employees functions
//Emp1
void Emp1(struct Item itm) {//Cleaning 
    itm.cleaning = 1;
    usleep(randNum(1000,2000));

}
//Emp2
void Emp2(struct Item itm) {//Collecting
    itm.collecting = 1;
    usleep(randNum(1000,2000));

}
//Emp3
void Emp3(struct Item itm) {//Put on line
    itm.put_in_oven = 1;
    usleep(randNum(1000,2000));

}
//Emp4
void Emp4(struct Item itm) {//manufacturing1
    itm.manufacturing1 = 1;
    usleep(randNum(1000,2000));

}
//Emp5
void Emp5(struct Item itm) {//manufacuring2
    itm.manufacturing2 = 1;
    usleep(randNum(1000,2000));

}
//Emp6
void Emp6(struct Item itm) {//manufacturing3
    itm.manufacturing3 = 1;
    usleep(randNum(1000,2000));

}
//Emp7
void Emp7(struct Item itm) {//checking
    itm.checking = 1;
    usleep(randNum(1000,2000));

}
//Emp8
void Emp8(struct Item itm) {//testing
    itm.testing = 1;
    usleep(randNum(1000,2000));

}
//Emp9
void Emp9(struct Item itm) {//putting in cans
    itm.putting_in_cans = 1;
    usleep(randNum(1000,2000));

}
//Emp10
void Emp10(struct Item itm) {//boxing
    itm.boxing = 1;
    itm.ready = 1;
    //Semaphore_For_Box
    sem_wait(&Box_sem);
    Box[Box_Count] = itm;
    Box_Count++;
    sem_post(&Box_sem);
    //Semaphore_For_Box
    usleep(randNum(1000,2000));

}

void error(char *msg)
{
fprintf(stderr, "%s: %s\n", msg, strerror(errno));
exit(1);
}

//This thread will use the raw material
//Comming from Truck A & Truck B
void* L1_Employees(void *a) {

    while(1) {
    //For Truck A
    //Aquire Lock
    sem_wait(&(shmp->mutex));
    //Sensitive Code
    if(shmp->raw_material_A > 0) {
        shmp->raw_material_A -= 10;
        shmp->S_1_A += 10;
        printf("L1_EMP moved material from Truck A to storage S_1\n"); 
    }
    sem_post(&(shmp->mutex));
    //Releasing Lock
       

    //For Truck B
    //Aquire Lock
    sem_wait(&(shmp->mutex));
    //Sensitive Code
    if(shmp->raw_material_B > 0) {
        shmp->raw_material_B -= 12;
        shmp->S_1_B += 12;
        printf("L1_EMP moved material from Truck B to storage S_1\n");

    }
    if((shmp->S_1_B) >= (shmp->S_1_A)*2){
             shmp->stop = 1;
    }else 
            shmp->stop = 0;
    sem_post(&(shmp->mutex));
    //Releasing Lock
  
    }
    return NULL;

}
/*
These Employees are responsible of getting the produced
Items into Storage 2
*/
void* L_3_storage_Emp(void *a) {
    
    int i = 0;
    
    while(i<1000) {
        
        //Sem Aquire
        wait1:
        sem_wait(&Box_sem);
        if((Box_Count - i) < 5){
            sem_post(&Box_sem);
            usleep(10000);
            goto wait1;
        }
        
        char *readyItemsString = malloc(200);
        strcpy(readyItemsString, "Moving to Storage 2: ");
        char numStr[5]; 
        for(int j=i; j<Box_Count; j++){
            Storage_2[j] = Box[j];
            sprintf(numStr, "%d ", Box[j].id);
            strcat(readyItemsString, numStr);
            Box[j].stored = 1;
            i = j+1;   
        }
        sem_post(&Box_sem);
        //Sem Release


        strcat(readyItemsString, "\n");
        printf("%s", readyItemsString);
        free(readyItemsString);
        
        
        usleep(40000);
    }
        
return NULL;
}


void* Det_Trucks_Fun(void *a) {
    int not_Shipped_Boxes = 0;
    int j = 0;
    int counter = 0;
    char *stringready = malloc(200);
    strcpy(stringready, "Shipping: ");
    char num[5];
    while(1){
        
       sem_wait(&Box_sem);
       for(int i=j; i< Box_Count; i++){
           sprintf(num, "%d ", i);
           strcat(stringready, num);
           Shipped[i] = Box[i];
           Shipped[i].shipped = 1;
           counter++;
       }        
       sem_post(&Box_sem);
       j = Box_Count;
       

       usleep(10000);
       if(counter >= 20) {
           printf("Shipping %s\n", stringready);
           printf("Lines have to stop for a while\n");
           sem_wait(&StopLines);
           stopLinesValue = 1;
           sem_post(&StopLines);
           counter = 0;
           strcpy(stringready, "Shipping: ");
           sleep(4);
           sem_wait(&StopLines);
           stopLinesValue = 0;
           sem_post(&StopLines);
       }
       


    }




    return NULL;
}

void* Timer_Fun(void *a){

    sleep(120);//If exceeds two minutes it will terminate
    kill(getppid(), SIGUSR2);

    return NULL;
}

int main(int argc, char *argv[]) {

    //Using the created shared memory
    sem_t *sem = sem_open(SNAME, 0); /* Open a preexisting semaphore. */
    int shmid = atoi(argv[1]);
    // Attach to the segment to get a pointer to it.
    shmp = shmat(shmid, NULL, 0);
    if (shmp == (void *) -1) {
        perror("Shared memory attach");
        return 1;
    }

    stopLinesValue = 0;
    sem_init(&threadSem, 0, 1);
    sem_init(&Box_sem, 0, 1);
    sem_init(&StopLines, 0, 1);
    memset(itms, 0, sizeof(itms));
    memset(Storage_2, 0, sizeof(Storage_2));
    Box_Count = 0;
    storage2_Available = 0;
    struct thread_args *args = malloc (sizeof (struct thread_args));


    //Creating semaphore

    pthread_t LineThd[5];
    //Creating the Lines
    for(int i=0; i<5; i++)
        if (pthread_create(&LineThd[i], NULL, Line, NULL) == -1)
            error("Can't create Line thread");
        
    
    //Creating L1_Employees as a Thread
    pthread_t L1_Employees_Thread;
    if (pthread_create(&L1_Employees_Thread, NULL, L1_Employees, NULL) == -1)
            error("Can't create L1_Employees thread");

    //Creating Storage 2 Employees  as a Thread
    pthread_t Storage_2_Thread;
    if (pthread_create(&Storage_2_Thread, NULL, L_3_storage_Emp, NULL) == -1)
            error("Can't create Storage_2_Thread");

    //Detergent Trucks Thread
    pthread_t Detergent_Trucks_Thread;
    if (pthread_create(&Detergent_Trucks_Thread, NULL, Det_Trucks_Fun, NULL) == -1)
            error("Can't create Detergent_Trucks_Thread");

    //Timer
    pthread_t Timer_Thread;
    if (pthread_create(&Timer_Thread, NULL, Timer_Fun, NULL) == -1)
            error("Can't create Detergent_Trucks_Thread");




    //Thread Join
    //Waiting for all threads to be terminated
    for(int i=0; i<5; i++)
    if (pthread_join(LineThd[i], NULL) == -1)
        error("Line Thread failed to join");
    
    if (pthread_join(L1_Employees_Thread, NULL) == -1)
        error("L1_Emp failed to join!");

    if (pthread_join(Storage_2_Thread, NULL) == -1)
        error("Storage 2 failed to join!");

    if (pthread_join(Detergent_Trucks_Thread, NULL) == -1)
        error("Detergent_Trucks_Thread failed to join!");
    
    if (pthread_join(Timer_Thread, NULL) == -1)
        error("Timer_Thread failed to join!");


    //Destroying semaphore
    sem_destroy(&threadSem);
    sem_destroy(&Box_sem);
    sem_destroy(&StopLines);
    return 0;
}
