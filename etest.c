#include <stdio.h>
#include <string.h>

#define NUM_ROW 10
#define ROW_LEN 10

struct Ships
{
  char name[12];
  int health;
  int length;
  int row_coords[5];
  int col_coords[5];
  int placed;
}ship;

void print_ship(struct Ships* ship);
void ship_hit(struct Ships *ship);
void ship_declare(struct Ships *ship,char *n,int h);
void ship_place(struct Ships *ship);

int main()
{
  struct Ships car;
  ship_declare(&car,"Carrier",5);
  struct Ships bat;
  ship_declare(&bat,"Battleship",4);
  struct Ships cru;
  ship_declare(&cru,"Cruiser",3);
  struct Ships sub;
  ship_declare(&sub,"Submarine",3);
  struct Ships des;
  ship_declare(&des,"Destroyer",2);

  ship_place(&car);
  ship_hit(&car);
  print_ship(&car);
  
  return 0;
}
//=============
void print_ship(struct Ships *ship)
{
  if(ship->placed == 0)
  {
    printf("Ship name:\n %s\n",ship->name);
    printf("Location:\n");
    for(int i=0;i<ship->length;i++)
    {
      printf(" (%d,%d)",ship->row_coords[i],ship->col_coords[i]);
    }
    printf("\n");
    printf("Health:\n %d\n",ship->health);
    printf("Length:\n %d\n",ship->length);
  }else
  {
    printf("%s not placed properly\n",ship->name);
  }
}
//=====
void ship_hit(struct Ships *ship)
{
  ship->health = ship->health - 1;
}
//=====
void ship_declare(struct Ships *ship,char *n,int h)
{
  strcpy(ship->name,n);
  ship->health=h;
  ship->length=h;
  ship->placed=-1;
}
//=====
void ship_place(struct Ships *ship)
{
  int len = ship->length;
  char dir[6];
  int r,c;
  int check = -1;
  int i;
  
  printf("Enter location and direction of ship:\n");
  printf("   r c d -> integer integer character(u d l r)\n");
  scanf("%d %d %s",&r,&c,dir);

  printf("r:%d c:%d d:%s\n",r,c,dir);

  if(r>=0 && r<=9 && r-len+1>=0 && r+len-1<=9)
  {
    if(c>=0 && c<=9 && r-len+1>=0 && r+len-1<=9)
    {
      check=1;
    }
  }

  if(check !=-1)
  {
    ship->placed=0;
    switch(*dir)
    {
    case('u'):
      for(i=0;i<len;i++)
      {
	ship->row_coords[len-i-1]=r-i;
	ship->col_coords[len-i-1]=c;
      }
      break;
    case('d'):
      for(i=0;i<len;i++)
      {
	ship->row_coords[i]=r+i;
	ship->col_coords[i]=c;
      }
      break;
    case('l'):
      for(i=0;i<len;i++)
      {
	ship->row_coords[len-i-1]=r;
	ship->col_coords[len-i-1]=c-i;
      }
      break;
    case('r'):
      for(i=0;i<len;i++)
      {
	ship->row_coords[i]=r;
	ship->col_coords[i]=c+i;
      }
      break;
    default:
      for(i=0;i<len;i++)
      {
	ship->row_coords[i]=-1;
	ship->col_coords[i]=-1;
	ship->placed=-1;
      }
    }
  }else
  {
    for(i=0;i<len;i++)
    {
      ship->row_coords[i]=-1;
      ship->col_coords[i]=-1;
    }
  }
}
//=====
