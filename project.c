//Meng Chen
//snake game
//project.c

#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <string.h>
#include "gfx2.h"
#define WIDTH 500
#define HEIGHT 500
#define sz 25

//elements on the board
typedef struct{
	int x,y;//the top left point
	char direction;//0:null; n:north; e:east; s:south; w:west 
	int status;//0:blank; 1:snake body; 2:fruit; 3:snake head
}Element;

//init snake position
typedef struct{
	Element* head;
	Element* tail;
}Position;

void disp_message();
void disp_gameover(int);
Position init_snake(Element[][sz]);
void init_board(Element[][sz]);
void update_board(Element[][sz]);
void rand_fruit(Element[][sz]);
int check_next(Element[][sz],char, Element* head);
Element* move_head(Element[][sz],char c, Element* head);
Element* move_tail(Element[][sz],int,Element* tail);


int main(){
	//set variables
	Element board[sz][sz];//boad
	char c = 0;//command
	int pausetime = 170000;//pause time
	int count = 0;//points
	int checkflag = -1; //check_next flag
	Element* head;//snake head pointer
	Element* tail;//snake tail pointer

	//init the board and show message
	gfx_open(WIDTH,HEIGHT,"snake game");
	init_board(board);
	Position position = init_snake(board);
	head = position.head;
	tail = position.tail;
	rand_fruit(board);
	disp_message();
	gfx_clear();
	c = 'e'; //initial direction

	//play the game
	while (c != 'q'){
		if(gfx_event_waiting()){	
			c = gfx_wait();
		}
		//gfx_flush();
		gfx_clear();
		usleep(pausetime);
		switch (c){
			case 'a'://left
				checkflag = check_next(board,c,head);
				if (checkflag == 1){//snake dies, quit the loop
					c = 'q';
				} else if(checkflag == 3) c = 'd';//pause
				else{ //move
					head = move_head(board,c,head);
					tail = move_tail(board,checkflag,tail);
					if (checkflag == 2){//new fruit
						count++;
						rand_fruit(board);
					}
				}
				break;
			case 'w'://up
				checkflag = check_next(board,c,head);
				if (checkflag == 1){
					c = 'q';
				} else if(checkflag == 3) c = 's';
				else {
					head = move_head(board,c,head);
					tail = move_tail(board,checkflag,tail);
					if (checkflag == 2){
						count++;
						rand_fruit(board);
					}
				}
				break;
			case 's'://down
				checkflag = check_next(board,c,head);
				if (checkflag == 1){
					c ='q';
				} else if(checkflag == 3) c = 'w';
				else {
					head = move_head(board,c,head);
					tail = move_tail(board,checkflag,tail);
					if (checkflag == 2){
						count++;
						rand_fruit(board);
					}
				}
				break;
			case 'd'://right
				checkflag = check_next(board,c,head);
				if (checkflag == 1){
					c ='q';
				} else if(checkflag == 3) c = 'a';
				else{
					head = move_head(board,c,head);
					tail = move_tail(board,checkflag,tail);
					if (checkflag == 2){
						count++;
						rand_fruit(board);
					}
				}
				break;
			default:
				break;		
		}
		update_board(board);
	}

	//end game
	c = ' ';
	while(c !='q'){
		disp_gameover(count);
		c = gfx_wait();
	}
	return 0;
}

void disp_message(){
	char c;
	//print message
	gfx_color(255,255,255);
	gfx_text(140,260, "Use A/W/S/D to control direction");
	gfx_text(140,360, "Press SPACE to start the game");
	gfx_text(140,340, "Press the opposite direction to make a short stop");
	gfx_text(140,280, "The yellow square is the snake head");
	gfx_text(140,300, "When you hit the wall or yourself, you die!");
	gfx_text(140,320, "When you eat red fruit, you become longer");
	gfx_text(340,480,"Designed by Meng Chen");
	char* font="12x24";
	gfx_changefont(font);
	gfx_text(120,240, "----SNAKE GAME----");
	//print snake
	gfx_color(0,205,0);
	gfx_fill_circle(165,170,30);
	gfx_fill_rectangle(165,180,130,20);
	gfx_fill_circle(295,190,10);
	gfx_color(255,255,255);
	gfx_fill_circle(150,150,10);
	gfx_fill_circle(180,150,10);
	gfx_color(0,0,0);
	gfx_fill_circle(150,153,3);
	gfx_fill_circle(182,150,3);
	gfx_color(255,0,0);
	gfx_fill_circle(165,185,10);
	gfx_color(250,150,0);
	gfx_fill_circle(165,190,5);
	//press SPACE to continue
	while(1){
		c = gfx_wait();
		if (c == ' ') break;
	}
}


void init_board(Element board[][sz]){
	int i,j;
	int len = 20;
	for(i=0;i<sz;i++){
		for(j=0;j<sz;j++){
			board[i][j].x = i*len;
			board[i][j].y = j*len;
			board[i][j].status = 0;
			board[i][j].direction = '0';
		}
	}
}


