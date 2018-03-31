#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>
#include <stdlib.h>

#define BUF_MAX_SIZE 512

void controlhandler(int signo);
int main(int argc, char* argv[]){
  printf("Try to Crtl C \n");
  signal(SIGINT,controlhandler);
  pid_t pid;
  while(1)
  {
    sleep(2);
    pid = fork();
    if(pid == 0){
      signal(SIGINT,SIG_IGN);
      //printf("My parent is PPID: %d \n",getppid() );
    }
  }
  return 0;
}

void controlhandler(int signo){
  signal(SIGINT,controlhandler);
  printf("Are you sure you want to terminate the program? (Y/N) \n");
  char buffer[BUF_MAX_SIZE];
  scanf("%s",buffer);

  while( (strcmp(buffer,"Y") != 0) && strcmp(buffer, "N") != 0 ){
    printf("\n Are you sure you want to terminate the program? (Y/N) \n");
    scanf("%s",buffer);
  }

  if(strcmp(buffer,"N") == 0){
    printf("Ok program will not stop \n");
  } else {
    printf("Program will stop \n");
    kill(0,SIGTERM);
    exit(0);
  }

}
