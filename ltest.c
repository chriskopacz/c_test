//continuation of ktest.c
/*
implement communication between two computers for play between people instead
of generating random guesses from the computer
*/
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <math.h>
//from chat implementation
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>

//from chat
#define PORT "4950"    // the port users will be connecting to
#define MAXBUFLEN 100

#define NUM_ROW 10
#define ROW_LEN 10

#define OCC_LEN 17
int ROW_OCC[17]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int COL_OCC[17]={-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
char SHIP_LET[18];
int PTR_OCC=0;
int MY_HEALTH=17;
int OPP_HEALTH = 17;
//int TURN = 0;

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

//chat implementation
// get sockaddr, IPv4 or IPv6:
void *get_in_addr(struct sockaddr *sa);
void me_listen(char *rec);
int me_talk(char *addr,int my_r,int my_c);
int me_receive_response();
void me_send_response(char *addr,int opp_out);

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

int main(int argc, char *argv[])
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

  //set TURN to equal argv[2]
  int TURN = atoi(argv[2]);

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
      int temp_guess=-1;
      while(temp_guess==-1)
      {

        //pass my_r_guess, my_c_guess to me_talk()
        printf("Input your guess: r c\n>>> ");
        scanf("%d %d",&my_r_guess,&my_c_guess); //take input from user


        //check my_grid to see that this r,c hasnt been previously guessed
        if(my_guesses[my_r_guess][my_c_guess]==1)
        {//if it has been guessed, print a message
          printf("Already guessed. Go again.\n");
        }else
        {//if it hasnt been guessed already
          //update my_grid
          change_int_element(my_guesses[my_r_guess],my_c_guess);
          temp_guess = 0; //change temp_guess
        }
      }
      //send guess to opponent
      me_talk(argv[1],my_r_guess,my_c_guess);

      //receive response
      prev_guess = me_receive_response();

      //update my_guesses_ch
      if(prev_guess==1)
      {//previous guess was a hit
        change_char_element(my_guesses_ch[my_r_guess],my_c_guess,'X');
        OPP_HEALTH = OPP_HEALTH - 1;
      }else if(prev_guess==0)
      {//previous guess was a miss
        change_char_element(my_guesses_ch[my_r_guess],my_c_guess,'o');
      }else
      {//previous guess was an unknown
        change_char_element(my_guesses_ch[my_r_guess],my_c_guess,'B');
      }
    }else //1 -> opponent guess
    {
      //receive guess from opponent
      me_listen(rec_buf);

      //read opponent guess from a string received over sockets
      sscanf(rec_buf,"%d.%d",&rec_r,&rec_c);

      //check opponent guess for hit/miss against my ships (my_grid)
      opp_guess_out = check_opp_guess(my_grid[rec_r],rec_c);

      //update my_grid_ch
      if(opp_guess_out==1)
      {
        MY_HEALTH = MY_HEALTH - 1;
        find_hit(&car,&bat,&cru,&sub,&des,rec_r,rec_c);
        change_char_element(my_grid_ch[rec_r],rec_c,'x');
      }else
      {
        change_char_element(my_grid_ch[rec_r],rec_c,'o');
      }

      //send hit/miss response back to opponent
      me_send_response(argv[1],opp_guess_out);
    }

    //clear console and print updated character grid
    system("clear");
    /*if(TURN%2==0)
    {
      printf("sent to opponent: (%d,%d)\n",my_r_guess,my_c_guess);
    }
    */

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
void find_hit(struct Ships *a, struct Ships *b,struct Ships *c,struct Ships *d,
  struct Ships *e,int row, int col)
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
//============
//chat implementation functions
//=====
void *get_in_addr(struct sockaddr *sa)
{
  if (sa->sa_family == AF_INET)
  {
    return &(((struct sockaddr_in*)sa)->sin_addr);
  }

  return &(((struct sockaddr_in6*)sa)->sin6_addr);
}
//=====
void me_listen(char *rec)
{
  int sockfd;
  struct addrinfo hints, *servinfo, *p;
  int rv;
  int numbytes;
  struct sockaddr_storage their_addr;
  char buf[MAXBUFLEN];
  socklen_t addr_len;
  char s[INET6_ADDRSTRLEN];
  int bind_d;


  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE; // use my IP

  rv = getaddrinfo(NULL,PORT,&hints,&servinfo);
  if(rv != 0)
  {
    fprintf(stderr, "getaddrinfo listen: %s\n", gai_strerror(rv));
  }

  // loop through all the results and bind to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next)
  {
    sockfd = socket(p->ai_family,p->ai_socktype,p->ai_protocol);
    if(sockfd == -1)
    {
      perror("listener: socket");
      continue;
    }

    bind_d = bind(sockfd,p->ai_addr,p->ai_addrlen);
    if(bind_d == -1)
    {
      close(sockfd);
      perror("listener: bind");
      continue;
    }

    break;
  }

  if(p == NULL)
  {
    fprintf(stderr, "listener: failed to bind socket\n");
  }

  freeaddrinfo(servinfo);

  printf("waiting...\n");

  addr_len = sizeof their_addr;

  //receive message
  numbytes = recvfrom(sockfd,buf,MAXBUFLEN-1,0,(struct sockaddr *)&their_addr,
                      &addr_len);
  if(numbytes == -1)
  {
    perror("recvfrom");
    exit(1);
  }

  //append null terminator to end of received message
  buf[numbytes] = '\0';

  //copy the received message into a string pointer that main() has
  strncpy(rec,buf,numbytes+1);

  close(sockfd);
}
//=====
int me_talk(char *addr,int my_r,int my_c)
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    rv = getaddrinfo(addr,PORT,&hints,&servinfo);
    if (rv != 0)
    {
      fprintf(stderr, "getaddrinfo talk: %s\n", gai_strerror(rv));
      return 1;
    }

    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next)
    {
      sockfd = socket(p->ai_family,p->ai_socktype,p->ai_protocol);
      if (sockfd == -1)
      {
        perror("talker: socket");
        continue;
      }

      break;
    }

    if (p == NULL) {
        fprintf(stderr, "talker: failed to create socket\n");
        return 2;
    }

    int talk_len = 12;
    char talk_buff[talk_len];
    //format guess into a string
    sprintf(talk_buff,"%d.%d",my_r,my_c);

    //send string to opponent
    numbytes = sendto(sockfd,talk_buff,talk_len,0,p->ai_addr,p->ai_addrlen);

    if(numbytes == -1)
    {
      perror("talker: sendto");
      exit(1);
    }

    freeaddrinfo(servinfo);

    //printf("talker: sent %d bytes to %s\n", numbytes, addr);
    close(sockfd);

    return 0;
}
//=====
int me_receive_response()
{
  int sockfd;
  struct addrinfo hints, *servinfo, *p;
  int rv;
  int numbytes;
  struct sockaddr_storage their_addr;
  char buf[MAXBUFLEN];
  socklen_t addr_len;
  char s[INET6_ADDRSTRLEN];
  int bind_d;


  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC; // set to AF_INET to force IPv4
  hints.ai_socktype = SOCK_DGRAM;
  hints.ai_flags = AI_PASSIVE; // use my IP

  rv = getaddrinfo(NULL,PORT,&hints,&servinfo);
  if(rv != 0)
  {
    fprintf(stderr, "getaddrinfo listen: %s\n", gai_strerror(rv));
  }

  // loop through all the results and bind to the first we can
  for(p = servinfo; p != NULL; p = p->ai_next)
  {
    sockfd = socket(p->ai_family,p->ai_socktype,p->ai_protocol);
    if(sockfd == -1)
    {
      perror("listener: socket");
      continue;
    }

    bind_d = bind(sockfd,p->ai_addr,p->ai_addrlen);
    if(bind_d == -1)
    {
      close(sockfd);
      perror("listener: bind");
      continue;
    }

    break;
  }

  if(p == NULL)
  {
    fprintf(stderr, "listener: failed to bind socket\n");
  }

  freeaddrinfo(servinfo);

  printf("waiting...\n");

  addr_len = sizeof their_addr;

  //receive message
  numbytes = recvfrom(sockfd,buf,MAXBUFLEN-1,0,(struct sockaddr *)&their_addr,
                      &addr_len);
  if(numbytes == -1)
  {
    perror("recvfrom");
    exit(1);
  }

  //append null terminator to end of received message
  buf[numbytes] = '\0';

  //copy the received message into a string pointer that main() has
  //strncpy(rec,buf,numbytes+1);

  //guet integer from response
  int guess_out=-1;
  sscanf(buf,"%d",&guess_out);

  close(sockfd);

  //return the outcome of your guess
  return guess_out;
}
//=====
void me_send_response(char *addr,int opp_out)
{
    int sockfd;
    struct addrinfo hints, *servinfo, *p;
    int rv;
    int numbytes;

    memset(&hints, 0, sizeof hints);
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_DGRAM;

    rv = getaddrinfo(addr,PORT,&hints,&servinfo);
    if (rv != 0)
    {
      fprintf(stderr, "getaddrinfo talk: %s\n", gai_strerror(rv));
      //return 1;
    }

    // loop through all the results and make a socket
    for(p = servinfo; p != NULL; p = p->ai_next)
    {
      sockfd = socket(p->ai_family,p->ai_socktype,p->ai_protocol);
      if (sockfd == -1)
      {
        perror("talker: socket");
        continue;
      }

      break;
    }

    if (p == NULL) {
        fprintf(stderr, "talker: failed to create socket\n");
        //return 2;
    }

    int talk_len = 12;
    char talk_buff[talk_len];
    //format guess into a string
    sprintf(talk_buff,"%d",opp_out);

    //send string to opponent
    numbytes = sendto(sockfd,talk_buff,talk_len,0,p->ai_addr,p->ai_addrlen);

    if(numbytes == -1)
    {
      perror("talker: sendto");
      exit(1);
    }

    freeaddrinfo(servinfo);

    close(sockfd);

    //return 0;
}
