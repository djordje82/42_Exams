#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <signal.h>
#include <string.h>

// Timeout flag
volatile sig_atomic_t timeout = 0;

// Signal handler for SIGALRM
void handle_alarm(int sig) {
    if (sig == SIGALRM) {
        timeout = 1;
        fprintf(stderr, "Timeout occurred! Terminating...\n");
    }
}

// Function to create a child process with a pipe
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
        // Child process
        if (mode == 'r') {
            close(fd[0]);                // Close read end
            dup2(fd[1], STDOUT_FILENO);  // Redirect stdout to write end
            close(fd[1]);
        } else if (mode == 'w') {
            close(fd[1]);                // Close write end
            dup2(fd[0], STDIN_FILENO);   // Redirect stdin to read end
            close(fd[0]);
        } else {
            exit(EXIT_FAILURE);
        }
        execve(exec_name, argv, NULL);   // Execute the given program
        perror("execve");
        exit(EXIT_FAILURE);
    }

    // Parent process
    if (mode == 'r') {
        close(fd[1]);  // Close write end
        return fd[0];  // Return read end
    } else if (mode == 'w') {
        close(fd[0]);  // Close read end
        return fd[1];  // Return write end
    }

    return -1;
}

int main() {
    // Set up the SIGALRM handler
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = handle_alarm;
    sigaction(SIGALRM, &sa, NULL);

    // Set a timeout of 5 seconds
    alarm(5);

    // Test case 1: Running "ls" and capturing its output
    char *ls_args[] = {"/bin/ls", NULL};
    int read_fd = execute_with_pipe("/bin/ls", ls_args, 'r');
    
    // Test case 2: Running "wc -l" and sending input to it
    char *wc_args[] = {"/usr/bin/wc", "-l", NULL};
    int write_fd = execute_with_pipe("/usr/bin/wc", wc_args, 'w');

    // Buffer for reading data
    char buffer[4096];
    ssize_t bytes_read;

    // Read from "ls" and write to "wc"
    while (!timeout && (bytes_read = read(read_fd, buffer, sizeof(buffer))) > 0) {
        write(write_fd, buffer, bytes_read);
    }

    // Close the pipes
    close(read_fd);
    close(write_fd);

    // Wait for child processes to finish
    int status;
    while (wait(&status) > 0);

    // Cancel the alarm after completion
    alarm(0);

    if (timeout) {
        fprintf(stderr, "Process terminated due to timeout.\n");
        return EXIT_FAILURE;
    }

    printf("Processes completed successfully.\n");
    return EXIT_SUCCESS;
}
