#include <stdio.h>

int main()
{
  char s[] = {"abcd"};

  printf("%c\n",s[0]);

  s[0] = 'E';

  printf("%s %c\n",s,s[0]);
  
  return 0;
}
