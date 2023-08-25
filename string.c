#include "string.h"

char *strcat(char *dest, const char *src) {
  size_t i, k;
  for (i = 0; dest[i] != '\0'; i++)
    ;
  for (k = 0; src[k] != '\0'; k++) dest[i + k] = src[k];
  dest[i + k] = '\0';

  return dest;
}

char *strncat(char *dest, const char *src, size_t n) {
  size_t i, k;
  for (i = 0; dest[i] != '\0'; i++)
    ;
  for (k = 0; k != n; k++) dest[i + k] = src[k];
  dest[i + k] = '\0';

  return dest;
}

char *strchr(const char *str, int c) {
  size_t i = 0;
  if (str != NULL) {
    while (str[i] && str[i] != c) {
      i++;
    }
    if (str[i] == c)
      str = str + i;
    else
      str = NULL;
  } else
    str = NULL;
  return (char *)str;
}

int strcmp(const char *str1, const char *str2) {
  int size = 0;
  int flag = 0;
  size_t i = 0;
  while (flag == 0) {
    if (str1[i] != str2[i]) {
      size = str1[i] - str2[i];
      flag = 1;
    }
    if (str1[i] == '\0' && str2[i] == '\0') {
      flag = 1;
    }
    i++;
  }
  return size;
}

int strncmp(const char *str1, const char *str2, size_t n) {
  int size = 0;
  int flag = 0;
  size_t i = 0;
  if (n > 0) {
    while (flag == 0) {
      if (str1[i] != str2[i]) {
        size = str1[i] - str2[i];
        flag = 1;
      }
      if (str1[i] == '\0' && str2[i] == '\0') flag = 1;
      i++;
      if (i == n) flag = 1;
    }
  }
  return size;
}

char *strcpy(char *dest, const char *src) {
  if (0 == strlen(src)) {
    dest[0] = '\0';
  } else {
    for (size_t i = 0; i < strlen(src); i++) {
      dest[i] = src[i];
      dest[i + 1] = '\0';
    }
  }
  return dest;
}

char *strncpy(char *dest, const char *src, size_t n) {
  for (size_t i = 0; i < n; i++) {
    dest[i] = src[i];
  }
  return dest;
}

size_t strcspn(const char *str1, const char *str2) {
  size_t len = 0;
  int bool_break = 0;
  while (len < strlen(str1) && bool_break == 0) {
    for (size_t k = 0; k < strlen(str2); k++) {
      if (str1[len] == str2[k]) {
        bool_break = 1;
        len--;
      }
    }
    len++;
  }
  return len;
}

