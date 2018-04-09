#ifndef _SIGHANDLERS_H_
#define _SIGHANDLERS_H_

void termination_handler(int signo);
void exit_handler(int signo);
void pausing_handler(int signo);
void install_main_handler();
void install_child_handler();

#endif  /* _SIGHANDLERS_H_ */