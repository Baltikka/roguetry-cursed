#include <ncurses.h>
#include <stdlib.h>
#include <stdio.h>
#include <time.h>

int randomgen(int low, int up);
int movement(char inp);
int mapgen(int mx, int my);
int regenerate();
int displaylog();
int displaystats(WINDOW *stat_win);
bool checkposition(int x, int y, int newX, int newY);
int playermove(int x, int y, int newX, int newY);
int x=15;
int y=40;

int depth=1,level=1,hp=10,maxhp=10,expr=0, turns=0;

char* logmsg;
int main()
{
	logmsg="Welcome. Again.";
	WINDOW *log_win;
	WINDOW *stat_win;
	initscr();
	cbreak();
	noecho();
	start_color();
	curs_set(0);
	log_win = newwin(20, 30, 10, 82);
	stat_win = newwin(10,30,0,82);
	refresh();
	displaystats(stat_win);
	displaylog(log_win);
	wrefresh(log_win);
	wrefresh(stat_win);
	refresh();
	srand(time(0));
	mapgen(15,40);
	mvaddch(x,y,'@');
	char inp;
	while(true){
		inp = getch();
		movement(inp);
		displaylog(log_win);
		displaystats(stat_win);
		wrefresh(stat_win);
		wrefresh(log_win);
		turns++;
	}
	getch();
	endwin();
	return 0;
}

int movement(char inp)
{
	int newX=x, newY=y;;
	switch(inp)
	{
		case('w'):
		case('W'):
                	newX=x-1;
			if(checkposition(x,y,newX,newY))
			{
				playermove(x,y,newX,newY);
				x=newX;
			}
			else
				newX=x;
			break;
		case('s'):
		case('S'):
                	newX=x+1;
			if(checkposition(x,y,newX,newY))
                        {
                                playermove(x,y,newX,newY);
                                x=newX;
                        }
                        else
                                newX=x;
			break;
		case('a'):
		case('A'):
                	newY=y-1;
                	if(checkposition(x,y,newX,newY))
                        {
                                playermove(x,y,newX,newY);
                                y=newY;
                        }
                        else
                                newY=y;
			break;
		case('d'):
		case('D'):
                	newY=y+1;
                	if(checkposition(x,y,newX,newY))
                        {
                                playermove(x,y,newX,newY);
                                y=newY;
                        }
                        else
                                newY=y;
			break;
	}
        move(0,0);
	return 0;
}

bool checkposition(int x, int y, int newX, int newY)
{
	switch(mvinch(newX,newY))
	{
		case('='):
			logmsg="...";
			return(true);
			break;
		case('>'):
			depth++;
			regenerate();
			break;
		default:
			logmsg="you bump into something...";
			return(false);
			break;
	}
}

int playermove(int x, int y, int newX, int newY)
{
	mvaddch(x,y,'=');
	mvaddch(newX,newY,'@');
}

int randomgen(int low, int up)
{
	return (rand()%(up-low+1))+low;
}
int mapgen(int mx, int my)
{
	move(mx,my);
	int length = depth*100;
	int limX=mx*2-1, limY=my*2-1;
	int counter=0;
	int prev=3;
	int r=3;
	while(counter<=length)
	{
		r=randomgen(0,3);
		if(r!=prev)
		{
			prev = r;
			mvaddch(mx,my,'=');
			if(r==0 && mx>0){
				mx--;
			}
			else if(r==1 && mx<limX){
				mx++;
			}
			else if(r==2 && my>0){
				my--;
			}
			else if(r==3 && my<limY){
				my++;
			}
			counter++;
		}
	}
	mvaddch(mx,my,'>');
	return 0;
}

int regenerate(WINDOW *log_win)
{
	erase();
	wrefresh(log_win);
	x=15;
	y=40;
	mapgen(15,40);
        mvaddch(x,y,'@');
        move(0,0);
	logmsg="you descend deeper...";
	return 0;
}

int displaylog(WINDOW *log_win){
	werase(log_win);
	wprintw(log_win,"\n ");
	waddstr(log_win,logmsg);
	box(log_win,0,0);
	return 0;
}

int displaystats(WINDOW *stat_win)
{
	werase(stat_win);
	wprintw(stat_win,"\n DEPTH: %d TURNS: %d", depth,turns);
	box(stat_win,0,0);
	return 0;
}