char *strerror(int errnum) {
#if defined(__linux__)  // Or #if __linux__
  char arr_error[135][256] = {
      "Success",
      "Operation not permitted",
      "No such file or directory",
      "No such process",
      "Interrupted system call",
      "Input/output error",
      "No such device or address",
      "Argument list too long",
      "Exec format error",
      "Bad file descriptor",
      "No child processes",
      "Resource temporarily unavailable",
      "Cannot allocate memory",
      "Permission denied",
      "Bad address",
      "Block device required",
      "Device or resource busy",
      "File exists",
      "Invalid cross-device link",
      "No such device",
      "Not a directory",
      "Is a directory",
      "Invalid argument",
      "Too many open files in system",
      "Too many open files",
      "Inappropriate ioctl for device",
      "Text file busy",
      "File too large",
      "No space left on device",
      "Illegal seek",
      "Read-only file system",
      "Too many links",
      "Broken pipe",
      "Numerical argument out of domain",
      "Numerical result out of range",
      "Resource deadlock avoided",
      "File name too long",
      "No locks available",
      "Function not implemented",
      "Directory not empty",
      "Too many levels of symbolic links",
      "Unknown error 41",
      "No message of desired type",
      "Identifier removed",
      "Channel number out of range",
      "Level 2 not synchronized",
      "Level 3 halted",
      "Level 3 reset",
      "Link number out of range",
      "Protocol driver not attached",
      "No CSI structure available",
      "Level 2 halted",
      "Invalid exchange",
      "Invalid request descriptor",
      "Exchange full",
      "No anode",
      "Invalid request code",
      "Invalid slot",
      "Unknown error 58",
      "Bad font file format",
      "Device not a stream",
      "No data available",
      "Timer expired",
      "Out of streams resources",
      "Machine is not on the network",
      "Package not installed",
      "Object is remote",
      "Link has been severed",
      "Advertise error",
      "Srmount error",
      "Communication error on send",
      "Protocol error",
      "Multihop attempted",
      "RFS specific error",
      "Bad message",
      "Value too large for defined data type",
      "Name not unique on network",
      "File descriptor in bad state",
      "Remote address changed",
      "Can not access a needed shared library",
      "Accessing a corrupted shared library",
      ".lib section in a.out corrupted",
      "Attempting to link in too many shared libraries",
      "Cannot exec a shared library directly",
      "Invalid or incomplete multibyte or wide character",
      "Interrupted system call should be restarted",
      "Streams pipe error",
      "Too many users",
      "Socket operation on non-socket",
      "Destination address required",
      "Message too long",
      "Protocol wrong type for socket",
      "Protocol not available",
      "Protocol not supported",
      "Socket type not supported",
      "Operation not supported",
      "Protocol family not supported",
      "Address family not supported by protocol",
      "Address already in use",
      "Cannot assign requested address",
      "Network is down",
      "Network is unreachable",
      "Network dropped connection on reset",
      "Software caused connection abort",
      "Connection reset by peer",
      "No buffer space available",
      "Transport endpoint is already connected",
      "Transport endpoint is not connected",
      "Cannot send after transport endpoint shutdown",
      "Too many references: cannot splice",
      "Connection timed out",
      "Connection refused",
      "Host is down",
      "No route to host",
      "Operation already in progress",
      "Operation now in progress",
      "Stale file handle",
      "Structure needs cleaning",
      "Not a XENIX named type file",
      "No XENIX semaphores available",
      "Is a named type file",
      "Remote I/O error",
      "Disk quota exceeded",
      "No medium found",
      "Wrong medium type",
      "Operation canceled",
      "Required key not available",
      "Key has expired",
      "Key has been revoked",
      "Key was rejected by service",
      "Owner died",
      "State not recoverable",
      "Operation not possible due to RF-kill",
      "Memory page has hardware error",
      "Unknown error"};
  if (errnum < 0 || errnum >= 134) {
    char temp[300];
    sprintf(temp, "%s %d", arr_error[134], errnum);
    char *string = temp;
    return string;
  } else {
    char *string = arr_error[errnum];
    return string;
  }
#elif __APPLE__
  char arr_error[108][256] = {"Undefined error: 0",
                              "Operation not permitted",
                              "No such file or directory",
                              "No such process",
                              "Interrupted system call",
                              "Input/output error",
                              "Device not configured",
                              "Argument list too long",
                              "Exec format error",
                              "Bad file descriptor",
                              "No child processes",
                              "Resource deadlock avoided",
                              "Cannot allocate memory",
                              "Permission denied",
                              "Bad address",
                              "Block device required",
                              "Resource busy",
                              "File exists",
                              "Cross-device link",
                              "Operation not supported by device",
                              "Not a directory",
                              "Is a directory",
                              "Invalid argument",
                              "Too many open files in system",
                              "Too many open files",
                              "Inappropriate ioctl for device",
                              "Text file busy",
                              "File too large",
                              "No space left on device",
                              "Illegal seek",
                              "Read-only file system",
                              "Too many links",
                              "Broken pipe",
                              "Numerical argument out of domain",
                              "Result too large",
                              "Resource temporarily unavailable",
                              "Operation now in progress",
                              "Operation already in progress",
                              "Socket operation on non-socket",
                              "Destination address required",
                              "Message too long",
                              "Protocol wrong type for socket",
                              "Protocol not available",
                              "Protocol not supported",
                              "Socket type not supported",
                              "Operation not supported",
                              "Protocol family not supported",
                              "Address family not supported by protocol family",
                              "Address already in use",
                              "Can't assign requested address",
                              "Network is down",
                              "Network is unreachable",
                              "Network dropped connection on reset",
                              "Software caused connection abort",
                              "Connection reset by peer",
                              "No buffer space available",
                              "Socket is already connected",
                              "Socket is not connected",
                              "Can't send after socket shutdown",
                              "Too many references: can't splice",
                              "Operation timed out",
                              "Connection refused",
                              "Too many levels of symbolic links",
                              "File name too long",
                              "Host is down",
                              "No route to host",
                              "Directory not empty",
                              "Too many processes",
                              "Too many users",
                              "Disc quota exceeded",
                              "Stale NFS file handle",
                              "Too many levels of remote in path",
                              "RPC struct is bad",
                              "RPC version wrong",
                              "RPC prog. not avail",
                              "Program version wrong",
                              "Bad procedure for program",
                              "No locks available",
                              "Function not implemented",
                              "Inappropriate file type or format",
                              "Authentication error",
                              "Need authenticator",
                              "Device power is off",
                              "Device error",
                              "Value too large to be stored in data type",
                              "Bad executable (or shared library)",
                              "Bad CPU type in executable",
                              "Shared library version mismatch",
                              "Malformed Mach-o file",
                              "Operation canceled",
                              "Identifier removed",
                              "No message of desired type",
                              "Illegal byte sequence",
                              "Attribute not found",
                              "Bad message",
                              "EMULTIHOP (Reserved)",
                              "No message available on STREAM",
                              "ENOLINK (Reserved)",
                              "No STREAM resources",
                              "Not a STREAM",
                              "Protocol error",
                              "STREAM ioctl timeout",
                              "Operation not supported on socket",
                              "Policy not found",
                              "State not recoverable",
                              "Previous owner died",
                              "Interface output queue is full",
                              "Unknown error:"};
  if (errnum < 0 || errnum >= 107) {
    char temp[300];
    sprintf(temp, "%s %d", arr_error[107], errnum);
    char *string = temp;
    return string;
  } else {
    char *string = arr_error[errnum];
    return string;
  }
#endif
}

