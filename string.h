#ifndef STRING_H_
#define STRING_H_

#include <ctype.h>
#include <math.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

typedef unsigned long long size_t;

#define NULL (void *)0

// -- sprintf params --
#define SIGN_NONE 0
#define SIGN_MINUS 1
#define SIGN_PLUS 2
#define SIGN_SPACE 3

#define SIDE_LEFT 1
#define SIDE_RIGHT 2

#define PARAM_NONE -1

#define LENGTH_DEFAULT 0
#define LENGTH_SHORT 1
#define LENGTH_LONG 2
#define LENGTH_LONG_DOUBLE 3

#define SPACE_DEFAULT ' '
#define SPACE_ZERO '0'

#define PARAM_ARG '*'

#define RANK_DECIMAL 10
#define RANK_OCTAL 8
#define RANK_HEX 16

typedef struct {
  bool printChar;

  bool nextIsFormat;

  int signMode;
  int width;
  int accuracy;
  int side;
  int length;
  char space_sim;
  bool sharp;
  bool bigChar;
  bool is_ptr;
} sprintf_params;

typedef struct {
  bool nextIsFormat;
  int length;
  int width;
  bool ignore;
  bool err;
  bool ignore_flag;
} sscanf_params;

sprintf_params default_sprintf_params();
sscanf_params default_sscanf_params();

void clear_sprintf_params(sprintf_params *pm);
void clear_sscanf_params(sscanf_params *pm);
// --------------------

// ----- Part 2 -----
int sprintf(char *str, const char *format, ...);
// ------------------

int read_int(const char **sptr);

const char *scan_int(const char *str, long long *nptr, int rank,
                     sscanf_params *pm);

const char *scan_float(const char *str, long double *fptr, sscanf_params *pm);

const char *scan_char(const char *str, char *cptr, sscanf_params *pm);

const char *scan_str(const char *str, char *sptr, sscanf_params *pm);

const char *pass_space(const char *str);

char *write_char(char *str, long long sim, sprintf_params *pm);

char *write_int(char *str, long long num, int rank, sprintf_params *pm);

char *write_str(char *res, const char *str, sprintf_params *pm);

char *write_float(char *str, long double num, sprintf_params *pm);

char *write_expo(char *str, long double num, sprintf_params *pm);

char *write_g(char *str, long double num, sprintf_params *pm);

void str_reverse(char *str, int n);
int num_len(long long num, int rank);
const char *pass_space(const char *str);
bool is_rank_digit(char c, int rank);
int rank_digit(char c);
char *print_rank(char *str, int rank, bool bigChar);
int float_left_len(long double numl);
int end_zeros_count(long double num, int accuracy);

void *memchr(const void *str, int c, size_t n);
int memcmp(const void *str1, const void *str2, size_t n);
void *memcpy(void *dest, const void *src, size_t n);
void *memmove(void *dest, const void *src, size_t n);
void *memset(void *str, int c, size_t n);
char *strcat(char *dest, const char *src);
char *strncat(char *dest, const char *src, size_t n);
char *strchr(const char *str, int c);
int strcmp(const char *str1, const char *str2);
int strncmp(const char *str1, const char *str2, size_t n);
char *strcpy(char *dest, const char *src);
char *strncpy(char *dest, const char *src, size_t n);
size_t strcspn(const char *str1, const char *str2);
char *strerror(int errnum);
size_t strlen(const char *str);
char *strpbrk(const char *str1, const char *str2);
char *strrchr(const char *str, int c);
size_t strspn(const char *str1, const char *str2);
char *strstr(const char *haystack, const char *needle);
char *strtok(char *str, const char *delim);

int sscanf(const char *str, const char *format, ...);
int sprintf(char *str, const char *format, ...);

void *to_upper(const char *str);
void *to_lower(const char *str);
void *insert(const char *src, const char *str, size_t start_index);
void *trim(const char *src, const char *trim_chars);

#endif  // STRING_H_
