#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <curses.h>

pid_t create_process(char* argv[]);

void sig_handler(int signum) {
    if (signum == SIGCHLD) {
    }
}

int main() {
	pid_t pid;
    int c;

    char* child_argv[2];
    char name[] = "./toofast";
    child_argv[0] = name;
    child_argv[1] = NULL;

    pid = create_process(child_argv);
    if (pid == -1) {
        return EXIT_FAILURE;
    }

    initscr();
    cbreak();
    noecho();
    while ((c = getch()) != ERR) {
        if (c == 'u') {
            kill(pid, SIGUSR1);
        }
        else if (c == 'd') {
            kill(pid, SIGUSR2);
        }
        else if (c == 's') {
            kill(pid, SIGCONT);
        }
        else if (c == 'p') {
            kill(pid, SIGSTOP);
        }
        else if (c == 'q') {
            kill(pid, SIGINT);
            break;
        }
    }
    endwin();
	return EXIT_SUCCESS;
}

pid_t create_process(char* argv[]) {
	pid_t pid;

    pid = fork();
	if (pid == -1) {
		perror("fork");
		return -1;
	}

	if (pid == 0) {
        if (execv(argv[0], argv) == -1) {
            perror("execv");
            exit(EXIT_FAILURE);
        }
	}

    return pid;
}
