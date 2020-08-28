#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

//countcmd parte da 0, aumenta dopo il primo (tutto slittato di 1 in dietro)

struct row{
    char* stringa;
    int comando;
    struct row* next;
    struct row* prev;
};

#define INPUTLENGHT   32
#define ROWLENGHT   1024
 
void estraiDue(char* input,const char* delim, int* primary, int* secondary){//OK
    char* token = strtok(input, delim);
    primary[0] = atoi(token);
    token = strtok(NULL, delim);
    secondary[0] = atoi(token);
}

void estraiUno(char* input,const char* delim, int* primary){//OK
    char* token = strtok(input, delim);
    primary[0] = atoi(token);
}

void printer(int* primary, int* secondary, int* cmd, struct row*** versioni, int* countRow){
    short dist;
    short out=0;
    if(secondary[0]>countRow[0]) {
        dist = countRow[0] - primary[0] + 1;
        out = secondary[0] - countRow[0];
    }
    else dist = secondary[0] - primary[0] + 1;
    if(countRow[0]!=0){
        for(short i=0; i<dist;i++){
            printf("%s", versioni[cmd[0]][primary[0]-1+i]->stringa);
        }
    }
    while(out>0){
        printf(".\n");
        out--;  
    }
}

struct row* changer(int* primary, int* secondary, int* cmd, struct row* tail, struct row*** versioni, int* sizeVersioni, int* countRow){ 
    struct row* new;
    cmd[0]++;
    short dist = secondary[0] - primary[0] + 1;
    //creo nuova versione
    if(cmd[0]!=0){
        //int sizePrev;
        //if(versioni[cmd[0]-1]!=NULL){
        //    sizePrev = sizeof(versioni[cmd[0]-1])/sizeof(struct row*);
        //}
        //else sizePrev = 0; 
        if(cmd[0]==sizeVersioni[0]){
            sizeVersioni[0] =  sizeVersioni[0]*2;
            versioni = (struct row***) realloc(versioni, sizeof(struct row**)*sizeVersioni[0]);
        }
        if(secondary[0]<=countRow[0]){
            versioni[cmd[0]] = (struct row**) realloc(versioni[cmd[0]-1], sizeof(struct row*)*(countRow[0]));
        }
        else{
            versioni[cmd[0]] = (struct row**) realloc(versioni[cmd[0]-1], sizeof(struct row*)*secondary[0]);
            countRow[0]=secondary[0];
        }        
    }
    else{
        versioni[0] = (struct row**) malloc(sizeof(struct row*)*dist);
        countRow[0] = secondary[0];
    }
    for(short i=0; i<dist;i++){
        //creo nuova riga e la metto in coda, nuova riga ora è la coda.
        new = (struct row*) malloc(sizeof(struct row));
        new->comando = cmd[0];
        new->stringa = (char*) malloc(sizeof(char)*ROWLENGHT);
        fgets(new->stringa, sizeof(char)*ROWLENGHT, stdin);
        new->prev = tail;
        tail->next = new;
        tail = new;
        //metto il puntatore alla nuova riga nella versione, al posto corrispondente
        versioni[cmd[0]][primary[0]-1+i] = new;
    }
    return new;
}

