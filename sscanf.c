#include "string.h"

sscanf_params default_sscanf_params() {
  return (sscanf_params){
      .nextIsFormat = false,
      .width = PARAM_NONE,
      .err = false,
      .ignore = false,
      .length = LENGTH_DEFAULT,
      .ignore_flag = false,
  };
}

void clear_sscanf_params(sscanf_params* pm) {
  pm->nextIsFormat = false;
  pm->width = PARAM_NONE;
  pm->length = LENGTH_DEFAULT;
  pm->ignore = false;
  pm->ignore_flag = false;
}

int sscanf(const char* str, const char* format, ...) {
  va_list va;
  va_start(va, format);

  int count = 0;

  sscanf_params pm = default_sscanf_params();

  const char* sptr = str;
  const char* fptr = format;

  const char* nptr = NULL;

  for (; *fptr != '\0' && count >= 0; ++fptr) {
    char fc = *fptr;

    if (pm.nextIsFormat) {
      if (fc == '*') {
        pm.ignore = true;
      } else if (isdigit(fc)) {
        pm.width = read_int(&fptr);
        fptr -= 1;
      } else if (fc == 'h') {
        pm.length = LENGTH_SHORT;
      } else if (fc == 'l') {
        pm.length = LENGTH_LONG;
      } else if (fc == 'L') {
        pm.length = LENGTH_LONG_DOUBLE;
      } else {
        if (*sptr == '\0' && fc != '%' && fc != 'n') {
          count = EOF;
          continue;
        }

        switch (fc) {
          case 'c':;
            char sim = '\0';
            sptr = scan_char(sptr, &sim, &pm);
            if (!pm.ignore) {
              *(va_arg(va, char*)) = sim;
              count += 1;
            }
            break;

          case 's':
            if (pm.width == 0) pm.width = PARAM_NONE;

            char* s_argptr = NULL;
            if (!pm.ignore) {
              s_argptr = va_arg(va, char*);
            }

            sptr = scan_str(sptr, s_argptr, &pm);

            if (!pm.ignore && !pm.err) count += 1;
            break;

          case 'd':
          case 'i':;
            if (pm.width <= 0) pm.width = INT16_MAX;

            int drank = RANK_DECIMAL;

            if (fc == 'i' && sptr[0] == '0') {
              if (sptr[1] == 'x' || sptr[1] == 'X') {
                drank = RANK_HEX;
              } else {
                drank = RANK_OCTAL;
              }
            }

            long long dnum = 0;
            sptr = scan_int(sptr, &dnum, drank, &pm);

            if (!pm.ignore) {
              if (pm.length == LENGTH_SHORT) {
                short* ptr = va_arg(va, short*);
                *ptr = (short)dnum;
              } else if (pm.length == LENGTH_LONG) {
                long* ptr = va_arg(va, long*);
                *ptr = (long)dnum;
              } else {
                int* ptr = va_arg(va, int*);
                *ptr = (int)dnum;
              }
            }

            if (!pm.ignore && !pm.err) count += 1;
            break;

          case 'u':
          case 'x':
          case 'X':
          case 'o':
          case 'p':;
            if (pm.width <= 0) pm.width = INT16_MAX;

            int rank = RANK_DECIMAL;

            if (fc == 'x' || fc == 'X' || fc == 'p') {
              rank = RANK_HEX;
            } else if (fc == 'o') {
              rank = RANK_OCTAL;
            }
            long long unum = 0;
            sptr = scan_int(sptr, &unum, rank, &pm);

            if (!pm.ignore) {
              if (pm.length == LENGTH_SHORT) {
                unsigned short* argptr = va_arg(va, unsigned short*);
                *argptr = unum;
              } else if (pm.length == LENGTH_LONG) {
                unsigned long* argptr = va_arg(va, unsigned long*);
                *argptr = unum;
              } else {
                unsigned int* argptr = va_arg(va, unsigned int*);
                *argptr = unum;
              }
            }

            if (!pm.ignore && !pm.err) count += 1;
            break;

          case 'n':;
            int* argptr = va_arg(va, int*);
            *argptr = nptr - str;
            break;

          case 'e':
          case 'E':
          case 'g':
          case 'G':
          case 'f':;
            if (pm.width == 0) pm.width = PARAM_NONE;

            long double num = 0.0L;
            sptr = scan_float(sptr, &num, &pm);

            if (!pm.ignore) {
              if (pm.length == LENGTH_LONG_DOUBLE) {
                long double* argptr = va_arg(va, long double*);
                *argptr = (long double)num;
              } else if (pm.length == LENGTH_LONG) {
                double* argptr = va_arg(va, double*);
                *argptr = (double)num;
              } else {
                float* argptr = va_arg(va, float*);
                *argptr = (float)num;
              }
            }

            if (!pm.ignore && !pm.err) count += 1;
            break;

          case '%':;
            break;

          default:
            pm.err = true;
            break;
        }

        if (pm.err && *sptr == '\0') {
          count = EOF;
        }

        clear_sscanf_params(&pm);
      }
    } else if (fc == '%') {
      pm.nextIsFormat = true;
      nptr = sptr;
    } else if (isspace(fc) && isspace(*sptr)) {
      sptr = pass_space(sptr);
    } else if (fc == *sptr) {
      sptr += 1;
    }
  }
  va_end(va);

  return count;
}
