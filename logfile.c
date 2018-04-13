#include "logfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

FILE *fp = NULL;
clock_t start;

void writeinLog(char* text) {
  if(fp != NULL) {
    clock_t currentTime = clock();
    double timeFromStart = ((double) (currentTime - start)) / CLOCKS_PER_SEC;
    fprintf(fp, "%f - %.8d - %s\n", timeFromStart, getpid(), text);
    fflush(fp);
  }
}

int initLog() {
  const char* fileName = getenv("LOGFILENAME");

  if(fileName == NULL) {
    return 0;
  }

  fp = fopen(fileName, "a");
  if(fp == NULL) {
    //Error opening file
    return -1;
  }

  start = clock();
  return 0;
}

void finishLog() {
  fclose(fp);
}
