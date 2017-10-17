#include <stdio.h>

#define LEN 5

void init(int *arr);
void user_change(int *arr);

int main()
{
  int i;
  int array[LEN];

  init(array);
  for(i=0;i<LEN;i++)
  {
    printf("%d ",array[i]);
  }
  printf("\n");

  user_change(array);
  for(i=0;i<LEN;i++)
  {
    printf("%d ",array[i]);
  }
  printf("\n");
  
  return 0;
}
//==================
void init(int *arr)
{
  int i;

  for(i=0;i<LEN;i++)
  {
    arr[i] = 0;
  }
}
//=====
void user_change(int *arr)
{
  int temp;
  int i;

  for(i=0;i<LEN;i++)
  {
    scanf("%d",&temp);
    arr[i] = temp;
  }
}