void deleter(int* primary, int* secondary, int* cmd, struct row*** versioni, int* sizeVersioni, int* countRow){
    cmd[0]++;
    short dist;
    if(countRow[0]==0){ //comandi successivi sono del oppure ho già cancellato tutte le righe
        if(cmd[0]==sizeVersioni[0]){
            sizeVersioni[0] =  sizeVersioni[0]*2;
            versioni = (struct row***) realloc(versioni, sizeof(struct row**)*sizeVersioni[0]);
        }
        versioni[cmd[0]] = NULL;
        return;        
    } 
    if(primary[0]>countRow[0]){ //se la del è su righe non presenti 
        versioni[cmd[0]] = (struct row **) malloc(sizeof(struct row *)* countRow[0]);
        for(short i=0; i<countRow[0];i++){
            versioni[cmd[0]][i] = versioni[cmd[0]-1][i];
        }
        return;
    }
    if(secondary[0]>countRow[0]) {
        secondary[0] = countRow[0];
    }
    dist = secondary[0] - primary[0] + 1;
    if(cmd[0]!=0){
        if(cmd[0]==sizeVersioni[0]){
            sizeVersioni[0] =  sizeVersioni[0]*2;
            versioni = (struct row***) realloc(versioni, sizeof(struct row**)*sizeVersioni[0]);
        }
        countRow[0]=countRow[0]-dist;   
        versioni[cmd[0]] = (struct row**) malloc(sizeof(struct row*)*(countRow[0]));
        //for dove copio solo i puntatori che mi interessano
        short shift = 1;
        for(short i=0; i<countRow[0];i++){
            if(i<primary[0]-1){
                versioni[cmd[0]][i] = versioni[cmd[0]-1][i];
            }
            else{ //if(i>=primary[0]-1)
                versioni[cmd[0]][i] = versioni[cmd[0]-1][i+dist];
            }
        }
        return;
    }
}

void undo(){
    return;
}

void libera(){
    return;
}

int main () {
    
    char input[INPUTLENGHT];  //contenitore num1,num2Command 
    char dot[2];
    char cmd[1]; //contenitore Command


    int countRow[1];
    countRow[0]=0;

    //inizio lista concatenata di righe
    struct row* head;
    struct row* tail = (struct row*) malloc(sizeof(struct row));
    tail->next=NULL;
    tail->prev=NULL;
    tail->stringa=NULL;
    tail->comando=0;
    head = tail;


    //Matrice versioni
    int countcmd[1]; //conta anche le versioni
    countcmd[0]=-1; //conto solo c o d
    int sizeVersion[1];
    sizeVersion[0]=100;
    struct row*** versioni = (struct row***) malloc(sizeof(struct row**)*sizeVersion[0]);


    int primary[1]; //modifico questi indici nella matrice delle versioni
    int secondary[1];

    int virtual[1];
    virtual[0]=-1;

    const char delim[] = ", c d p u r"; 
    fgets(input, sizeof(input), stdin);
    while(strcmp(input, "q\n")!=0){
        cmd[0]=input[strlen(input)-2];
        while((cmd[0]=='u')||(cmd[0]=='r')){
            if(virtual[0]==-1) break;
            estraiUno(input, delim, primary);
            if(cmd[0]=='u') virtual[0]=virtual[0]+primary[0];
            else virtual[0]=virtual[0]-primary[0];
            fgets(input, sizeof(char)*INPUTLENGHT, stdin);
        }
        if(cmd[0]=='c'){ //change
            if(virtual[0]<countcmd[0]){
                undo();
                libera();
                countcmd[0] = virtual[0];
            }
            estraiDue(input, delim, primary, secondary);
            tail = changer(primary, secondary, countcmd, tail, versioni, sizeVersion, countRow);
            fgets(dot, sizeof(char)*2, stdin); //termino con punto, solo per scorrere gli input   
            virtual[0] = countcmd[0]; 
            //printf("Change finita\n");       
        }
        else if (cmd[0]=='d'){ //delete
            if(virtual[0]<countcmd[0]){
                undo();
                libera();
                countcmd[0] = virtual[0];
            }
            estraiDue(input, delim, primary, secondary);
            deleter(primary, secondary, countcmd, versioni, sizeVersion, countRow);
            virtual[0] = countcmd[0]; 
        }
        else if (cmd[0]=='p'){ //print
            if(virtual[0]<countcmd[0]){
                undo();
            }
            else if (virtual[0]>countcmd[0]){
                redo();//al massimo torno a countcmd
            }
            estraiDue(input, delim, primary, secondary);
            printer(primary, secondary, countcmd, versioni, countRow);
            //printf("Print finita\n");
        }
        //printf("%d %c\n", countcmd[0], lstcmds[countcmd[0]].cmd[0]);
        fgets(input, sizeof(char)*INPUTLENGHT, stdin);     
    }
    return 0;
}