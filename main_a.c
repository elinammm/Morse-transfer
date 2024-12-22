#define _POSIX_C_SOURCE 202009L

#include <string.h>
#include <stdlib.h>
#include "morse_code.h"
#include <stdio.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <assert.h>
#include <signal.h>

#define BLOCKSIZE 4096
#define LINE_BUFFER_SIZE 1050

//receiving program


//used the signal-better.c as a reference for the signal handler
static int sigpipe = 0;
static char *receiving_buffer = NULL;    // Buffer for receiving Morse code is stored
static size_t buffer_index = 0; //Index 
static char *line_buffer = NULL;  // Buffer for accumulating the line
static size_t line_len = 0; 




static void sig_usr(int signo) {
	char ret = signo;
	ssize_t bytes_written = write(sigpipe, &ret, 1); // Write signal to pipe

    if (bytes_written < 0) {
        perror("Write failed");
    }
    

}




///as1_mockup.c used as a reference

int main(int argc, char **argv){
    int ifd, ofd;

   
    //if the input contains one argument, output stdout
    if (argc==2){
        ofd=STDOUT_FILENO;
        //standard input
        if (strcmp(argv[1],"-")==0){
            ifd=STDIN_FILENO;
          } 
        else {
            ifd = open(argv[1],O_RDONLY);
            if (ifd < 0) {
                fprintf(stderr,"Opening input file (%s) failed\n",argv[1]);
                return -1;
            }
        }
    } 
     else if (argc == 3) { // Both input and output file given
        if (strcmp(argv[1],"-") == 0) {
            ifd = STDIN_FILENO;
        } else {
            ifd = open(argv[1],O_RDONLY);
            if (ifd < 0) {
                fprintf(stderr,"Opening input file (%s) failed\n",argv[1]);
                return -1;
            }
        }
        if (strcmp(argv[2],"-") == 0) {
            ofd = STDOUT_FILENO;
        } else {
            ofd = open(argv[2],O_WRONLY|O_CREAT|O_TRUNC,0644);
            if (ofd < 0) {
                fprintf(stderr,"Creating output file (%s) failed\n",argv[2]);
                return -1;
            }
        }
    } 
    else {
        fprintf(stderr,"Usage: %s [input|-] [output|-]\n",argv[0]);
        return -1;
    }

    //char ch;
    //size_t bytes_read;

    //printf("Reading characters from the input file:\n");
    //while ((bytes_read = read(ifd, &ch, 1)) > 0) {
     //printf("Character read: '%c'\n", ch);
    //}

    //The signal handling 


    //Used the signal-better.c as a reference


    
    struct sigaction sig;
	int pipefd[2];

	assert(pipe(pipefd) == 0);
	sigpipe = pipefd[1];

    //Signal handling
	sigemptyset(&sig.sa_mask);
  	sig.sa_flags= SA_RESTART;
	sig.sa_handler = sig_usr;

	assert((sigaction(SIGUSR1,&sig,NULL)) == 0);  //short
	assert((sigaction(SIGUSR2,&sig,NULL)) == 0); //long
    assert((sigaction(SIGALRM, &sig, NULL)) == 0); //after character
    assert((sigaction(SIGINT, &sig, NULL))==0); //after word (space needed)
    assert((sigaction(SIGQUIT, &sig, NULL))==0); //end of transmission
    assert((sigaction(SIGTERM, &sig, NULL)) == 0); // End of line


    //Creating the child process using fork() 
    pid_t pid;  
    pid=fork();

       if (pid == 0) {  // Child process
        char *arguments[] = {"./main_b", argv[1], NULL};
        execvp(arguments[0], arguments);
        perror("execvp");
        return -1;

    } else if (pid > 0) {  // Parent process
        int signal;
        line_buffer = malloc(LINE_BUFFER_SIZE);

        if (line_buffer == NULL) {
            perror("Failed to allocate line buffer");
            exit(EXIT_FAILURE);
        }
       

        while (1) {
            int res = read(pipefd[0], &signal, 1);
            if (res == 1) {
                if (signal == SIGUSR1 || signal == SIGUSR2) { //dot or dash
                    char *temp = realloc(receiving_buffer, buffer_index + 2);
                    if (temp == NULL) {
                        perror("Failed to realloc memory");
                        free(receiving_buffer);
                        free(line_buffer);
                        exit(EXIT_FAILURE);
                    }
                    receiving_buffer = temp;
                    receiving_buffer[buffer_index++] = (signal == SIGUSR1) ? '.' : '-';
                    receiving_buffer[buffer_index] = '\0';
                   

                } else if (signal == SIGALRM) { //end of character
                    if (receiving_buffer != NULL) {
                        receiving_buffer[buffer_index] = '\0';
                        char decoded_char = decode_morse(receiving_buffer);
                        if (decoded_char != '\0') {
                            if (line_len < LINE_BUFFER_SIZE - 1) {
                                line_buffer[line_len++] = decoded_char;
                            }
                        }
                        free(receiving_buffer);
                        receiving_buffer = NULL;
                        buffer_index = 0;
                    }

                } else if (signal == SIGINT) { // end of word
                    if (line_len < LINE_BUFFER_SIZE - 1) {
                        line_buffer[line_len++] = ' ';
                    }

                } else if (signal == SIGTERM) { // end of line
                    if (line_len < LINE_BUFFER_SIZE - 1) {
                        line_buffer[line_len++] = '\n';
                    }

                } else if (signal == SIGQUIT) { // end of transmission
                    if (line_len > 0) {
                        ssize_t bytes_written = write(ofd, line_buffer, line_len);
                        if (bytes_written < 0) {
                            perror("Failed to write final buffer to file");
                        }
                    }
                    free(receiving_buffer);
                    free(line_buffer);
                    close(ofd);
                    break;
                }
            }
        }
    } else {
        perror("fork error");
        free(receiving_buffer);
        free(line_buffer);
        exit(1);
    }

    return 0;
}