Position init_snake(Element board[][sz]){
	int xc = 8, yc = 17;
	//change init snake status & link together
	int i;
	for (i = 0;i<4;i++){
		board[xc+i][yc].status = 1; 
		board[xc+i][yc].direction = 'e'; 
	}
	//assign location to head and tail pointer
	board[xc+i-1][yc].status = 3;
	board[xc+i-1][yc].direction = 'e';
	Element *head = &board[xc+i-1][yc];
	Element *tail = &board[xc][yc];
	Position pos = {head, tail};
	return pos;
}

void rand_fruit(Element board[][sz]){
	srand(time(0));
	int f = 0;
	int x,y;
	while (f == 0){
		x = rand()%sz;
		y = rand()%sz;
		if (board[x][y].status==0){
			board[x][y].status = 2;
			f = 1;
		}
	}
}

void disp_gameover(int count){
	gfx_clear();
	char temp[20];
	sprintf(temp,"%d",count);
	//display score
	gfx_color(250,160,0);
	char* font="12x24";
	gfx_changefont(font);
	gfx_text(230,220,temp);
	//display text
	gfx_color(255,255,255);
	gfx_text(80,220,"Final Score:");
	gfx_text(80,250,"Press Q to exit the game");
}


void update_board(Element board[][sz]){
	int i,j;
	int len = 20;
	for (i=0;i<sz;i++){
		for (j=0;j<sz;j++){
			if (board[i][j].status == 1){ //draw snake
				gfx_color(0,205,0);
				gfx_fill_rectangle(board[i][j].x, board[i][j].y,len,len);
			}
			if (board[i][j].status == 3){ //draw snake head
				gfx_color(255,255,0);
				gfx_fill_rectangle(board[i][j].x, board[i][j].y,len,len);
			}
			if (board[i][j].status == 2){ //draw fruit
				gfx_color(255,0,255);
				gfx_fill_rectangle(board[i][j].x,board[i][j].y,len,len);
			}
		}
	}
}

int check_next(Element board[][sz], char c, Element* head){
	int headx = (head->x)/20;
	int heady = (head->y)/20;
	switch (c){
		case 'a':
			if(headx == 0 || (board[headx-1][heady].status==1 && head->direction !='e')) return 1; //snake dies
			else if (head->direction == 'e') return 3;//snake stops
				else return board[headx-1][heady].status;//snake survives, 0:blank; 2:food
			break;
		case 'w':
			if(heady == 0 || (board[headx][heady-1].status==1 && head->direction !='s')) return 1;
			else if (head->direction == 's') return 3;
				else return board[headx][heady-1].status;
			break;
		case 's':
			if(heady == 24 || (board[headx][heady+1].status==1 && head->direction !='n')) return 1; 
			else if (head->direction == 'n') return 3;
				else return board[headx][heady+1].status;
			break;
		case 'd':
			if(headx == 24 || (board[headx+1][heady].status==1 && head->direction !='w')) return 1;
			else if (head->direction == 'w') return 3;
				else return board[headx+1][heady].status;
			break;
	}
}


Element*  move_head(Element board[][sz],char c, Element* head){
	int x = head->x/20;
	int y = head->y/20;
	switch(c){
		case 'a':
			head->status = 1;//change current element status to snake body
			head->direction = 'w';//change the direction to the command direction
			head=&board[x-1][y];//move head
			head->status = 3;//change the status to snake head
			head->direction = 'w';//change direction
			return head;
			break; 
		case 'w':
			head->status = 1;
			head->direction = 'n';
			head=&board[x][y-1];
			head->status = 3;
			head->direction = 'n';
			return head;
			break; 
		case 's':
			head->status = 1;
			head->direction = 's';
			head=&board[x][y+1];
			head->status = 3;
			head->direction = 's';
			return head;
			break; 
		case 'd':
			head->status = 1;
			head->direction = 'e';
			head=&board[x+1][y];
			head->status = 3;
			head->direction = 'e';
			return head;
			break; 
	}
}

Element* move_tail(Element board[][sz],int f,Element* tail){
	int x = tail->x/20;
	int y = tail->y/20;
	if(f == 0){
		switch(tail->direction){
			case 'w':
				tail->status = 0;//change status to blank
				tail->direction = '0';//change direction to null
				tail=&board[x-1][y];//move tail
				return tail;
				break;
			case 'n':
				tail->status = 0;
				tail->direction = '0';
				tail=&board[x][y-1];
				return tail;
				break;
			case 's':
				tail->status = 0;
				tail->direction = '0';
				tail=&board[x][y+1];
				return tail;
				break;
			case 'e':
				tail->status = 0;
				tail->direction = '0';
				tail=&board[x+1][y];
				return tail;
				break;
		}
	}
	return tail;
}
