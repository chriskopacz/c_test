#include <stdio.h>
#include <stdlib.h>

#define ROW_LEN 3
#define NUM_ROW 4

void init_row(int *a);
void change_element(int *row,int c);
void init_char_row(char *a);
void change_char_row(int *a,char *b);
void print_hor();

int main()
{
  int array[NUM_ROW][ROW_LEN];
  int i,j;
  int urow, ucol;
  int char_len=7;
  //                                  0123456
  char chars[NUM_ROW][char_len];// = "| | | |";

  //clear console
  system("clear");
  
  //initialize matrix
  for(i=0;i<NUM_ROW;i++)
  {
    init_row(array[i]);
  }

  //print initialized matrix
  printf("Original:\n");
  for(i=0;i<NUM_ROW;i++)
  {
    for(j=0;j<ROW_LEN;j++)
    {
      printf("%d ",array[i][j]);
    }
    printf("\n");
  }
  printf("\n");

  //get row and column from user
  printf("Change which element? (r,c)\n");
  scanf("%d %d",&urow,&ucol);
  change_element(array[urow-1],ucol-1);
  
  //print new matrix
  printf("New:\n");
  for(i=0;i<NUM_ROW;i++)
  {
    for(j=0;j<ROW_LEN;j++)
    {
      printf("%d ",array[i][j]);
    }
    printf("\n");
  }
  printf("\n");


  //init char row
  for(i=0;i<NUM_ROW;i++)
  {
    init_char_row(chars[i]);
  }

  //print initialized char matrix
  printf("Original:\n");
  print_hor();
  for(i=0;i<NUM_ROW;i++)
  {
    for(j=0;j<char_len;j++)
    {
      printf("%c",chars[i][j]);
    }
    printf("\n");
    print_hor();
  }
  printf("\n\n");

  //change char matrix from previous user input
  for(i=0;i<NUM_ROW;i++)
  {
    change_char_row(array[i],chars[i]);
  }

  //print altered char matrix
  printf("New:\n");
  print_hor();
  for(i=0;i<NUM_ROW;i++)
  {
    for(j=0;j<char_len;j++)
    {
      printf("%c",chars[i][j]);
    }
    printf("\n");
    print_hor();
  }
  
  return 0;
}
//=================
void init_row(int *a)
{
  int i;

  for(i=0;i<ROW_LEN;i++)
  {
    a[i] = 0;
  }
}
//=====
void change_element(int *row,int c)
{
  int temp;
  printf("Change it to what?\n");
  scanf("%d",&temp);

  row[c] = temp;
}
//=====
void init_char_row(char *a)
{
  int i;
  int char_len = 7;

  for(i=0;i<char_len;i++)
  {
    if(i%2==0)
    {
      a[i] = '|';
    }else
    {
      a[i] = ' ';
    }
  }
}
//=====
void change_char_row(int *a, char *b)
{
  int i;

  for(i=0;i<ROW_LEN;i++)
  {
    if(a[i] != 0)
    {
      b[(2*i)+1] = 'x';
    }
  }
}
//=====
void print_hor()
{
  printf("-------\n");
}
