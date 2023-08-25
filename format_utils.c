#include "string.h"

void str_reverse(char* str, int n) {
  for (int i = 0; i < n / 2; ++i) {
    char temp = str[i];
    str[i] = str[n - i - 1];
    str[n - i - 1] = temp;
  }
}

int read_int(const char** sptr) {
  const char* str = *sptr;

  int res = 0;

  while (*str >= '0' && *str <= '9') {
    res *= 10;
    res += *str - '0';
    str += 1;
  }

  *sptr = str;
  return res;
}

int num_len(long long num, int rank) {
  int len = 1;
  if (num > 0) {
    len = 0;
    while (num > 0) {
      num /= rank;
      len += 1;
    }
  }
  return len;
}

const char* pass_space(const char* str) {
  while (*str != '\0' && isspace(*str)) str += 1;
  return str;
}

bool is_rank_digit(char c, int rank) {
  bool res = false;
  if (rank <= RANK_DECIMAL) {
    res = c >= '0' && c <= ('0' + rank - 1);
  } else {
    res = (c >= '0' && c <= '9') || (c >= 'a' && c <= 'f') ||
          (c >= 'A' && c <= 'F');
  }
  return res;
}

int rank_digit(char c) {
  int num = 0;
  if (c >= '0' && c <= '9') {
    num = c - '0';
  } else if (c >= 'a' && c <= 'f') {
    num = c - 'a' + 10;
  } else if (c >= 'A' && c <= 'F') {
    num = c - 'A' + 10;
  }
  return num;
}

const char* scan_char(const char* str, char* cptr, sscanf_params* pm) {
  *cptr = *str++;

  pm->width -= 1;
  for (int i = 0; i < pm->width && str[i] != '\0'; ++i) str += 1;

  return str;
}

const char* scan_str(const char* str, char* sptr, sscanf_params* pm) {
  str = pass_space(str);

  for (char c = *str;
       !isspace(c) && c != '\0' && (pm->width == PARAM_NONE || pm->width > 0);
       c = *++str) {
    if (!pm->ignore) *sptr = c;

    sptr += 1;
    if (pm->width != PARAM_NONE) pm->width -= 1;
  }
  if (!pm->ignore) *sptr = '\0';

  // if (pm->width == 0 && !isspace(*str)) str -= 1;

  return str;
}

const char* scan_int(const char* str, long long* nptr, int rank,
                     sscanf_params* pm) {
  str = pass_space(str);

  int sign = SIGN_PLUS;

  if (*str == '-') {
    sign = SIGN_MINUS;
    str += 1;
    pm->width -= 1;
  } else if (*str == '+') {
    sign = SIGN_PLUS;
    str += 1;
    pm->width -= 1;
  }

  if (pm->width == 0 || !is_rank_digit(*str, rank)) pm->err = true;

  if (!pm->err) {
    if (rank == RANK_HEX && str[0] == '0' && str[1] == 'x' &&
        is_rank_digit(str[2], rank)) {
      pm->width -= 2;
      str += 2;
    }

    long long num = 0;

    while (*str == '0') {
      str += 1;
      pm->width -= 1;
    }

    while (is_rank_digit(*str, rank) && pm->width > 0) {
      num += rank_digit(*str);
      pm->width -= 1;
      str += 1;

      if (is_rank_digit(*str, rank) && pm->width > 0) {
        num *= rank;
      }
    }

    if (sign == SIGN_MINUS) num = -num;
    *nptr = num;
  }

  return str;
}

const char* scan_float(const char* str, long double* fptr, sscanf_params* pm) {
  str = pass_space(str);

  int sign = SIGN_PLUS;

  if (*str == '-') {
    sign = SIGN_MINUS;
    str += 1;
  } else if (*str == '+') {
    sign = SIGN_PLUS;
    str += 1;
  }

  if (!isdigit(*str)) pm->err = true;

  if (!pm->err) {
    long double num = 0.0L;

    int expo = 0;

    bool floating_side = false;
    bool exit = false;

    while (!exit && pm->width != 0) {
      if (!floating_side && *str == '.') {
        floating_side = true;
        num *= 10.0L;
        if (pm->width != PARAM_NONE) {
          expo += 1;
          pm->width -= 1;
        }
        str += 1;
      } else if (isdigit(*str)) {
        num += *str - '0';

        if (floating_side) expo += 1;

        if (pm->width != PARAM_NONE) pm->width -= 1;

        str += 1;
        if (isdigit(*str)) {
          num *= 10.0L;
        }
      } else {
        exit = true;
      }
    }

    for (int i = 0; i < expo; ++i) num /= 10.0L;

    if (str[0] == 'e') {
      str += 1;

      int expo_sign = SIGN_PLUS;
      if (str[0] == '-' || str[0] == '+') {
        expo_sign = *str++ == '-' ? SIGN_MINUS : SIGN_PLUS;
      }

      if (!isdigit(str[0])) pm->err = true;

      expo = read_int(&str);

      if (expo_sign == SIGN_MINUS) expo = -expo;

      if (expo > 0) {
        for (int i = 0; i < expo; ++i) num *= 10.0L;
      } else {
        for (int i = expo; i < 0; ++i) num /= 10.0L;
      }
    }

    if (sign == SIGN_MINUS) num = -num;
    *fptr = num;
  }

  return str;
}

