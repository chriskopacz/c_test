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
  int p1_array[NUM_ROW][ROW_LEN];
  int p2_array[NUM_ROW][ROW_LEN];
  int i,j;
  int urow_1, ucol_1;
  int urow_2, ucol_2;
  int char_len=7;
  //                                     0123456
  char p1_chars[NUM_ROW][char_len];// = "| | | |";
  char p2_chars[NUM_ROW][char_len];

  //clear console
  system("clear");
  
  //initialize matrix
  for(i=0;i<NUM_ROW;i++)
  {
    init_row(p1_array[i]);
    init_row(p2_array[i]);
  }

  //print initialized matrix
  printf("Org p1:    Org p2:\n");
  for(i=0;i<NUM_ROW;i++)
  {
    for(j=0;j<ROW_LEN;j++)
    {
      printf("%d ",p1_array[i][j]);
    }
    printf("     ");
    for(j=0;j<ROW_LEN;j++)
    {
      printf("%d ",p2_array[i][j]);
    }
    printf("\n");
  }
  printf("\n");

  //get row and column from user
  printf("Change which element in p1? -> r c\n");
  scanf("%d %d",&urow_1,&ucol_1);
  change_element(p1_array[urow_1-1],ucol_1-1);
  printf("Change which element in p2? -> r c\n");
  scanf("%d %d",&urow_2,&ucol_2);
  change_element(p2_array[urow_2-1],ucol_2-1);
  
  //print new matrix
  printf("New p1:    New p2:\n");
  for(i=0;i<NUM_ROW;i++)
  {
    for(j=0;j<ROW_LEN;j++)
    {
      printf("%d ",p1_array[i][j]);
    }
    printf("     ");
    for(j=0;j<ROW_LEN;j++)
    {
      printf("%d ",p2_array[i][j]);
    }
    printf("\n");
  }
  printf("\n");


  //init char row
  for(i=0;i<NUM_ROW;i++)
  {
    init_char_row(p1_chars[i]);
    init_char_row(p2_chars[i]);
  }

  //print initialized char matrix
  printf("Org p1:     Org p2:\n");
  //print_hor();
  printf("-------     -------\n");
  for(i=0;i<NUM_ROW;i++)
  {
    for(j=0;j<char_len;j++)
    {
      printf("%c",p1_chars[i][j]);
    }
    printf("     ");
    for(j=0;j<char_len;j++)
    {
      printf("%c",p2_chars[i][j]);
    }
    printf("\n");
    //print_hor();
    printf("-------     -------\n");
  }
  printf("\n\n");

  //change char matrix from previous user input
  for(i=0;i<NUM_ROW;i++)
  {
    change_char_row(p1_array[i],p1_chars[i]);
    change_char_row(p2_array[i],p2_chars[i]);
  }

  //print altered char matrix
  printf("New p1:     Newp2:\n");
  //print_hor();
  printf("-------     -------\n");
  for(i=0;i<NUM_ROW;i++)
  {
    for(j=0;j<char_len;j++)
    {
      printf("%c",p1_chars[i][j]);
    }
    printf("     ");
    for(j=0;j<char_len;j++)
    {
      printf("%c",p2_chars[i][j]);
    }
    printf("\n");
    //print_hor();
    printf("-------     -------\n");
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
