#include "string.h"

#define PRINTF_ERR -1

#define DEFAULT_ACCURACY 6

void swap_accuracy_to_width_for_digit(sprintf_params* pm);

void clear_sprintf_params(sprintf_params* pm) {
  pm->nextIsFormat = false;
  pm->accuracy = PARAM_NONE;
  pm->width = PARAM_NONE;
  pm->side = SIDE_RIGHT;
  pm->signMode = SIGN_MINUS;
  pm->length = LENGTH_DEFAULT;
  pm->space_sim = ' ';
  pm->sharp = false;
  pm->bigChar = false;
  pm->is_ptr = false;
}

sprintf_params default_sprintf_params() {
  return (sprintf_params){false,      false,      SIGN_MINUS,     PARAM_NONE,
                          PARAM_NONE, SIDE_RIGHT, LENGTH_DEFAULT, SPACE_DEFAULT,
                          false,      false,      false};
}

void swap_accuracy_to_width_for_digit(sprintf_params* pm) {
  if (pm->width == PARAM_NONE && pm->accuracy != PARAM_NONE) {
    pm->width = pm->accuracy;
  }
}

int sprintf(char* str, const char* format, ...) {
  va_list va;
  va_start(va, format);

  int count = 0;

  // Указатели на str, format для чтения, заполнения строк
  const char* fptr = format;
  char* sptr = str;

  char* nptr = NULL;

  sprintf_params pm = default_sprintf_params();

  for (; *fptr != '\0' && count >= 0; ++fptr) {
    char fc = *fptr;

    if (pm.nextIsFormat) {
      if (fc == '-') {
        pm.side = SIDE_LEFT;
      } else if (fc == '+') {
        pm.signMode = SIGN_PLUS;
      } else if (fc == ' ') {
        if (pm.signMode != SIGN_PLUS) pm.signMode = SIGN_SPACE;
      } else if (fc == '#') {
        pm.sharp = true;
      } else if (fc == SPACE_ZERO) {
        pm.space_sim = SPACE_ZERO;
      } else if (fc == PARAM_ARG) {
        pm.width = va_arg(va, int);
      } else if (isdigit(fc)) {
        pm.width = read_int(&fptr);
        fptr -= 1;
      } else if (fc == '.') {
        fc = *++fptr;
        if (fc == PARAM_ARG) {
          pm.accuracy = va_arg(va, int);
        } else if (isdigit(fc)) {
          pm.accuracy = read_int(&fptr);
          fptr -= 1;
        } else {
          fptr -= 1;
          pm.accuracy = 0;
        }
      } else if (fc == 'h') {
        pm.length = LENGTH_SHORT;
      } else if (fc == 'l') {
        pm.length = LENGTH_LONG;
      } else if (fc == 'L') {
        pm.length = LENGTH_LONG_DOUBLE;
      } else {
        switch (fc) {
          case 'c':
            if (pm.length == LENGTH_SHORT)
              sptr = write_char(sptr, (short)va_arg(va, int), &pm);
            else if (pm.length == LENGTH_LONG)
              sptr = write_char(sptr, va_arg(va, long), &pm);
            else
              sptr = write_char(sptr, va_arg(va, int), &pm);
            break;

          case 'd':
          case 'i':;
            swap_accuracy_to_width_for_digit(&pm);
            if (pm.length == LENGTH_SHORT)
              sptr = write_int(sptr, (short)va_arg(va, int), RANK_DECIMAL, &pm);
            else if (pm.length == LENGTH_LONG)
              sptr = write_int(sptr, va_arg(va, long), RANK_DECIMAL, &pm);
            else
              sptr = write_int(sptr, va_arg(va, int), RANK_DECIMAL, &pm);
            break;

          case 'n':;
            int nprinted = nptr - str;
            int* ptr = va_arg(va, int*);
            *ptr = nprinted;
            break;

          case 'p':
            pm.is_ptr = true;
            pm.sharp = true;
            if (sizeof(void*) == 32)
              sptr = write_int(sptr, va_arg(va, int32_t), RANK_HEX, &pm);
            else
              sptr = write_int(sptr, va_arg(va, int64_t), RANK_HEX, &pm);
            break;

          case 'o':
          case 'x':
          case 'X':
          case 'u':;
            swap_accuracy_to_width_for_digit(&pm);
            pm.signMode = SIGN_NONE;

            int rank = RANK_DECIMAL;

            if (fc == 'o') {
              rank = RANK_OCTAL;
            } else if (fc == 'x' || fc == 'X') {
              rank = RANK_HEX;
              pm.bigChar = fc == 'X';
            }

            if (pm.length == LENGTH_SHORT)
              sptr = write_int(sptr, (unsigned short)va_arg(va, unsigned int),
                               rank, &pm);
            else if (pm.length == LENGTH_LONG)
              sptr = write_int(sptr, va_arg(va, unsigned long), rank, &pm);
            else
              sptr = write_int(sptr, va_arg(va, unsigned int), rank, &pm);
            break;

          case 'f':
            if (pm.accuracy == PARAM_NONE) pm.accuracy = DEFAULT_ACCURACY;
            if (pm.length == LENGTH_LONG_DOUBLE)
              sptr = write_float(sptr, va_arg(va, long double), &pm);
            else
              sptr = write_float(sptr, va_arg(va, double), &pm);
            break;

          case 'g':
          case 'G':;
            if (pm.accuracy == PARAM_NONE)
              pm.accuracy = DEFAULT_ACCURACY;
            else if (pm.accuracy == 0)
              pm.accuracy = 1;

            if (fc == 'G') pm.bigChar = true;

            if (pm.length == LENGTH_LONG_DOUBLE)
              sptr = write_g(sptr, va_arg(va, long double), &pm);
            else
              sptr = write_g(sptr, va_arg(va, double), &pm);
            break;

          case 'e':
          case 'E':;
            if (pm.accuracy == PARAM_NONE) pm.accuracy = DEFAULT_ACCURACY;

            if (fc == 'E') pm.bigChar = true;

            if (pm.length == LENGTH_LONG_DOUBLE)
              sptr = write_expo(sptr, va_arg(va, long double), &pm);
            else
              sptr = write_expo(sptr, va_arg(va, double), &pm);
            break;

          case 's':;
            if (pm.length != LENGTH_DEFAULT) count = -1;
            const char* str = va_arg(va, char*);
            sptr = write_str(sptr, str, &pm);
            break;

          case '%':
            pm.printChar = true;
            break;
        }

        clear_sprintf_params(&pm);
      }

    } else if (fc == '%') {
      nptr = sptr;
      pm.nextIsFormat = true;
    } else {
      pm.printChar = true;
    }

    if (pm.printChar) {
      *sptr++ = fc;
      pm.printChar = false;
    }
  }

  *sptr = '\0';

  va_end(va);

  if (count >= 0) {
    count = sptr - str;
  } else {
    *str = '\0';
  }

  return count;
}