char* print_rank(char* str, int rank, bool bigChar) {
  if (rank != RANK_DECIMAL) *str++ = '0';

  if (rank == RANK_HEX) *str++ = bigChar ? 'X' : 'x';

  return str;
}

char* write_char(char* str, long long sim, sprintf_params* pm) {
  int lendelta = pm->width - 1;

  if (pm->side == SIDE_RIGHT) {
    for (int i = 0; i < lendelta; ++i) *str++ = pm->space_sim;
  }

  *str++ = sim;

  if (pm->side == SIDE_LEFT) {
    for (int i = 0; i < lendelta; ++i) *str++ = SPACE_DEFAULT;
  }

  return str;
}

char* write_int(char* str, long long num, int rank, sprintf_params* pm) {
  char sc = '\0';
  if (pm->signMode != SIGN_NONE) {
    if (num < 0.0L)
      sc = '-';
    else if (pm->signMode == SIGN_PLUS)
      sc = '+';
    else if (pm->signMode == SIGN_SPACE)
      sc = ' ';
  }

  if (rank != RANK_DECIMAL) sc = '\0';

  if (sc != '\0') pm->width -= 1;

  if (num < 0) num = -num;

  int numlen = num_len(num, rank);

  int lendelta = pm->width - numlen;
  if (num == 0 && pm->accuracy == 0) lendelta += 1;

  if (pm->sharp) {
    if (rank == RANK_OCTAL)
      lendelta -= 1;
    else if (rank == RANK_HEX)
      lendelta -= 2;
  }

  int zeros = 0;
  if (pm->accuracy != PARAM_NONE)
    zeros = pm->accuracy - numlen;
  else if (pm->space_sim == SPACE_ZERO && pm->side == SIDE_RIGHT)
    zeros = lendelta;

  if (zeros < 0) zeros = 0;
  lendelta -= zeros;

  if (pm->side == SIDE_RIGHT) {
    for (int i = 0; i < lendelta; ++i) *str++ = SPACE_DEFAULT;
  }

  if (sc != '\0') *str++ = sc;
  if ((pm->sharp && num != 0) || pm->is_ptr) {
    if (rank == RANK_OCTAL) zeros -= 1;
    str = print_rank(str, rank, pm->bigChar);
  }

  for (int i = 0; i < zeros; ++i) *str++ = SPACE_ZERO;

  if (num != 0 || pm->accuracy != 0) {
    char* num_ptr = str;

    do {
      char sim = num % rank;

      if (sim < RANK_DECIMAL) {
        sim += '0';
      } else {
        sim += 'a' - 10;
        if (pm->bigChar) sim -= 'a' - 'A';
      }

      *str++ = sim;

      num /= rank;
    } while (num > 0);

    str_reverse(num_ptr, numlen);
  }

  if (pm->side == SIDE_LEFT) {
    for (int i = 0; i < lendelta; ++i) *str++ = ' ';
  }

  return str;
}

char* write_str(char* res, const char* str, sprintf_params* pm) {
  if (str == NULL) {
    str = "(null)";
  }

  int len = strlen(str);
  if (pm->accuracy != PARAM_NONE && len > pm->accuracy) len = pm->accuracy;
  int lendelta = pm->width - len;

  if (lendelta > 0 && pm->side == SIDE_RIGHT) {
    for (int i = 0; i < lendelta; ++i) *res++ = pm->space_sim;
  }

  for (const char* ptr = str;
       *ptr != '\0' && (pm->accuracy == PARAM_NONE || ptr - str < pm->accuracy);
       ++ptr) {
    *res++ = *ptr;
  }

  if (lendelta > 0 && pm->side == SIDE_LEFT) {
    for (int i = 0; i < lendelta; ++i) *res++ = ' ';
  }

  return res;
}

int float_left_len(long double numl) {
  int len = 0;

  if (numl == 0) len = 1;

  while (numl > 0) {
    len += 1;
    numl = floorl(numl / 10);
  }
  if (len == 0) len = 1;

  return len;
}

