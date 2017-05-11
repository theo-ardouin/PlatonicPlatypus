#include "extract.h"

void extract_as_char(char* str, void* ptr)
{
  *(char*)ptr = atoi(str);
}

void extract_as_str(char* str, void* ptr)
{
  char** nstr = ptr;
  *nstr = strdup(str);
}

void extract_as_int(char* str, void* ptr)
{
  *(int*)ptr = atoi(str);
}

void extract_as_float(char* str, void* ptr)
{
  *(float*)ptr = atof(str);
}

void extract_from_char(char* str, void* ptr)
{
  *(char*)ptr = str[0];
}

char* extract_modulo(char* str)
{
  return (*str == '%' ? ++str : NULL);
}

char* extract_identifier(char* str)
{
  if (!isalpha(*str) && *str != '_')
    return (NULL);
  while (isalpha(*str) || isdigit(*str) || *str == '_')
    ++str;
  return (str);
}

char* extract_number(char* str)
{
  if (*str == '-')
    ++str;
  if (!isdigit(*str))
    return (NULL);
  while (isdigit(*str))
    ++str;
  return (str);
}

char* extract_floating_point(char* str)
{
  char has_period = 0;

  if (!isdigit(*str) && *str != '.')
    return (NULL);
  while (isdigit(*str) || *str == '.')
  {
    has_period = (*str == '.' ? has_period + 1 : has_period);
    if (has_period > 1)
      return (str);
    ++str;
  }
  return (str);
}

struct s_extract* extract_get(char flag)
{
  static struct s_extract flags[] = {
    { 's', 0, { .str = extract_identifier }, extract_as_str },
    { 'd', 0, { .str = extract_number }, extract_as_int },
    { 'k', 0, { .str = extract_number }, extract_as_char },
    { 'f', 0, { .str = extract_floating_point }, extract_as_float },
    { 'a', 1, { .chr = isalpha }, extract_as_str },
    { '_', 1, { .chr = isspace }, extract_as_str },
    { 'l', 1, { .chr = isalnum }, extract_as_str },
    { 'c', 1, { .chr = islower }, extract_as_str },
    { 'C', 1, { .chr = isupper }, extract_as_str },
    { 'g', 1, { .chr = isgraph }, extract_as_str },
    { '.', 1, { .chr = ispunct }, extract_as_str },
    { 'p', 1, { .chr = isprint }, extract_as_str },
    { '%', 0, { .str = extract_modulo }, extract_from_char },
    { 0, 0, { NULL }, NULL }
  };
  for (int i = 0; flags[i].flag; ++i)
  {
    if (flags[i].flag == flag)
      return (&flags[i]);
  }
  return (NULL);
}

char* extract_eval(char* str, struct s_extract* flag)
{
  if (flag->type == 0)
  {
    return (flag->eval.str(str));
  }
  return (str_consume(str, flag->eval.chr));
}

int extract_consume(char** str, ba_t* array, struct s_extract* flag, va_list* ap)
{
  char* ptr = *str;

  ba_clear(array);
  if ((ptr = extract_eval(*str, flag)) == NULL || *str == ptr)
    return (-1);
  if (ba_app(array, *str, ptr - *str) == NULL || ba_app_byte(array, 0) == NULL)
    return (-1);
  *str = ptr;
  if ((ptr = va_arg(*ap, void*)) != NULL && flag->extract)
    flag->extract(array->bytes, ptr);
  return (0);
}

int extract(const char* str, const char* format, ...)
{
  va_list ap;
  ba_t* array;
  struct s_extract* flag;

  if ((array = ba_create(NULL, 0)) == NULL)
    return (-1);
  va_start(ap, format);
  while (*format)
  {
    if (*format == '%' && ++format)
    {
      if ((flag = extract_get(*format)) == NULL || \
        extract_consume((char**)&str, array, flag, &ap) < 0)
        break;
    }
    else
    {
      if (*str != *format)
        break;
      ++str;
    }
    ++format;
  }
  ba_destroy(array);
  va_end(ap);
  return (*str == 0 && *format == 0 ? 0 : -1);
}