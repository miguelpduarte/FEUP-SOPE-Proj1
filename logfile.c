#include "logfile.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

FILE *fp = NULL;
struct timespec start_time;

struct timespec get_time_diff(struct timespec start, struct timespec end) {
	struct timespec temp;
	if ((end.tv_nsec-start.tv_nsec)<0) {
		temp.tv_sec = end.tv_sec-start.tv_sec-1;
		temp.tv_nsec = 1000000000+end.tv_nsec-start.tv_nsec;
	} else {
		temp.tv_sec = end.tv_sec-start.tv_sec;
		temp.tv_nsec = end.tv_nsec-start.tv_nsec;
	}
	return temp;
}

double get_milis_since_start() {
  struct timespec current_time;
  if(clock_gettime(CLOCK_REALTIME, &current_time) != 0) {
    fprintf(stderr, "Error in measuring current time!\n");
    return -1;
  }

  struct timespec timediff = get_time_diff(start_time, current_time);

  return (double) timediff.tv_sec * 1000.0 + (double) timediff.tv_nsec / 1000.0;
}

void writeinLog(char* text) {
  if(fp != NULL) {
    fprintf(fp, "%.2f - %.8d - %s\n", get_milis_since_start(), getpid(), text);
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

  if(clock_gettime(CLOCK_REALTIME, &start_time) != 0) {
    fprintf(stderr, "Error in starting timer for logfile!\n");
    return -2;
  }

  return 0;
}

void finishLog() {
  fclose(fp);
}