size_t strlen(const char *str) {
  size_t len = 0;
  while (str[len] != '\0') {
    len++;
  }
  return len;
}

void *to_upper(const char *str) {
  char *copy = NULL;
  if (str != NULL) {
    copy = (char *)calloc((strlen(str) + 1), sizeof(char));
    if (copy != NULL) {
      strcpy(copy, str);
      for (size_t i = 0; i < strlen(copy); i++) {
        if (97 <= copy[i] && copy[i] <= 122) {
          copy[i] = copy[i] - 32;
        }
      }
    }
  }
  return copy;
}

void *to_lower(const char *str) {
  char *copy = NULL;
  if (str != NULL) {
    copy = (char *)calloc((strlen(str) + 1), sizeof(char));
  }
  if (copy != NULL) {
    strcpy(copy, str);
    for (size_t i = 0; i < strlen(copy); i++) {
      if (65 <= copy[i] && copy[i] <= 90) {
        copy[i] = copy[i] + 32;
      }
    }
  }
  return copy;
}

void *insert(const char *src, const char *str, size_t start_index) {
  char *copy = NULL;
  if (src != NULL && str != NULL) {
    size_t size_str = strlen(str);
    size_t size_src = strlen(src);
    size_t size_result = size_src + size_str + 1;
    if (size_result == 1 && start_index == 0) {
      copy = (char *)calloc(size_result, sizeof(char));
      copy[0] = '\0';
    } else {
      copy = (char *)calloc(size_result, sizeof(char));
      if (copy != NULL && start_index < size_src) {
        for (size_t i = 0, j = 0; j < size_result; i++, j++) {
          if (j == start_index) {
            strcpy(copy + j, str);
            j = size_str + j - 1;
            i--;
          } else {
            copy[j] = src[i];
          }
        }
      } else {
        free(copy);
        copy = NULL;
      }
    }
  }
  return copy;
}

void *trim(const char *src, const char *trim_chars) {
  char *copy = NULL;
  if (src != NULL) {
    if (trim_chars == NULL || trim_chars[0] == '\0') {
      trim_chars = " ";
    }
    int size_src = (int)strlen(src);
    if (size_src != 0) {
      int start = -1;
      int end = -1;
      int i_flag = 1, j_flag = 1;
      for (int i = 0, j = size_src - 1; i < size_src; i++, j--) {
        if (strchr(trim_chars, src[i]) == NULL && i_flag) {
          start = i;
          i_flag = 0;
        }
        if (strchr(trim_chars, src[j]) == NULL && j_flag) {
          end = j;
          j_flag = 0;
        }
        if (!i_flag && !j_flag) break;
      }
      copy = (char *)calloc((end - start + 2), sizeof(char));
      if (copy != NULL) {
        for (int i = start, j = 0; i <= end; i++, j++) {
          copy[j] = src[i];
        }
        copy[end + 1] = '\0';
      }
    } else {
      copy = (char *)calloc(1, sizeof(char));
      copy[0] = '\0';
    }
  }
  return copy;
}

