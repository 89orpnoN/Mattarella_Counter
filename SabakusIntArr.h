#ifndef SabakusIntArr
#define SabakusIntArr
#include <stdlib.h>

//Vi presento la versione "migliorata" dei Sabaku's arrays
//questa volta più grossi, più spavaldi e più peggiori di sempre
//I SABAKU'S HEAVY DUTY ARRAYS, che includono un contatore dello spazio in formato Unsigned long long int

//array di interi
typedef struct  { //secondo i miei calcoli questo array può ospitare 18*10^18 valori, che riempito completamente ammonterebbe a 65 milioni di terabyte
    int* Arr;
    long long unsigned int Len;
    long long unsigned int Tlen;
}IntArr;

 IntArr IntArrContructor(long long int len){
     IntArr item;
    item.Len = len;
    item.Tlen = len+100;
    item.Arr = calloc(item.Tlen,sizeof( int ));
    return item;
};

int ExpandIntArr( IntArr* arr){
    arr->Tlen*=2;
    arr->Arr = realloc(arr->Arr,sizeof(int)*arr->Tlen);
    if (arr->Arr == NULL) return 0;
    return 1;
};

int IntAppend( IntArr* arr, int i){
    arr->Arr[arr->Len] = i;
    arr->Len++;
    if (arr->Len>=arr->Tlen){

        if(!ExpandIntArr(arr)) return 0;
    }
    return 1;
};

int IntPop( IntArr* arr){
    if(arr->Len<1) return 0;
    arr->Len--;
    arr->Arr[arr->Len]=0;
    return 1;
};

#endif //Sabakus_Arrays
