#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void myfunc(char *a);

int main()
{
  char buf[12] = "1 2 3";
  char buf2[12];
  int a,b,c,d,e;

  sscanf(buf,"%d%d%d",&a,&b,&c);


  printf("String -> %s\n",buf);
  printf("a:%d b:%d c:%d\n",a,b,c);

  printf("\nFunction:\n");
  myfunc(buf);

  printf("\n\nEnter: r.c\n");
  scanf("%s",buf2);
  printf("from scanf() -> %s\n",buf2);
  sscanf(buf2,"%d.%d",&d,&e);
  printf("from sscanf() -> d=%d, e=%d\n",d,e);



  return 0;
}
//================
void myfunc(char *a)
{
  char mybuf[12];
  int a2,b2,c2;

  strcpy(mybuf,a);

  sscanf(mybuf,"%d%d%d",&a2,&b2,&c2);

  printf("a2:%d b2:%d c2:%d\n",2*a2,2*b2,2*c2);
}