char* write_float(char* str, long double num, sprintf_params* pm) {
  char sc = '\0';
  if (num < 0.0L)
    sc = '-';
  else if (pm->signMode == SIGN_PLUS)
    sc = '+';
  else if (pm->signMode == SIGN_SPACE)
    sc = ' ';

  int width = pm->width;

  if (sc != '\0') width -= 1;

  if (num < 0) num = -num;

  long double numl = floorl(num);

  int numl_len = float_left_len(numl);

  int len = numl_len;
  if (pm->accuracy > 0) len += pm->accuracy;
  if (pm->accuracy > 0 || pm->sharp) len += 1;

  for (int i = 0; i < pm->accuracy; i++) num *= 10;
  num = roundl(num);

  int lendelta = width - len;

  if (sc != '\0' && pm->space_sim == SPACE_ZERO) *str++ = sc;

  if (lendelta > 0 && pm->side == SIDE_RIGHT) {
    for (int i = 0; i < lendelta; ++i) *str++ = pm->space_sim;
  }

  if (sc != '\0' && pm->space_sim == SPACE_DEFAULT) *str++ = sc;

  char* nptr = str;

  if (pm->accuracy > 0) {
    for (int i = 0; i < pm->accuracy; i++) {
      *str++ = (int)fmodl(num, 10) + '0';
      num = floorl(num / 10);
    }
  }

  if (pm->accuracy > 0 || pm->sharp) *str++ = '.';

  for (int i = 0; i < numl_len; i++) {
    *str++ = (int)fmodl(num, 10) + '0';
    num = floorl(num / 10);
  }

  str_reverse(nptr, len);

  if (lendelta > 0 && pm->side == SIDE_LEFT) {
    for (int i = 0; i < lendelta; ++i) *str++ = SPACE_DEFAULT;
  }

  return str;
}

int calc_expo(long double* nump) {
  int expo = 0;
  long double num = *nump;

  long double mul = 10.0L;
  int inc = -1;

  if (num >= 10.0L) {
    mul = 0.1L;
    inc = 1;
  }

  if (num != 0.0L) {
    while (num < 1 || num >= 10) {
      num *= mul;
      expo += inc;
    }
  }

  *nump = num;
  return expo;
}

char* write_expo(char* str, long double num, sprintf_params* pm) {
  bool has_sign = false;
  if (num < 0) {
    has_sign = true;
    num = -num;
  }

  int expo = calc_expo(&num);

  int expo_width = 4;
  if (expo >= 100) expo_width += 1;

  pm->width -= expo_width;
  if (pm->width < 0) pm->width = PARAM_NONE;

  if (has_sign) num = -num;

  int side = pm->side;

  if (side == SIDE_LEFT) pm->side = PARAM_NONE;

  char* nptr = str;
  str = write_float(str, num, pm);
  int writed = str - nptr;

  nptr = str;

  char expo_sign = expo < 0 ? '-' : '+';
  if (expo < 0) expo = -expo;

  int e = expo;
  do {
    *str++ = e % 10 + '0';
    e /= 10;
  } while (e > 0);

  if (expo < 10) *str++ = '0';

  *str++ = expo_sign;
  *str++ = pm->bigChar ? 'E' : 'e';

  str_reverse(nptr, expo_width);

  if (side == SIDE_LEFT) {
    int lendelta = pm->width - writed;
    if (lendelta < 0) lendelta = 0;

    for (int i = 0; i < lendelta; ++i) *str++ = SPACE_DEFAULT;
  }

  return str;
}

int end_zeros_count(long double num, int accuracy) {
  for (int i = 0; i < accuracy; ++i) num *= 10.0L;

  int count = 0;

  if (num == 0.0L) count = 1000000;

  num = roundl(num);

  bool pass = true;

  while (num > 0.0L && pass) {
    pass = fmodl(num, 10) == 0.0L;
    if (pass) {
      count += 1;
      num /= 10.0L;
    }
  }

  return count;
}

char* write_g(char* str, long double num, sprintf_params* pm) {
  if (isinf(num)) {
    str = write_str(str, "inf", pm);
  } else if (isnan(num)) {
    str = write_str(str, "nan", pm);
  } else {
    bool has_sign = num < 0.0L;
    if (has_sign) num = -num;

    long double expo_num = num;
    int expo = calc_expo(&expo_num);

    int numl_len = float_left_len(num);

    if (pm->accuracy > expo && expo > -4) {
      pm->accuracy -= numl_len;
      if (pm->sharp) pm->accuracy = 0;

      if (pm->accuracy > 0) pm->accuracy -= end_zeros_count(num, pm->accuracy);

      if (pm->accuracy < 0) pm->accuracy = PARAM_NONE;

      if (has_sign) num = -num;
      str = write_float(str, num, pm);
    } else {
      pm->accuracy -= 1;

      if (pm->accuracy < 0) pm->accuracy = PARAM_NONE;

      if (has_sign) num = -num;
      str = write_expo(str, num, pm);
    }
  }

  return str;
}
