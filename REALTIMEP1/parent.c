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
#include <math.h>
#include <fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>

struct Child { 
    pid_t pid;
    char type[3];
    int x;
    int y;   
};

struct Child arena[50][50];

//Generating a random number
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



struct Child location(struct Child arena[50][50], struct Child child) {
    int i = randNum(0, 50);
    int j = randNum(0, 50);
    while(i == j)
     j = randNum(0, 50);
    //Check if this position is not occupied
    if(arena[i][j].pid == 0) {        
        child.x = i;
        child.y = j;
        arena[i][j] = child;
        return child;
    }
    else location(arena, child);
}



struct Child Sold[10];
int blue = 0;
int red = 0;
int round1 = 1;
//blue wins
void handle_sigusr1(int sig) {
    blue++;
    system("pkill sold");
    printf("🔷  Blue Team won this round (⌐■_■)–︻╦╤─\n");
    if(blue == 5) {
        printf("\n");
        printf("🎖️  Blue Won the whole GAME!\n");
        exit(0);
    }
    return 0;
}

//red wins
void handle_sigusr2(int sig) {
    red++;
    system("pkill sold");
    printf("🔴 RED Team won this round (⌐■_■)–︻╦╤─\n");
    if(red == 5) {
        printf("\n");
        printf("🎖️  RED Won the whole GAME!\n");
        exit(0);
    }
    return 0;
}

void showArena() {
    printf("🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡\n");
    for(int i=0; i<50; i++) {
        for(int j=0; j<50; j++) {
            if(arena[i][j].pid >0)
                {
                    if(!strcmp(arena[i][j].type, "red"))
                    printf("🔴");
                    else 
                    printf("🔵");
                }
            else {
            
                    printf("⚫");
                
            }
        }
        printf("🟡\n");
    }
    printf("🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡🟡\n");
    return 0;
}



int main(void) {
    
                                                                  
    ///////////////////////////
    //Handle signals sent by children...
    signal(SIGUSR1, &handle_sigusr1);
    signal(SIGUSR2, &handle_sigusr2);   

system("clear");
    while(1) {
        memset(arena, 0, sizeof(arena));//Clear Array before using it... :)
        printf("\033[0;33m"); 
        printf("####################################################################\n");
        printf("#######                                                       ######\n");
        printf("########            🆆 🅰 🆁    🆂 🅸 🅼 🆄 🅻 🅰 🆃 🅸 🅾 🅽            ########\n");
        printf("#########                                                  #########\n");
        printf("####################################################################\n");
        printf("#####  🔴 🆁 🅴 🅳   🆃 🅴 🅰 🅼     🆚     🔵 🅱 🅻 🆄 🅴  🆃 🅴 🅰 🅼       #####\n");
        printf("######❶ View Arena    ❷ Start Round #%d         ❸ EXIT         ######\n", round1);
        printf("####################################################################\n");
        printf("\033[0m"); 
        
        printf("Choice:> ");
        char buf[10];
        int choice;
        fgets(buf, sizeof buf, stdin);
        choice = atoi(buf);
        struct Child soldiure;

    pid_t pid = getpid();
    for(int i=0; i<5; i++) {
         if(pid>0) {
            pid = fork();
            if(pid>0) {
                strcpy(Sold[i].type, "red");
                Sold[i].pid = pid;             
                Sold[i].x = soldiure.x;
                Sold[i].y = soldiure.y;
                soldiure = location(arena, Sold[i]); 
                
            }
         }
         
    }
    char Soldx[5];
    char Soldy[5];
    sprintf(Soldx, "%d", soldiure.x);
    sprintf(Soldy, "%d", soldiure.y);
    if(pid == 0) { //Code for Red team
        const char* args[] = {"sold", "red", Soldx, Soldy, NULL};
        int ret;
        ret = execv("./sold", args);
        if(ret == -1) {
            perror("Error in Execv!");
            exit(EXIT_FAILURE);
        }
        return 0;
    }

    //////////////////////////////////////////////////////////
    //Creating blue team
    for(int i=5; i<10; i++) {
         if(pid>0) {
            pid = fork();
            if(pid>0) {
                strcpy(Sold[i].type, "blu");
                Sold[i].pid = pid;                
                Sold[i].x = soldiure.x;
                Sold[i].y = soldiure.y;
                soldiure = location(arena, Sold[i]); 
                
            }
         }
         
    }

    sprintf(Soldx, "%d", soldiure.x);
    sprintf(Soldy, "%d", soldiure.y);
    if(pid == 0) { //Code for blu team
        const char* args[] = {"sold", "blu", Soldx, Soldy, NULL};
        int ret;
        ret = execv("./sold", args);
        if(ret == -1) {
            perror("Error in Execv!");
            exit(EXIT_FAILURE);
        }
        return 0;
    }
/////////////////////////////////////////////////
/////////////////// FIFO ////////////////////////
/////////////////////////////////////////////////
    char str[200] = "";
    
    if(pid > 0) {
        char sPid[10];
        char sX[10];
        char sY[10];
        for(int i=0; i<10; i++) {
            char Line[50] = "";
            sprintf(sPid, "%d", Sold[i].pid);
            sprintf(sX, "%d", Sold[i].x);
            sprintf(sY, "%d", Sold[i].y);
            strcat(Line , Sold[i].type);
            strcat(Line, " ");
            strcat(Line ,sPid);
            strcat(Line, " ");
            strcat(Line ,sX);
            strcat(Line, " ");
            strcat(Line ,sY);
            strcat(Line, "\n");
            strcat(str, Line);                 
        
        }

    }  


    for(int i=0; i<5; i++) {//Giving a RED once and a BLUE ONCE to avoid race conditions
        char namefifo[20];//Giving RED FIFO
        sprintf(namefifo, "%d", Sold[i].pid);
        mkfifo(namefifo, 0777);
        int publicfifo = open(namefifo, O_WRONLY);
        write(publicfifo,(char *) &str, sizeof(str));  
        close(namefifo); 
        remove(namefifo);


        char namefifo1[20];//Giving BLUE FIFO
        sprintf(namefifo1, "%d", Sold[i+5].pid);
        mkfifo(namefifo1, 0777);
        int publicfifo1 = open(namefifo1, O_WRONLY);
        write(publicfifo1,(char *) &str, sizeof(str));  
        close(namefifo1); 
        remove(namefifo1);
    }

    start:
    if(choice == 2) {
        for(int i=0; i<5; i++) {
            kill(Sold[i].pid, SIGUSR2);
            kill(Sold[i+5].pid, SIGUSR2);
            
            
        }
        round1++;
    } else if(choice == 3) 
        {
            system("pkill sold");
            exit(0);
    } else if(choice == 1) {
            showArena();
            goto while_Loop;
            
    } else if((choice > 3 || choice <1)){
            printf("\33[2K");
            printf("\033[A");
            printf("\r");
            goto while_Loop;
            
    }
    if(0) {
        while_Loop:
            printf("Choice:> ");
            choice = 0;
            fgets(buf, sizeof buf, stdin);
            choice = atoi(buf);
            goto start;
    }


        pid_t wpid;
        int status = 0;
        while ((wpid = wait(&status)) > 0);
 }
/////////////////////////////////////////////////
/////////////////// FIFO ////////////////////////
/////////////////////////////////////////////////
return 0;
}




