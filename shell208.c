/*
  Ben Zhao and Ntense Obono
  shell208.c
  A simple command shell that supports a few features
*/

#include    <stdio.h>
#include    <string.h>
#include    <assert.h>
#include    <unistd.h>
#include    <sys/types.h>
#include    <sys/wait.h>
#include    <stdlib.h>
#include	<sys/stat.h>
#include	<fcntl.h>
#include	<ctype.h>

// The command buffer will need to have room to hold the
// command, the \n at the end of the command, and the \0.
// That's why the maximum command size is 2 less than the
// command buffer size.
#define COMMAND_BUFFER_SIZE       102
#define MAX_COMMAND_SIZE          COMMAND_BUFFER_SIZE - 2

// Return values for get_command
#define COMMAND_INPUT_SUCCEEDED   0
#define COMMAND_INPUT_FAILED      1
#define COMMAND_END_OF_FILE       2
#define COMMAND_TOO_LONG          3

int get_command(char *command_buffer, int buffer_size);
void execute_command(char *command_line);

int main() {
    const char *prompt = "WrongSyntaxShell$ ";
    char command_line[COMMAND_BUFFER_SIZE];
    

    // The main infinite loop
    while (1) {
        printf("%s", prompt);
        fflush(stdout);

        int result = get_command(command_line, COMMAND_BUFFER_SIZE);
        if (result == COMMAND_END_OF_FILE) {
            // stdin has reached EOF, so it's time to be done. This often happens
            // when the user hits Ctrl-D.
            break;

        } else if (result == COMMAND_INPUT_FAILED) {
            fprintf(stderr, "There was a problem reading your command. Please try again.\n");
            clearerr(stdin);
            continue;

        } else if (result == COMMAND_TOO_LONG) {
            fprintf(stderr, "Commands are limited to length %d. Please try again.\n", MAX_COMMAND_SIZE);

        } 
        else if (strcmp(command_line, "q") == 0) {
            break; // Exit the loop when user inputs "q"
        } else {
            execute_command(command_line);
            }
    }

    return 0;
}

/*
    Retrieves the next line of input from stdin (where, typically, the user
    has typed a command) and stores it in command_buffer.

    The newline character (\n, ASCII 10) character at the end of the input
    line will be read from stdin but not stored in command_buffer.

    The input stored in command_buffer will be \0-terminated.

    Returns:
        COMMAND_TOO_LONG if the number of chars in the input line
            (including the \n), is greater than or equal to buffer_size
        COMMAND_INPUT_FAILED if the input operation fails with an error
        COMMAND_END_OF_FILE if the input operation fails with feof(stdin) == true
        COMMAND_INPUT_SUCCEEDED otherwise

    Preconditions:
        - buffer_size > 0
        - command_buffer != NULL
        - command_buffer points to a buffer large enough for at least buffer_size chars
*/
int get_command(char *command_buffer, int buffer_size) {
    assert(buffer_size > 0);
    assert(command_buffer != NULL);

    if (fgets(command_buffer, buffer_size, stdin) == NULL) {
        if (feof(stdin)) {
            return COMMAND_END_OF_FILE;
        } else {
            return COMMAND_INPUT_FAILED;
        }
    }

    int command_length = strlen(command_buffer);
    if (command_buffer[command_length - 1] != '\n') {
        // If we get here, the input line hasn't been fully read yet.
        // We need to read the rest of the input line so the unread portion
        // of the line doesn't corrupt the next command the user types.
        char ch = getchar();
        while (ch != '\n' && ch != EOF) {
            ch = getchar();
        }

        return COMMAND_TOO_LONG;
    }

    // remove the newline character
    command_buffer[command_length - 1] = '\0';
    return COMMAND_INPUT_SUCCEEDED;
}

