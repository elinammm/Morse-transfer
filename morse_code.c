#include <stdio.h>
#include <ctype.h>
#include <string.h>


#include "morse_code.h"


//Morse code for A-Z
static const char *alphabet[]={
     ".-",   // A
    "-...", // B
    "-.-.", // C
    "-..",  // D
    ".",    // E
    "..-.", // F
    "--.",  // G
    "....", // H
    "..",   // I
    ".---", // J
    "-.-",  // K
    ".-..", // L
    "--",   // M
    "-.",   // N
    "---",  // O
    ".--.", // P
    "--.-", // Q
    ".-.",  // R
    "...",  // S
    "-",    // T
    "..-",  // U
    "...-", // V
    ".--",  // W
    "-..-", // X
    "-.--", // Y
    "--.."  // Z
 };

//Morse code for 1-9
 static const char *nums[]={
    "-----", ".----", "..---", "...--","....-",".....","-....","--...",
    "---..", "----."
 };

//Morse for punctuation marks
static const char *punctuation[]={
    ".-...",   // &
    ".----.",  // '
    ".--.-.",  // @
    "-.--.-",  // )
    "-.--.",   // (
    "---...",  // :
    "--..--",  // ,
    "-...-",   // =
    "-.-.--",  // !
    ".-.-.-",  // .
    "-....-",  // -
    ".-.-.",   // +
    ".-..-.",  // "
    "..--..",  // ?
    "-..-."    // /
 };

//Equivalent punctuation marks
static const char punctuation_symbols[]={
    '&', '\'', '@', ')', '(', ':', ',', '=', '!', '.', '-', '+', '"', '?', '/'
    };

//Morse code for new line prosign

static const char *newline="-.-";

//a function for encoding characters into morse

const char *encode_morse(char letter) {
    //Check whether the character is an alphabet
    if (isalpha(letter)) {
        letter = toupper(letter); // Convert to uppercase
        //printf("Encoding letter '%c' to Morse: %s\n", letter, alphabet[letter - 'A']);
        return alphabet[letter - 'A'];
    }
    // Check for numbers (0-9)
    else if (isdigit(letter)) {
        return nums[letter - '0']; // Get Morse for the digit
    } 

    // Check if the character is a supported punctuation mark
    else {
        for (int i = 0; i < sizeof(punctuation_symbols)/sizeof(punctuation_symbols[0]); i++) {
            if (letter == punctuation_symbols[i]) {
                return punctuation[i]; // Get Morse code from punctuation array
            }
        }
    }

    // Check if the character is a newline
    if (letter == '\n') {
        return newline;
    }

    // otherwise return NULL
    //return (const char *)NULL;
    return NULL;
}  

//a function for decoding morse into a character

char decode_morse(const char *morse) {
    // Decode alphabet letters (A-Z)
    for (int i = 0; i < 26; i++) {
        if (strcmp(morse, alphabet[i]) == 0) {
            return 'A' + i;  // Convert index to uppercase letter
        }
    }

    // Decode numbers (0-9)
    for (int i = 0; i < 10; i++) {
        if (strcmp(morse, nums[i]) == 0) {
            return '0' + i;  
        }
    }

    // Decode punctuation
    for (int i = 0; i < sizeof(punctuation_symbols)/sizeof(punctuation_symbols[0]); i++) {
        if (strcmp(morse, punctuation[i]) == 0) {
            return punctuation_symbols[i]; 
           
        }
    }

    // Decode newline
    if (strcmp(morse, newline) == 0) {
        return '\n';  
    }

    //return null character
    return '\0';
}