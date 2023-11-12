#include "Sabakus2WayPipe.h"
#include "SabakusIntArr.h"
#include "SabakusCharArr.h"
#include <stdio.h>
#include <unistd.h>
#include <ctype.h>
#include <time.h>

int SrcCharInArr(CharArr Array,  char a){ //cerca un char in una stringa, ritorna l'indice della prima comparsa.
    int i = 0;
    loop1:
    if (a == Array.Arr[i]) goto endloop1;
    i++;
    if (i<Array.Len) goto loop1;
    i=-1;
    endloop1:
    return i;
}


void Son(S2WPipe Pipe, int FatherPid,int SonPid){
    char* filepath = "..\\Mattarella.txt";  //dipende dalla posizione di esecuzione
    char* mode = "r";
    FILE *file = fopen(filepath,mode);

    if (file == NULL) {
        printf("si ma magari mettilo il file\n");
        return;
    }

    char c;

    while ((c = fgetc(file))!=EOF){
        S2WPipeWrite(Pipe, &c);
    }
    S2WPipeWrite(Pipe, &c);
    S2WPipeClose(Pipe);

}



void Father(S2WPipe Pipe, int FatherPid,int SonPid){
    IntArr Counters = IntArrContructor(0);
    CharArr Dictionary = CharArrContructor(0);
    clock_t Time = clock();
    char c;
    int i;

    while (1){

        SPReadings r = S2WPipeRead(Pipe);

        c = toupper(((char*)r.Data)[0]);

        if (c==EOF){
            break;
        }

        i = SrcCharInArr(Dictionary,c);
        if (c < 65 || c>90) continue;
        if (i == -1){
            CharAppend(&Dictionary,c);
            IntAppend(&Counters, 1);
        } else {
            Counters.Arr[i]++;
        }
    }

    for(i=0;i<Counters.Len;i++){
        printf("la lettera %c è presente %llu volte \n",Dictionary.Arr[i],Counters.Arr[i]);
    }
}




int main() { //qui presento il punto di inizio del programma dello sbocco.

    int FatherPid = getpid();
    S2WPipe Pipe = S2WPipeConstructor(FatherPid,0, 1);

    int SonPid = fork();
    short int AmSon;

    if (SonPid){
        AmSon = 0;
    } else {
        AmSon = 1;
        SonPid = getpid();
    }
    Pipe.Pid2 = SonPid;

    if (AmSon){
        Son(Pipe,FatherPid,SonPid);
    } else {
        Father(Pipe,FatherPid,SonPid);
    }

    return 0;
}