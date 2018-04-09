#include "sighandlers.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>

#define BUF_MAX_SIZE 512

void termination_handler(int signo) {
  
  //Halting child processes
  kill(0, SIGUSR2);

  char buffer[BUF_MAX_SIZE];
  int answer = 0;

  do {
    printf("\n Are you sure you want to terminate the program? (Y/N) \n");
    scanf("%s",buffer);

    if(strncasecmp(buffer, "Y", 1) != 0) {
        answer = 1;
        break;
    } else if(strncasecmp(buffer, "N", 1) != 0) {
        answer = 0;
        break;
    }

  } while(1);

  //If anwser was yes, exit and cascade kill
  if(answer == 1) {
    kill(0, SIGUSR1);
    exit(0);
  } else {
    //If answer was no, "unpause" child processes
    kill(0, SIGUSR2);
  }
}

void exit_handler(int signo) {
  exit(0);
}

void pausing_handler(int signo) {
  static bool paused = false;
  paused = !paused;

  while(paused) {
      sleep(1);
  }
}

void install_main_handler() {
  struct sigaction int_action;
  int_action.sa_handler = termination_handler;
  int_action.sa_flags = 0;
  sigemptyset(&(int_action.sa_mask));
  sigaddset(&(int_action.sa_mask), SIGINT);
  sigaddset(&(int_action.sa_mask), SIGUSR2);
  sigaction(SIGINT, &int_action, NULL);

  struct sigaction usr1_action;
  usr1_action.sa_handler = exit_handler;
  usr1_action.sa_flags = 0;
  sigemptyset(&(usr1_action.sa_mask));
  sigaction(SIGUSR1, &usr1_action, NULL);
}

void install_child_handler() {
  struct sigaction int_ign_action;
  int_ign_action.sa_handler = SIG_IGN;
  int_ign_action.sa_flags = 0;
  sigemptyset(&(int_ign_action.sa_mask));
  sigaction(SIGINT, &int_ign_action, NULL);

  struct sigaction pause_action;
  pause_action.sa_handler = pausing_handler;
  pause_action.sa_flags = 0;
  sigfillset(&(pause_action.sa_mask));
  sigdelset(&(pause_action.sa_mask), SIGTERM);
  sigdelset(&(pause_action.sa_mask), SIGUSR1);
  sigdelset(&(pause_action.sa_mask), SIGUSR2);
  sigaction(SIGUSR2, &pause_action, NULL);
}