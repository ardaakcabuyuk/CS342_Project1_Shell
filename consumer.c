#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>

int main(int argc, char *argv[])
{
  int char_count = atoi(argv[1]);

  char c;
  for (int i = 0; i < char_count; i++) {
    read(0, &c, 1);
    printf("%c\n", c);
  }

  //printf("%s\n", kelime);
}
