////////////////////////////////////////////////////
///Title: Primitive War Simulation              ///
///Std1: Basheer Rjoub   1180291               ///
///Std2: Zahra'a Aqel    1171405              ///
///Std3: Tamara Daghlis  1171324             ///
///////////////////////////////////////////////
#ifndef __LOCAL_H_
#define __LOCAL_H_

/*
 * Common header file: parent, producer and consumer
 */

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
#include<pthread.h>
#include<sys/sem.h>
#include<semaphore.h>
#define RED   "\x1B[31m"
#define GRN   "\x1B[32m"
#define YEL   "\x1B[33m"
#define BLU   "\x1B[34m"
#define MAG   "\x1B[35m"
#define CYN   "\x1B[36m"
#define WHT   "\x1B[37m"
#define RESET "\x1B[0m"
#define SHM_KEY IPC_PRIVATE
#define SNAME "/mysem"
struct Item {
int id;
int cleaning;//Emp1
int collecting;//Emp2
int put_in_oven;//Emp3
int manufacturing1;//Emp4
int manufacturing2;//Emp5
int manufacturing3;//Emp6
int checking;//Emp7
int testing;//Emp8
int putting_in_cans; //Emp9
int boxing; //Emp10

int ready; //If Item is ready after all previous manufacturing steps
//Being done right.
int stored;
int shipped;

};

struct shmseg {
   int raw_material_A;
   int raw_material_B;
   int S_1_A;
   int S_1_B;
   sem_t mutex;
   int stop;   
};




#endif