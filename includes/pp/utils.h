#ifndef PP_UTILS_H_
#define PP_UTILS_H_

#include <stdlib.h>

/*
  @return       Pointer to the first occurence from haystack or NULL if not found
  @description  Finds the first occurrence of needle in haystack
*/
void* bytesfirst(const void* haystack, size_t hsize, const void* needle, size_t nsize);

/*
  @return       dest
  @description  Copies the string from src to dest, including \0
                If src is greater than maxlen, dest will be truncated
*/
char* strset(char* dest, size_t maxlen, const char* src);

#endif
