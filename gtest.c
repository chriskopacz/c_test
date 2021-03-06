#include <stdio.h>
#include <string.h>

#define NUM_ROW 10
#define ROW_LEN 10

#define OCC_LEN 17
int ROW_OCC[17]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int COL_OCC[17]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int PTR_OCC=0;

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
int check_placement(struct Ships *ship);
void print_occ();
void place_all_ships(struct Ships *a, struct Ships *b,struct Ships *c, struct Ships *d, struct Ships *e);

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

  /*
  ship_place(&car);
  ship_place(&bat);
  ship_place(&cru);
  ship_place(&sub);
  ship_place(&des);
  */
  //init_my_grid();
  place_all_ships(&car,&bat,&cru,&sub,&des);
  //update_my_board();
  print_occ();
  
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
  int placement_check = -1;

  while(placement_check == -1)
  {
    printf("Enter location and direction of %s(%d):\n",ship->name,len);
    printf("   r c d -> integer integer character(u d l r)\n");
    scanf("%d %d %s",&r,&c,dir);

    //printf("r:%d c:%d d:%s\n",r,c,dir);

    if(r>=0 && r<=9 && c>=0 && c<=9)
    {
      switch(*dir)
      {
      case('u'):
	if(r-len+1>=0)
	{
	  check = 0;
	  break;
	}
      case('d'):
	if(r+len-1<=9)
	{
	  check = 0;
	  break;
	}
      case('l'):
	if(c-len+1>=0)
	{
	  check = 0;
	  break;
	}
      case('r'):
	if(c+len-1<=9)
	{
	  check = 0;
	  break;
	}
      default:
	check = -1;
      }
    }

    if(check != -1)
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

    placement_check = check_placement(ship);
    if(placement_check == -1)
    {
      ship->placed=-1;
      printf("Bad placement. Try again.\n");
    }
  }
}
//=====
int check_placement(struct Ships *ship)
{
  int placement_ok=0;
  int i,j;
  
  for(i=0;i<OCC_LEN;i++)
  {
    for(j=0;j<ship->length;j++)
    {
      if(ship->row_coords[j]==ROW_OCC[i] && ship->col_coords[j]==COL_OCC[i])
      {
	placement_ok=-1;
      }
    }
  }

  if(placement_ok==0)
  {
    for(i=0;i<ship->length;i++)
    {
      ROW_OCC[i+PTR_OCC]=ship->row_coords[i];
      COL_OCC[i+PTR_OCC]=ship->col_coords[i];
    }
    PTR_OCC = PTR_OCC + ship->length;
    printf("%s placed successfully.\n",ship->name);
  }

  return placement_ok;
}
//=====
void print_occ()
{
  int i;

  for(i=0;i<OCC_LEN;i++)
  {
    printf("(%d,%d)\n",ROW_OCC[i],COL_OCC[i]);
  }
}
//=====
void place_all_ships(struct Ships *a, struct Ships *b, struct Ships *c, struct Ships *d, struct Ships *e)
{
  ship_place(a);
  ship_place(b);
  ship_place(c);
  ship_place(d);
  ship_place(e);
}
