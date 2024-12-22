

#include <string.h>
#include <stdlib.h>
#include <signal.h>
#include <sys/time.h>
#include <unistd.h>
#include "morse_code.h"
#include <fcntl.h>
#include <stdio.h>
#include <assert.h>
#include <sys/types.h>

#define BUFFER_SIZE 1024


//the sender program



//Sending the morse code to the parent using signals
void send_morse(const char *morse_code, pid_t parent_pid){
    kill(parent_pid, SIGCONT);

    if (morse_code == NULL || morse_code[0] == '\0') {
        fprintf(stderr, "Warning: Unsupported character. Skipping.\n");
        return;  // Exit without sending any signal
    }
    kill(parent_pid, SIGCONT);
    
    for (int i=0; morse_code[i] !='\0';i++){
  
        if(morse_code[i]=='.'){
            kill(parent_pid,SIGUSR1);
        }
        else if (morse_code[i]=='-'){
            kill(parent_pid,SIGUSR2);
        } 
        

        
        usleep(17000);  // Small delay between signals 
      
        
    }

    // End of character signal sent the A process(SIGALRM)
    kill(parent_pid, SIGALRM);
  
    usleep(17000);  // Small delay between characters
   
}
   




int main(int argc, char **argv){

    int ifd;
    pid_t parent_pid = getppid();

    // Check command line arguments for input file
    if (argc == 2) {
        if (strcmp(argv[1], "-") == 0) {
            ifd = STDIN_FILENO; // Use standard input
        } else {
            ifd = open(argv[1], O_RDONLY); // Open specified input file
            if (ifd < 0) {
                fprintf(stderr, "Opening input file (%s) failed\n", argv[1]);
                return -1; // Error opening file
            }
        }
    } else {
        fprintf(stderr, "Usage: %s [input|-]\n", argv[0]);
        return -1; // Incorrect usage
    }




   
    char buffer[BUFFER_SIZE];
    ssize_t bytesToRead;

    //reads the file and encodes and sends morse

    while ((bytesToRead= read(ifd, buffer, BUFFER_SIZE)) > 0) {
     for (ssize_t i = 0; i < bytesToRead; i++) {
        char ch = buffer[i];

        
        //if the word ended send SIGINT
        if (ch ==' ') {
            kill(parent_pid, SIGINT);
            usleep(9000);  
            continue;  
        }
        
       
        //encode character and send a corresponding signal
        const char *morse_code = encode_morse(ch);  
        if (morse_code && strlen(morse_code) > 0) {
            send_morse(morse_code, parent_pid);
            
            
           
        }
        
    }
    break;
}

    //end of transmission for program a
    kill(getppid(), SIGQUIT);
    close(ifd);
    return 0;

}