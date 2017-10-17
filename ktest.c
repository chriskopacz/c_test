//next iteration from itest.c
/*
read in a guesses from user -> save them in a string (which would then be
sent to the opponents game for checking)
also update a grid of my guesses or print a list of my guesses (?)
*/
/*
after reading the user guess, randomly generate an "opponent's" guess
via computer's rng - then check that guess against your board, register a
hit or miss (to be sent to opponent),  then ready for next guess input
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>

#define NUM_ROW 10
#define ROW_LEN 10

#define OCC_LEN 17
int ROW_OCC[17]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int COL_OCC[17]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
char SHIP_LET[18];
int PTR_OCC=0;
int MY_HEALTH=17;
int OPP_HEALTH = 17;
int TURN = 0;

struct Ships
{
  char name[12];
  int health;
  int length;
  int row_coords[5];
  int col_coords[5];
  int placed;
  char letter;
}ship;

void print_ship(struct Ships* ship);
void ship_hit(struct Ships *ship);
void ship_declare(struct Ships *ship,char *n,int h,char l);
void ship_place(struct Ships *ship);
int check_placement(struct Ships *ship);
void print_occ();
void place_all_ships(struct Ships *a, struct Ships *b,struct Ships *c,
  struct Ships *d, struct Ships *e);
void init_row(int *a);
void change_int_element(int *row, int c);
void init_char_row(char *a);
void change_char_element(char *a, int c,char b);
int check_opp_guess(int *r,int c);
void find_hit(struct Ships *a,struct Ships *b,struct Ships *c,struct Ships *d,
  struct Ships *e,int row, int col);

int main()
{
  srand(time(NULL)); //make more random!
  //variables and such
  int my_grid[NUM_ROW][ROW_LEN];      //contains placement of my ships
  int my_guesses[NUM_ROW][ROW_LEN];   //contains my guesses against opponent
  char my_grid_ch[NUM_ROW][ROW_LEN];  //character representaion of my_grid
  char my_guesses_ch[NUM_ROW][ROW_LEN]; //char representation of my_guesses
  int i,j;
  int play_game = 1;
  int opp_row=-1;
  int opp_col=-1;
  int opp_guess_out=-1;
  char my_guess[12];
  int my_r_guess, my_c_guess;
  char rec_buf[12];
  int rec_r,rec_c,prev_guess;
  //initialize matrixes
  for(i=0;i<NUM_ROW;i++)
  {
    init_row(my_grid[i]);
    init_row(my_guesses[i]);
    init_char_row(my_grid_ch[i]);
    init_char_row(my_guesses_ch[i]);
  }

  //declare ships
  struct Ships car;
  ship_declare(&car,"Carrier",5,'C');
  struct Ships bat;
  ship_declare(&bat,"Battleship",4,'B');
  struct Ships cru;
  ship_declare(&cru,"Cruiser",3,'U');
  struct Ships sub;
  ship_declare(&sub,"Submarine",3,'S');
  struct Ships des;
  ship_declare(&des,"Destroyer",2,'D');

  //place ships
  place_all_ships(&car,&bat,&cru,&sub,&des);

  //update my_grid
  for(i=0;i<OCC_LEN;i++)
  {
    //int's
    change_int_element(my_grid[ROW_OCC[i]],COL_OCC[i]);
    //char's
    change_char_element(my_grid_ch[ROW_OCC[i]],COL_OCC[i],SHIP_LET[i]);
  }

  //clear console
  system("clear");

  //print my_grid (my ships)
  printf("MY SHIPS:\n");
  printf("0 1 2 3 4 5 6 7 8 9\n");
  printf("===================\n");
  for(i=0;i<NUM_ROW;i++)
  {
    for(j=0;j<ROW_LEN;j++)
    {
      //int's
      //printf("%d ",my_grid[i][j]);
      //char's
      printf("%c ",my_grid_ch[i][j]);
    }
    printf(" | %d\n",i);
  }
  printf("LAST OPP GUESS: (%d,%d)\n",opp_row,opp_col);
  printf("==================== MY HEALTH = %d\n\n",MY_HEALTH);

  //print my_guesses (my guesses)
  printf("MY GUESSES:\n");
  printf("0 1 2 3 4 5 6 7 8 9\n");
  printf("===================\n");
  for(i=0;i<NUM_ROW;i++)
  {
    for(j=0;j<ROW_LEN;j++)
    {
      printf("%c ",my_guesses_ch[i][j]);
    }
    printf(" | %d\n",i);
  }
  printf("==================== OPPONENT HEALTH = %d\n\n",OPP_HEALTH);


  //take in user input as a guess to be sent to opponent
  //randomly generate an opponent's "guess" via computer rng
  //test guess against your grid, send hit or miss to opponent
  //update and print grid
  //print a list of your guesses (?)
  //if TURN%2==1 -> computer guess, then check
  //if TURN%2==0 -> user input guess

  //this infinite loop is for the play of the game
  //it will need to be updated to check if MY_HEALTH > 0 (player loses)
  //or if the opponent sends a lose signal/opp health reaches 0 (player wins)
  //for(;;)
  while(MY_HEALTH>0 && OPP_HEALTH>0)
  {

    //0 -> user input guess
    if(TURN%2==0)
    {
      printf("Input your guess: r c\n>>> ");
      scanf("%d %d",&my_r_guess,&my_c_guess); //take input from user
      //sprintf(my_guess,"%d %d",&my_r_guess,&my_c_guess);
      //send guess to opponent
      //receive hit/miss from opponent
        //update OPP_HEALTH
      //update integer grid with guess (is this needed?)
      //update char grid with guess
      change_char_element(my_guesses_ch[my_r_guess],my_c_guess,'o');
      //^^this will need to be put in an if statement depending on hit/miss

      sprintf(my_guess,"%d.%d.%d",my_r_guess,my_c_guess,opp_guess_out);
    }else //1 -> computer guess
    {
      //generate random guess from computer
      //(in future this needs to be received from opponent)
      //rand()%(max + 1 - min) + min
      opp_row = rand()%(9+1-0)+0;
      opp_col = rand()%(9+1-0)+0;


      //simulate reading opponent guess from a string received over sockets
      sprintf(rec_buf,"%d.%d.%d",opp_row,opp_col,opp_guess_out);
      sscanf(rec_buf,"%d.%d.%d",&rec_r,&rec_c,&prev_guess);


      //check opponent guess
      opp_guess_out = check_opp_guess(my_grid[rec_r],rec_c);
      if(opp_guess_out==1)
      {
        MY_HEALTH = MY_HEALTH - 1;
        find_hit(&car,&bat,&cru,&sub,&des,opp_row,opp_col);
        change_char_element(my_grid_ch[opp_row],opp_col,'x');
      }else
      {
        change_char_element(my_grid_ch[opp_row],opp_col,'o');
      }
    }

    //clear console and print updated character grid
    system("clear");
    printf("sent to opponent: %s\n",my_guess);

    //print updated character grid -> my ships
    printf("MY SHIPS:\n");
    printf("0 1 2 3 4 5 6 7 8 9\n");
    printf("===================\n");
    for(i=0;i<NUM_ROW;i++)
    {
      for(j=0;j<ROW_LEN;j++)
      {
        printf("%c ",my_grid_ch[i][j]);
      }
      printf(" | %d\n",i);
    }
    //printf("LAST OPP GUESS: (%d,%d)\n",opp_row,opp_col);
    printf("LAST OPP GUESS: (%d,%d)\n",rec_r,rec_c);
    printf("==================== MY HEALTH = %d\n\n",MY_HEALTH);

    //print updated character grid -> my guesses
    printf("MY GUESSES:\n");
    printf("0 1 2 3 4 5 6 7 8 9\n");
    printf("===================\n");
    for(i=0;i<NUM_ROW;i++)
    {
      for(j=0;j<ROW_LEN;j++)
      {
        printf("%c ",my_guesses_ch[i][j]);
      }
      printf(" | %d\n",i);
    }
    printf("==================== OPPONENT HEALTH = %d\n\n",OPP_HEALTH);

    //iterate the turn counter
    TURN = TURN + 1;
  }//while

  if(MY_HEALTH==0)
  {
    printf("\nOPPONENT WINS!\n:(\n");
  }else
  {
    printf("\nYOU WIN!\n:)\n");
  }

//leftover code from itest.c --> remove?
  /*
  for(i=0;i<NUM_ROW;i++)
  {
    opp_row=i;
    opp_col=i;
    opp_guess_out = check_opp_guess(my_grid[opp_row],opp_col);
    if(opp_guess_out == 1)
    {
      MY_HEALTH = MY_HEALTH-1;
      find_hit(&car,&bat,&cru,&sub,&des,opp_row,opp_col);
      change_char_element(my_grid_ch[opp_row],opp_col,'x');
    }else
    {
      change_char_element(my_grid_ch[opp_row],opp_col,'o');
    }
  }

  //print simulation grids
  for(i=0;i<NUM_ROW;i++)
  {
    for(j=0;j<ROW_LEN;j++)
    {
      printf("%c ",my_grid_ch[i][j]);
    }
    printf("\n");
  }
  printf("HEALTH = %d\n\n",MY_HEALTH);
  */

  return 0;
}


