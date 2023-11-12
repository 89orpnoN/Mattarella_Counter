#ifndef SABAKUS2WAYPIPE
#define SABAKUS2WAYPIPE

#include <unistd.h>
#include <stdlib.h>



short int TypeLenghts[4] = {
        1,
        sizeof(char),
        sizeof(int),
        sizeof(float)
};

typedef struct {
    short int Err;
    int UpStream[2];
    int DownStream[2];
    short int Type;
    short int ChunkSize;
    int Pid1;
    int Pid2;

}S2WPipe;

typedef struct {
    short int Err;
    void* Data;
    short int Type;
}SPReadings;

S2WPipe S2WPipeConstructor(int Pid1,int Pid2, short int type){
    S2WPipe Pipe;

    Pipe.Pid1 = Pid1;
    Pipe.Pid2 = Pid2;
    Pipe.Type = type;
    Pipe.ChunkSize = TypeLenghts[type];
    Pipe.Err = 0;

    if (pipe(Pipe.UpStream)<0 || pipe(Pipe.DownStream)<0) Pipe.Err = 1;

    return Pipe;
}

SPReadings S2WPipeRead(S2WPipe pipe){
    SPReadings a;

    a.Data = calloc(TypeLenghts[pipe.Type],1);
    a.Err = 0;
    a.Type = pipe.Type;

    int access, current_Pid = getpid();
    if (current_Pid == pipe.Pid1){
        access = pipe.UpStream[0];
    } else if (current_Pid == pipe.Pid2){
        access = pipe.DownStream[0];
    } else {
        a.Err = 3;
        return a;
    }

    if (a.Data == NULL) a.Err = 1;

    if (read(access,a.Data,TypeLenghts[pipe.Type]) != TypeLenghts[pipe.Type]){
        a.Err = 2;
    }

    return a;
}

int S2WPipeWrite(S2WPipe pipe, void* ptr){

    int access, current_Pid = getpid();
    if (current_Pid == pipe.Pid1){
        access = pipe.DownStream[1];
    } else if (current_Pid == pipe.Pid2){
        access = pipe.UpStream[1];
    } else {
        return 3;
    }

    if (write(access,ptr,TypeLenghts[pipe.Type]) != TypeLenghts[pipe.Type]){
        return 2;
    }
    return 0;
}

int S2WPipeClose(S2WPipe pipe){

    int current_Pid = getpid();
    if (current_Pid == pipe.Pid1){
        close( pipe.DownStream[1]);
    } else if (current_Pid == pipe.Pid2){
        close( pipe.UpStream[1]);
    } else {
        return 3;
    }
}

#endif //SABAKUS2WAYPIPE
