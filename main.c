#include <stdio.h>
#include <stdlib.h>
#ifdef WIN32
#define __STDC_WANT_LIB_EXT1__
#endif
#include <string.h>

#include <xcleanup.h>

void foo() {
  int *temp = (int *)malloc(1000 * sizeof(*temp));
  AUTO_CLEANUP(temp);
}

_Noreturn void boo() {
  int *temp = (int *)malloc(1000 * sizeof(*temp));
  AUTO_CLEANUP(temp);

  exit(0);
}

int main() {
  CLEANUP_INIT();

  int *arr = (int *)malloc(50 * sizeof(*arr));
  AUTO_CLEANUP(arr);

  REMOVE_CLEANUP(arr);
  arr = (int *)realloc(arr, 100 * sizeof(*arr));
  AUTO_CLEANUP(arr);

  char *str = (char *)calloc(256, sizeof(*str));
  AUTO_CLEANUP(str);

#ifdef _WIN32
  strcpy_s(str, 256, "Hello, darkness, my old friend!..\0");
#else
  strcpy(str, "Hello, darkness, my old friend!..\0");
#endif
  printf("%s\n", str);

  FILE *fd = NULL;
#ifdef _WIN32
  fopen_s(&fd, "secret.txt", "w+");
#else
  fd = fopen("secret.txt", "w+");
#endif
  AUTO_CLEANUP(fd);

  fprintf(fd, "%s\n", str);

  foo();

  printf("end of foo()\n");

  boo();

  return 0;
}
