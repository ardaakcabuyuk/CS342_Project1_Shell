#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int main(int argc, char *argv[])
{
  static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

  int char_count = atoi(argv[1]);
  char c;
  srand(time(NULL));
  for (int i = 0; i < char_count; i++) {
    c = alphanum[rand() % (sizeof(alphanum) - 1)];
    printf("%c", c);
  }
}