void *memchr(const void *str, int c, size_t n) {
  size_t flag = 1;
  void *result = NULL;
  size_t i = 0;
  char *arr = (char *)str;
  while (i < n && flag) {
    if (arr[i] == '\0') {
      flag = 0;
    }
    if (arr[i] == c) {
      result = (char *)(arr + i);
      flag = 0;
    }
    i++;
  }
  return result;
}

int memcmp(const void *str1, const void *str2, size_t n) {
  int result = 0;
  char *arr1 = (char *)str1;
  char *arr2 = (char *)str2;
  size_t i = 0;
  while (result == 0 && i < n) {
    if (arr1[i] != arr2[i]) {
      result = arr1[i] - arr2[i];
    }
    i++;
  }
  return result;
}

void *memcpy(void *dest, const void *src, size_t n) {
  char *inp = (char *)dest;
  const char *exp = (const char *)src;
  if ((inp != NULL) && (exp != NULL)) {
    while (n) {
      *inp++ = *exp++;
      --n;
    }
  }
  return dest;
}

void *memmove(void *dest, const void *src, size_t n) {
  if (dest < src) {
    char *inp = (char *)src;
    char *exp = (char *)dest;
    for (size_t i = 0; i < n && *inp && *exp; i++) {
      *exp = *inp;
    }
  } else {
    char *inp = (char *)src + n;
    char *exp = (char *)dest + n;
    for (size_t i = 1; i <= n; i++) {
      *(exp - i) = *(inp - i);
    }
  }
  return dest;
}

void *memset(void *str, int c, size_t n) {
  char *arr = (char *)str;
  size_t i;
  for (i = 0; i < n; i++) {
    arr[i] = c;
  }
  return arr;
}

char *strpbrk(const char *str1, const char *str2) {
  int flag = 1;
  char *ch = NULL;
  for (int i = 0; flag == 1 && str1[i]; i++) {
    for (int j = 0; flag == 1 && str2[j]; j++) {
      if ((str1[i] == str2[j])) {
        ch = (char *)(str1 + i);
        flag = 0;
      }
    }
  }
  return ch;
}

char *strrchr(const char *str, int c) {
  char *res = NULL;
  for (; *str != '\0'; str++) {
    if (*str == (unsigned char)c) {
      res = (char *)str;
    }
  }
  return res;
}

size_t strspn(const char *str1, const char *str2) {
  int stop_flag = 1;
  int count_n = 0;

  if (str2 != 0) {
    for (int i = 0; str1[i] != '\0'; i++) {
      for (int j = 0; str2[j] != '\0'; j++) {
        if (str1[i] == str2[j]) {
          stop_flag = 0;
          count_n++;
          break;
        }
      }
      if (stop_flag) {
        break;
      }
      stop_flag = 1;
    }
  }
  return count_n;
}

char *strstr(const char *haystack, const char *needle) {
  size_t i, j;
  char *result = NULL;
  i = 0;
  if (needle[0] == '\0') {
    result = (char *)haystack;
  }
  while (haystack[i] != '\0') {
    j = 0;
    while (haystack[i + j] != '\0' && haystack[i + j] == needle[j]) {
      if (needle[j + 1] == '\0') {
        result = (char *)(haystack + i);
      }
      ++j;
    }
    ++i;
  }
  return result;
}

int del_check(char c, const char *delim);
char *strtok(char *str, const char *delim) {
  static char *backup = NULL;
  int flag = 1;
  int bflag = 1;
  char *retstr = NULL;
  if (str == NULL) {
    str = backup;
  }
  while (flag && bflag) {
    if (del_check(*str, delim)) {
      str++;
    } else {
      if (*str == '\0') {
        flag--;
      }
      bflag--;
    }
  }
  char *ret = str;
  while (flag) {
    if (*str == '\0') {
      backup = str;
      retstr = ret;
      flag--;
    }
    if (del_check(*str, delim)) {
      *str = '\0';
      backup = str + 1;
      retstr = ret;
      flag--;
    }
    str++;
  }
  return retstr;
}

int del_check(char c, const char *delim) {
  int res = 0;
  while (*delim != '\0') {
    if (c == *delim) {
      res++;
    }
    delim++;
  }
  return res;
}
