
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

int ricercaBin(int* indici, int num, int posfin){//posfin coincide sempre con il numero di righe attive (count)
    posfin--;
    if((num < indici[0])||(num>indici[posfin])){
        return -1;
    }
    int p=0;
    int q=0;
    while(p<=posfin){
        q=(p+posfin)/2;
        if(indici[q]==num) return q;
        if(indici[q]>num) posfin=q-1;
        else p=q+1;
    }
    return -1;
}


void countingSortMod(int* indici, int size ,int max){
        int c[max];
        for(int i=0; i<max; i++){
            c[i]=0;
        }
        for(int i=0; i<size; i++){
            if(indici[i]!=0){
                c[indici[i]-1]++;
            }
        }
        int k=0;
        for(int i=0; i<max;i++ ){
            while(c[i]>0){
                indici[k]=i+1;
                k++;
                c[i]--;
            }
        }
        for(k; k<size; k++){
            indici[k]=0;
        }
}

int main(){
    int numero = 28;
    int risultato = numero/10;
    printf("%d", risultato); 
    }