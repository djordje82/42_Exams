#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

int execute_with_pipe(const char *exec_name, char *const argv[], char mode) {
  int fd[2];
  pid_t pid;

  if (pipe(fd) == -1) {
    perror("pipe");
    exit(EXIT_FAILURE);
  }

  pid = fork();
  if (pid == -1) {
    perror("fork");
    exit(EXIT_FAILURE);
  }

  if (pid == 0) {
    if (mode == 'r') {
      close(fd[0]);
      dup2(fd[1], STDOUT_FILENO);
      close(fd[1]);
    } else if (mode == 'w') {
      close(fd[1]);
      dup2(fd[0], STDIN_FILENO);
      close(fd[0]);
    } else {
      fprintf(stderr, "Invalid mode. Use 'r' for read or 'w' for write.\n");
      exit(EXIT_FAILURE);
    }

    execve(exec_name, argv, NULL);
    perror("execve");
    exit(EXIT_FAILURE);
  } else {
    if (mode == 'r') {
      close(fd[1]);
      return fd[0];
    } else if (mode == 'w') {
      close(fd[0]);
      return fd[1];
    }
  }
  return -1;
}
