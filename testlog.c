#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

FILE *fp = NULL;
clock_t start;

int writeinLog(char* text,pid_t id ){
  if(fp != NULL){
    clock_t currentTime = clock();
    double timeFromStart = ((double) (currentTime - start)) / CLOCKS_PER_SEC;
    fprintf(fp,"%f - %d - %s \n" ,timeFromStart,id,text);
    return 0;
  }
  return -1;

}
int main(int argc, char* argv[]){

    const char* fileName = getenv("LOGFILENAME");


    fp = fopen(fileName, "a");
    start = clock();
    char* text = "COMMANDO teste";
    writeinLog(text,getpid());
}
