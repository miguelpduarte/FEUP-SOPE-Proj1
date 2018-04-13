#include "sighandlers.h"
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <stdbool.h>
#include <unistd.h>
#include "logfile.h"

#define BUF_MAX_SIZE 512

void termination_handler(int signo) {

  writeinLog("RECEBIDO SIGINT");
  
  //Halting child processes
  writeinLog("SINAL TSTP para process group (pause)");
  kill(0, SIGTSTP);

  char * buffer = NULL;
  int answer = 0;
  size_t n;

  do {
    printf("\nAre you sure you want to terminate the program? (Y/N)\n");
    getline(&buffer, &n, stdin);

    if(strncasecmp(buffer, "Y", 1) == 0) {
        answer = 1;
        break;
    } else if(strncasecmp(buffer, "N", 1) == 0) {
        answer = 0;
        break;
    }

  } while(1);

  free(buffer);

  //If anwser was yes, exit and cascade kill
  if(answer == 1) {
    writeinLog("SINAL USR1 para process group (terminate)");
    kill(0, SIGUSR1);
    exit(0);
  } else {
    //If answer was no, "unpause" child processes
    writeinLog("SINAL CONT para process group (unpause)");
    kill(0, SIGCONT);
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
  sigaddset(&(int_action.sa_mask), SIGUSR1);
  sigaddset(&(int_action.sa_mask), SIGUSR2);
  sigaddset(&(int_action.sa_mask), SIGTSTP);
  sigaddset(&(int_action.sa_mask), SIGCONT);
  sigaction(SIGINT, &int_action, NULL);

  struct sigaction usr1_action;
  usr1_action.sa_handler = exit_handler;
  usr1_action.sa_flags = 0;
  sigemptyset(&(usr1_action.sa_mask));
  sigaction(SIGUSR1, &usr1_action, NULL);

  struct sigaction tstp_ign_action;
  tstp_ign_action.sa_handler = SIG_IGN;
  tstp_ign_action.sa_flags = 0;
  sigemptyset(&(tstp_ign_action.sa_mask));
  sigaction(SIGTSTP, &tstp_ign_action, NULL);

  struct sigaction cont_ign_action;
  cont_ign_action.sa_handler = SIG_IGN;
  cont_ign_action.sa_flags = 0;
  sigemptyset(&(cont_ign_action.sa_mask));
  sigaction(SIGCONT, &cont_ign_action, NULL);
}

void install_child_handler() {
  struct sigaction int_ign_action;
  int_ign_action.sa_handler = SIG_IGN;
  int_ign_action.sa_flags = 0;
  sigemptyset(&(int_ign_action.sa_mask));
  sigaction(SIGINT, &int_ign_action, NULL);

  struct sigaction tstp_action;
  tstp_action.sa_handler = SIG_DFL;
  tstp_action.sa_flags = 0;
  sigemptyset(&(tstp_action.sa_mask));
  sigaction(SIGTSTP, &tstp_action, NULL);

  struct sigaction cont_action;
  cont_action.sa_handler = SIG_DFL;
  cont_action.sa_flags = 0;
  sigemptyset(&(cont_action.sa_mask));
  sigaction(SIGCONT, &cont_action, NULL);
}