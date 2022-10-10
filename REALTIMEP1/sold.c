//  _____       _     _      ______                            
// /  ___|     | |   | |     | ___ \                           
// \ `--.  ___ | | __| |     | |_/ / __ ___   ___ ___  ___ ___ 
//  `--. \/ _ \| |/ _` |     |  __/ '__/ _ \ / __/ _ \/ __/ __|
// /\__/ / (_) | | (_| |     | |  | | | (_) | (_|  __/\__ \__ \
// \____/ \___/|_|\__,_|     \_|  |_|  \___/ \___\___||___/___/
                                                                                                                                                                             
                                                                   
#include<stdio.h>
#include<string.h>
#include<unistd.h>
#include<signal.h>
#include<stdlib.h>
#include<sys/types.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <math.h>
typedef enum {false, true} bool;
struct Child { 
    pid_t pid;
    char type[3];
    int x;
    int y;   
};

double injured = 0;// 0 means doesn't got shot yet, every bullet increments this.

double fordistanceprob = 0;
pid_t closest(struct Child enem[], struct Child me) {
    pid_t enemPid = 0;
    double distance = 200000;
    double dist=0;
    for(int i=0; i<5; i++) {
        dist = sqrt((double)(((double)me.x - (double)enem[i].x)*((double)me.x - (double)enem[i].x))+ (double)(((double)me.y - (double)enem[i].y)*((double)me.y - (double)enem[i].y)));
        if(0 == kill(enem[i].pid, 0)) {
            if(dist < distance)
            {
                enemPid = enem[i].pid;
                distance = dist;
                fordistanceprob = dist;
            }                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   
        }     

    }
    return enemPid;
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

void recharge() {
    double chargeTime;
    chargeTime = (double)randNum(10, 20);
    chargeTime = chargeTime / 20;
    chargeTime = chargeTime*1000000;
    int usec = chargeTime;
    usleep(usec);//Recharching bullets by sleeping for a second 
}


int bulletLocation() {
    return randNum(0, 4);
}                                                                                                                                                                                       

int shoot() { //Calculate probability if the enemy got shot....
    bool gotShot = false;
    double maxdistance = 5000;
    double ran = (double)randNum(0, 100);
    double prob = ran / 100;
    double distEffect = 1 - fordistanceprob/maxdistance;
    distEffect = distEffect * 0.2;
    double focus = injured / 7;
    focus = focus * 0.3;
    prob += distEffect;
    prob -= focus;
    if(prob > 0.5)
        gotShot = true;
    return gotShot;
}

struct Child me;
int head = 2;
int neck = 3;
int chest = 3;
int abdomen = 3;
int limbs = 5;


//Signal Handler
void handle_sigusr1(int sig) {
    // system("clear");
    injured++;
    
    int loc = bulletLocation();
    switch (loc){
    case 0: head--; printf("%d Got Shot in Head\n", getpid()); break;
    case 1: neck--; printf("%d Got Shot in Neck\n", getpid()); break;
    case 2: chest--; printf("%d Got Shot in Chest\n", getpid()); break;
    case 3: abdomen--; printf("%d Got Shot in Abdomen\n", getpid()); break;
    case 4: limbs--; printf("%d Got Shot in Limbs\n", getpid()); break;
    default: printf("Error in location!!\n");
    }


    if(head <= 0 || neck <=0 || chest <=0 || abdomen <= 0 || limbs <= 0){
        if(!strcmp(me.type, "red")) {  
         printf("\033[0;31m");             
         printf("%d died\a\n", getpid());
         printf("\033[0m"); 
        }else {
         printf("\033[0;34m");    
         printf("%d died\a\n", getpid());
         printf("\033[0m"); 
        }
        exit(0);
    }
}

void handle_sigusr2(int sig) {
}

int main(int argc, char *argv[]) {

    int pid = getpid();
    char type[3];
    int x = atoi(argv[2]);
    int y = atoi(argv[3]);
    me.pid = pid;
    strcpy(me.type, argv[1]);
    me.x = x;
    me.y = y;
    // printf("PID: %d||Type: %s\n", me.pid, me.type);
    
    //Signal Handler
    signal(SIGUSR1, &handle_sigusr1);
    
    char namefifo[30] = "";
    sprintf(namefifo, "%d", getpid());

    char str[400] = " ";
    int fdopen = open(namefifo, O_RDONLY);
    int fread = read(fdopen, str, 400);;
    while(fread <= 0){
        fdopen = open(namefifo, O_RDONLY);
        fread = read(fdopen, str, 400);
        
        
    }
///////////////////////////////////////////////////// 
///////////////////////////////////////////////////// 
///////////////////////////////////////////////////// 
///////////////////////////////////////////////////// 
///////////////End of Read from FIFO/////////////////
///////////////////////////////////////////////////// 
///////////////////////////////////////////////////// 

    struct Child redTeam[5];
    struct Child blueTeam[5];
    int r =0;
    int b = 0;
    // Returns first token
    char *token = strtok(str, " ");
    for(int i =0; i<10; i++){
        if(!strcmp(token, "red")) {
        strcpy(redTeam[r].type, "red");
        token = strtok(NULL, " ");
        redTeam[r].pid = atoi(token);
        token = strtok(NULL, " ");
        redTeam[r].x = atoi(token);
        token = strtok(NULL, "\n");
        redTeam[r].y = atoi(token);
        r++;
        } else if(!strcmp(token, "blu")){
        strcpy(blueTeam[b].type, "blu");
        token = strtok(NULL, " ");
        blueTeam[b].pid = atoi(token);
        token = strtok(NULL, " ");
        blueTeam[b].x = atoi(token);
        token = strtok(NULL, "\n");
        blueTeam[b].y = atoi(token);   
        b++;
        }
        token = strtok(NULL, " ");
    }
       

/////////////////////////////////////////////////////////////////////////////// 
signal(SIGUSR2, &handle_sigusr2);
 //Wait for parent's order to start fighting
pause();
while(1){
    usleep(randNum(50000,100000));//Getting Ready and search time .....
    if(!strcmp(me.type, "blu")){
        if((-1 == kill(redTeam[0].pid, 0)) && (-1 == kill(redTeam[1].pid, 0)) && (-1 == kill(redTeam[2].pid, 0)) && (-1 == kill(redTeam[3].pid, 0)) && (-1 == kill(redTeam[4].pid, 0))) {
            kill(getppid(), SIGUSR1); //Blue win
            exit(4);            
        }else {
            pid_t enemPid = closest(redTeam, me); 
            printf("🪖 %s PID: %d ︻デ═一 PID: %d\n",me.type, me.pid, enemPid);
            if(shoot())          
                if(kill(enemPid, SIGUSR1) == 0) {
                    recharge();  /////////////////////////////////////
                } 

        }

    }else if(!strcmp(me.type, "red")){
        if((-1 == kill(blueTeam[0].pid, 0)) && (-1 == kill(blueTeam[1].pid, 0)) && (-1 == kill(blueTeam[2].pid, 0)) && (-1 == kill(blueTeam[3].pid, 0)) && (-1 == kill(blueTeam[4].pid, 0))){
            kill(getppid(), SIGUSR2); // Red win
            exit(5);
        }else {
            pid_t enemPid = closest(blueTeam, me);
            printf("🪖 %s PID: %d ︻デ═一 PID: %d\n",me.type, me.pid, enemPid);
            if(shoot())
                if(kill(enemPid, SIGUSR1) == 0) {
                    recharge();
                } 
        
        
        }
       
    }
    
}


return 0;
}