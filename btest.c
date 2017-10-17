#include <stdio.h>
#include <stdlib.h>

int grid[3] = {0,1,2};

void change(int i);

int main()
{
  int turn = 0;

  while(turn<3)
  {
    printf("%d %d %d\n",grid[0],grid[1],grid[2]);
    change(turn);
    turn = turn + 1;
  }
  
  return 0;
}
//=====
void change(int i)
{
  int temp;
  scanf("%d",&temp);
  grid[i] = temp;
}