//=============================================================================
//user defined functions
//=============================================================================
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

  if(ship->health == 0)
  {
    printf("%s was destroyed\n",ship->name);
  }
}
//=====
void ship_declare(struct Ships *ship,char *n,int h,char l)
{
  strcpy(ship->name,n);
  ship->health=h;
  ship->length=h;
  ship->placed=-1;
  ship->letter=l;
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
      SHIP_LET[i+PTR_OCC]=ship->letter;
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
//=====
void init_row(int *a)
{
  int i;

  for(i=0;i<ROW_LEN;i++)
  {
    a[i]=0;
  }
}
//=====
void change_int_element(int *row, int c)
{
  row[c]=1;
}
//=====
void init_char_row(char *a)
{
  int i;
  for(i=0;i<ROW_LEN;i++)
  {
    a[i]='-';
  }
}
//=====
void change_char_element(char *a, int c,char b)
{
  a[c]=b;
}
//=====
int check_opp_guess(int *r, int c)
{
  //if value = 1, change to 2 -> hit
  //if value = 0, change to 3 -> miss
  if(r[c] == 1)
  {
    r[c] = 2;
    return 1;
  }else if(r[c] == 0)
  {
    r[c] = 3;
    return 0;
  }else
  {
    return 0;
  }
}
//=====
void find_hit(struct Ships *a, struct Ships *b,struct Ships *c,struct Ships *d,struct Ships *e,int row, int col)
{
  int i;

  //check ship a
  for(i=0;i<a->length;i++)
  {
    if(row=a->row_coords[i] && col==a->col_coords[i])
    {
      ship_hit(a);
    }
  }

  //check ship b
  for(i=0;i<b->length;i++)
  {
    if(row==b->row_coords[i] && col==b->col_coords[i])
    {
      ship_hit(b);
    }
  }

  //check ship c
  for(i=0;i<c->length;i++)
  {
    if(row==c->row_coords[i] && col==c->col_coords[i])
    {
      ship_hit(c);
    }
  }

  //check ship d
  for(i=0;i<d->length;i++)
  {
    if(row==d->row_coords[i] && col==d->col_coords[i])
    {
      ship_hit(d);
    }
  }

  //check ship e
  for(i=0;i<e->length;i++)
  {
    if(row==e->row_coords[i] && col==e->col_coords[i])
    {
      ship_hit(e);
    }
  }
}
