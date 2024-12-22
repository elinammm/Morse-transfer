#ifndef _morse_h_
#define _morse_h_
#include <string.h>

//a library for encoding and decoding morse



//function to encode all characters
const char *encode_morse(char letter);

//function to decode morse
char decode_morse(const char *morse);



#endif