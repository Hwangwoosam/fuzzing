#ifndef CGI
#define CGI

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>

char hex_to_dec(char*);
char* cgi_decode(char*str);

#endif