#include "str_utils.h"

int str_count(char* str, char c)
{
  int count = 0;
  for (int i = 0; str[i]; ++i)
  {
    if (str[i] == c)
      ++count;
  }
  return (count);
}

char* str_consume(char* str, str_ctype_f contains)
{
  int i = 0;
  while (contains(str[i]))
    ++i;
  return (&str[i]);
}

char* str_trim(char* str)
{
  int starts = 0;
  int ends = strlen(str) - 1;

  while (isspace(str[starts]))
    ++starts;
  while (isspace(str[ends]))
    --ends;
  memmove(str, str + starts, ends);
  str[ends - starts + 1] = 0;
  return (str);
}

char* str_tokenize(char* str, char c)
{
  int j = 0;
  str = str_trim(str);
  for (int i = 0; str[i];)
  {
    if (isspace(str[i]))
    {
      str[j++] = c;
      while (isspace(str[i]))
        ++i;
    }
    else
      str[j++] = str[i++];
  }
  str[j] = 0;
  return (str);
}

char** str_split(char* str, char c, int* size)
{
  int i = 0;
  char** array;
  char* tmp = str;

  *size = str_count(str, c) + 1;
  if ((array = malloc(sizeof(*array) * (*size))) == NULL)
    return (NULL);
  while (*str)
  {
    if (*str == c)
    {
      array[i++] = tmp;
      *(str++) = 0;
      tmp = str;
    }
    else
      ++str;
  }
  array[i] = tmp;
  return (array);
}