void execute_command(char *command_line) {
    pid_t pid = fork();

    if (pid != 0) {
        /* Parent */


        int status;
        pid = wait(&status);
      

    } else {
        /* Child */
        char *input[MAX_COMMAND_SIZE];
        int argc = 0;
        char *values = strtok(command_line, " ");
        while (values != NULL){
            if (strcmp(values, "help") == 0) {
                printf("Available features:\n1) Print prompt and get one-word command from user, then execute command (ex: ls, wc, etc).\n2) Support single commands with command-line arguments (ex: wc -l something.txt, ls -l -a, etc).\n3) Support command > file redirecting command's stdout to a file.\n4)Supports Single Pipe Commands.\n5)Supports command < file redirecting command's stdin from a file.\n6)Type q to quit.\n");
                fflush(stdout);
                exit(0);
            }
            input[argc] = values;
            values = strtok(NULL, " ");
            argc++;
        }
        input[argc] = NULL;
        int index = 0;
        int pipe_exist = 0;
        int less = 0;
        for (int i = 0; i < argc; i++){
            if (strcmp(input[i], ">") == 0){
                index = i;
                break;
            }
            if (strcmp(input[i], "|") == 0){
                pipe_exist = i;
                break;
            }
            if (strcmp(input[i], "<") == 0){
                less = i;
                break;
            }
            
        }
        if (less > 0){
            char *file_name = input[less + 1];
            input[less] = NULL;
            int fd = open(file_name, O_RDONLY);
            if (fd < 0) {
                perror("Trouble opening file");
                exit(1);
	        }
            if (dup2(fd, 0) >= 0){
                execvp(input[0], input);
                perror("exec failed");
                fflush(stdout);
                exit(1);
            }
            perror("Trouble with direct to stdin");
            close(fd);
            exit(1);
        }
        if (index > 0){
            const char *file_name = input[index + 1];
            int fd = open(file_name, O_WRONLY | O_CREAT | O_TRUNC, 0644);
            if (fd < 0) {
                perror("Trouble opening file");
                exit(1);
	        }
            if (dup2(fd, STDOUT_FILENO) >= 0){
                input[index] = NULL;
                execvp(input[0], input);
                perror("exec failed");
                fflush(stdout);
                exit(1);
            }
            perror("Trouble dup2-ing to stdout");
            close(fd);
            exit(1);
        }
      

        if (pipe_exist > 0) {
            int fd[2];
            char *argv1[pipe_exist+1];
            for (int i = 0; i < pipe_exist; i++){
                argv1[i] = input[i];
            }
            argv1[pipe_exist] = NULL;
            char *argv2[argc - pipe_exist];
            int secondIndex = pipe_exist + 1;
            for (int i = 0; i < argc-pipe_exist-1; i++){
                argv2[i] = input[secondIndex];
                secondIndex++;
            }
            argv2[argc - pipe_exist - 1] = NULL;
            if (pipe(fd) < 0) {
                perror("Trouble creating pipe");
                exit(1);
            }

            int pid = fork();
            if (pid < 0) {
                perror("Trouble forking");
                exit(1);
            }

            if (pid == 0){
                close(fd[0]);
                if (dup2(fd[1], STDOUT_FILENO) == -1) {
                    perror("Trouble redirecting stdout");
                    exit(1);
                }
                close(fd[1]);
                execvp(argv1[0], argv1);
                perror("execvp in first child failed");
                exit(1);
            }
            if (pid != 0){
                int pid2 = fork();
                if (pid2 < 0) {
                    perror("Trouble forking");
                    exit(1);
                }
                if (pid2 != 0){
                    close(fd[0]);
                    close(fd[1]);
                    wait(NULL);
                }
                if (pid2 == 0) {
                    close(fd[1]);
                    if (dup2(fd[0], STDIN_FILENO ) == -1) {
                        perror("Trouble redirecting stdin");
                        exit(1);
                    }
                    close(fd[0]);
                    execvp(argv2[0], argv2);
                    perror("execvp in second child failed" );
                    exit(1);

                }
            }
                           
        }

        else{
            execvp(input[0], input);
            perror("exec failed");
            fflush(stdout);
            exit(1);

        }
    }

   

}
