/*Spacship GAME
AUTHORS: Mthandeni Magwaza 211524036
       : Lwazi Luthuli 211519616
	   : Nhlakanipho Nsimande 21050324
	   : Lungisani Dlangisa
	   : Llwelyn Naidoo 211515870
	   : Mlungisi Sikhahane  210510335
	   
*/

#include <allegro5\allegro.h>
#include <allegro5\allegro_primitives.h>
#include <allegro5\allegro_font.h>
#include <allegro5\allegro_ttf.h>
#include <allegro5\allegro_opengl.h>

#include<iostream>
#include <string>

#include "Oject.h"
#include "Bullet.h"
#include "Comet.h"
using namespace std;


//GLOBALS
const int WIDTH = 1000;
const int HEIGHT = 600;
const int NUM_BULLETS = 15;
const int NUM_COMETS = 15;


// keyboard key 
enum KEYS{ UP, DOWN, LEFT, RIGHT, SPACE };
bool keys[5] = { false, false, false, false, false };

//function prototypes

//Ship objects 
void InitShip(SpaceShip &ship);
void DrawShip(SpaceShip &ship);
void MoveShipUp(SpaceShip &ship);
void MoveShipDown(SpaceShip &ship);
void MoveShipLeft(SpaceShip &ship);
void MoveShipRight(SpaceShip &ship);

//Bullet objects
void InitBullet(Bullet bullet[], int size);
void DrawBullet(Bullet bullet[], int size);
void FireBullet(Bullet bullet[], int size, SpaceShip &ship);
void UpdateBullet(Bullet bullet[], int size);
void CollideBullet(Bullet bullet[], int bSize, Comet comets[], int cSize, SpaceShip &ship);

//Comet objects
void InitComet(Comet comets[], int size);
void DrawComet(Comet comets[], int size);
void StartComet(Comet comets[], int size);
void UpdateComet(Comet comets[], int size);
void CollideComet(Comet comets[], int cSize, SpaceShip &ship);


//MAIN LOOP
int main(void)
{
	string name;


	int spaces = 0;
	int LEVEL = 1;
	int increment = 0;
	int count_bulets = 0;
	//primitive variable
	bool done = false;      //indicator to whether game has finished i.e. used to escape from game loop
	bool redraw = true;		//used for renering
	bool isGameOver = false;
	//GAME RENDERED AT 60 FRAMES PER SECOND
	const int FPS = 60;

	//object variables
	SpaceShip ship;
	Bullet bullets[NUM_BULLETS];
	Comet comets[NUM_COMETS];

	//Allegro variables
	ALLEGRO_DISPLAY *display = NULL;
	ALLEGRO_EVENT_QUEUE *event_queue = NULL;
	ALLEGRO_TIMER *timer = NULL;
	ALLEGRO_FONT *font18 = NULL;

	//Initialization Functions
	if (!al_init())										//initialize Allegro
		return -1;

	display = al_create_display(WIDTH, HEIGHT);			//create our display object

	if (!display)										//test display object
		return -1;

	//initialisers
	al_init_primitives_addon();
	al_install_keyboard();
	al_init_font_addon();
	al_init_ttf_addon();
	event_queue = al_create_event_queue();
	timer = al_create_timer(1.0 / FPS);

	// random variable
	srand(time(NULL));

	//initialise objects
	InitShip(ship);
	InitBullet(bullets, NUM_BULLETS);
	InitComet(comets, NUM_COMETS);

	//font to be used to draw text to display
	font18 = al_load_font("arial.ttf", 18, 0);

	


	//capture events
	al_register_event_source(event_queue, al_get_keyboard_event_source());
	al_register_event_source(event_queue, al_get_timer_event_source(timer));
	al_register_event_source(event_queue, al_get_display_event_source(display));

	//start timer
	al_start_timer(timer);

	//game loop
	while (!done)
	{
		++count_bulets;
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);

		if (ev.type == ALLEGRO_EVENT_TIMER)
		{

			redraw = true;
			if (keys[UP])
				MoveShipUp(ship);
			if (keys[DOWN])
				MoveShipDown(ship);
			if (keys[LEFT])
				MoveShipLeft(ship);
			if (keys[RIGHT])
				MoveShipRight(ship);

			
			
			
			if (!isGameOver)
			{
				UpdateBullet(bullets, NUM_BULLETS);
				StartComet(comets, NUM_COMETS);
				UpdateComet(comets, NUM_COMETS);
				CollideBullet(bullets, NUM_BULLETS, comets, NUM_COMETS, ship);
				CollideComet(comets, NUM_COMETS, ship);

				if (ship.lives <= 0)
					isGameOver = true;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
		{
			done = true;
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_DOWN)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = true;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = true;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = true;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = true;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = true;
				FireBullet(bullets, NUM_BULLETS, ship);
				break;
			}
		}
		else if (ev.type == ALLEGRO_EVENT_KEY_UP)
		{
			switch (ev.keyboard.keycode)
			{
			case ALLEGRO_KEY_ESCAPE:
				done = true;
				break;
			case ALLEGRO_KEY_UP:
				keys[UP] = false;
				break;
			case ALLEGRO_KEY_DOWN:
				keys[DOWN] = false;
				break;
			case ALLEGRO_KEY_LEFT:
				keys[LEFT] = false;
				break;
			case ALLEGRO_KEY_RIGHT:
				keys[RIGHT] = false;
				break;
			case ALLEGRO_KEY_SPACE:
				keys[SPACE] = false;
				break;
			}
		}

		/*
		rendering , draw to screen
		*/
		if (redraw && al_is_event_queue_empty(event_queue))
		{
			redraw = false;

			if (!isGameOver)
			{
				DrawShip(ship);
				DrawBullet(bullets, NUM_BULLETS);
				DrawComet(comets, NUM_COMETS);
				//al_draw_text(font , al_map_rgb(255, 215, 0), 10, 5,0, "NKN" );
				for (int i = 0; i<name.length(); i++)
				{
					al_draw_textf(font18, al_map_rgb(255, 215, 0), 10 + i * 15, 5, 0, "%c ", name[i]);
					spaces = 10 + i * 15 + 50;
				}
				//cout<<spaces<<endl;

				al_draw_textf(font18, al_map_rgb(0, 255, 0), spaces, 5, 0, "LEVEL : %i       %i LIVES        %i COMETS DESTROYED    SCORE: %i", LEVEL, ship.lives, ship.score, ship.score * 5);
			}
			else
			{
				if (ship.score>40)
					al_draw_textf(font18, al_map_rgb(0, 255, 255), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, " !!!EXELLENT!!!       Game Over. Final Score: %i !!", ship.score * 5);
				else if (ship.score>30)
					al_draw_textf(font18, al_map_rgb(0, 255, 255), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, " !!!WELL TRIED!!!     Game Over. Final Score: %i !!", ship.score * 5);
				else if (ship.score>20)
					al_draw_textf(font18, al_map_rgb(0, 255, 255), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, " !!!AVERAGE PERFORMANCE!!!     Game Over. Final Score: %i !!", ship.score * 5);

				else
					al_draw_textf(font18, al_map_rgb(0, 255, 255), WIDTH / 2, HEIGHT / 2, ALLEGRO_ALIGN_CENTRE, "!!!!ARMARTURE!!!!   Game Over. Final Score: %i !!", ship.score * 5);

			}

			//after every 15 comets you shoot the level is increamented and you gain an etra life
			if (increment != 1 && ship.score != 0 && ship.score % 15 == 0)
			{

				LEVEL += 1;
				increment = 1;
				ship.lives += 1;
				for (int j = 0; j < NUM_BULLETS; j++)
				{
					bullets[j].speed += 1;
				}
				for (int i = 0; i < NUM_COMETS; i++)
				{
					comets[i].speed += 1;
				}
			
			}
			if (increment == 1 && ship.score % 10 != 0)
				increment = 0;

			al_flip_display();
			al_clear_to_color(al_map_rgb(0, 0, 0));


		}
	}

	al_destroy_event_queue(event_queue);
	al_destroy_timer(timer);
	al_destroy_font(font18);
	al_destroy_display(display);						//destroy our display object

	return 0;
}

//funtions

void InitShip(SpaceShip &ship)
{
	ship.x = 20;
	ship.y = HEIGHT / 2;
	ship.ID = PLAYER;
	ship.lives = 6;   // six lives given to player
	ship.speed = 7;
	ship.boundx = 6;
	ship.boundy = 7;
	ship.score = 0;
}
void DrawShip(SpaceShip &ship)
{
	al_draw_filled_rectangle(ship.x, ship.y - 9, ship.x + 10, ship.y - 7, al_map_rgb(255, 0, 255));
	al_draw_filled_rectangle(ship.x, ship.y + 9, ship.x + 10, ship.y + 7, al_map_rgb(255, 0, 255));

	al_draw_filled_triangle(ship.x - 12, ship.y - 17, ship.x + 12, ship.y, ship.x - 12, ship.y + 17, al_map_rgb(0, 255, 0));
	al_draw_filled_rectangle(ship.x - 12, ship.y - 2, ship.x + 15, ship.y + 2, al_map_rgb(0, 0, 255));
}
void MoveShipUp(SpaceShip &ship)
{
	ship.y -= ship.speed;
	if (ship.y < 0)
		ship.y = 0;
}
void MoveShipDown(SpaceShip &ship)
{
	ship.y += ship.speed;
	if (ship.y > HEIGHT)
		ship.y = HEIGHT;
}
void MoveShipLeft(SpaceShip &ship)
{
	ship.x -= ship.speed;
	if (ship.x < 0)
		ship.x = 0;
}
void MoveShipRight(SpaceShip &ship)
{
	ship.x += ship.speed;
	if (ship.x > 300)
		ship.x = 300;
}

void InitBullet(Bullet bullet[], int size)
{
	for (int i = 0; i < size; i++)
	{
		bullet[i].ID = BULLET;
		bullet[i].speed = 10;
		bullet[i].live = false;
	}
}
void DrawBullet(Bullet bullet[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (bullet[i].live)
		{
			al_draw_filled_circle(bullet[i].x1, bullet[i].y1, 2, al_map_rgb(255, 255, 0));
			al_draw_filled_circle(bullet[i].x2, bullet[i].y2, 2, al_map_rgb(255, 255, 255));
			al_draw_filled_circle(bullet[i].x3, bullet[i].y3, 2, al_map_rgb(255, 255, 255));
		}
	}
}
void FireBullet(Bullet bullet[], int size, SpaceShip &ship)
{
	for (int i = 0; i < size; i++)
	{
		if (!bullet[i].live)
		{
			bullet[i].x1 = ship.x + 19;
			bullet[i].y1 = ship.y;
			bullet[i].x2 = ship.x + 16;
			bullet[i].y2 = ship.y - 2;
			bullet[i].x3 = ship.x + 15;
			bullet[i].y3 = ship.y + 2;
			bullet[i].live = true;
			break;
		}
	}
}
void UpdateBullet(Bullet bullet[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (bullet[i].live)
		{
			bullet[i].x1 += bullet[i].speed;
			bullet[i].x2 += bullet[i].speed;
			bullet[i].x3 += bullet[i].speed;
			if (bullet[i].x1 > WIDTH)
				bullet[i].live = false;
			if (bullet[i].x2 > WIDTH)
				bullet[i].live = false;
			if (bullet[i].x3 > WIDTH)
				bullet[i].live = false;
		}
	}
}
void CollideBullet(Bullet bullet[], int bSize, Comet comets[], int cSize, SpaceShip &ship)
{
	for (int i = 0; i < bSize; i++)
	{
		if (bullet[i].live)
		{
			for (int j = 0; j < cSize; j++)
			{
				if (comets[j].live)
				{
					if (bullet[i].x1 >(comets[j].x - comets[j].boundx) &&
						bullet[i].x1 < (comets[j].x + comets[j].boundx) &&
						bullet[i].y1 >(comets[j].y - comets[j].boundy) &&
						bullet[i].y1 < (comets[j].y + comets[j].boundy))
					{
						bullet[i].live = false;
						comets[j].live = false;

						ship.score++;
					}
				}
			}
		}
	}
}

void InitComet(Comet comets[], int size)
{
	for (int i = 0; i < size; i++)
	{
		comets[i].ID = ENEMY;
		comets[i].live = false;
		comets[i].speed = 1;
		comets[i].boundx = 18;
		comets[i].boundy = 18;
	}
}
void DrawComet(Comet comets[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (comets[i].live && rand() % 4 == 0)
		{
			al_draw_filled_circle(comets[i].x, comets[i].y, 18, al_map_rgb(0, 169, 0));//red
		}
		if (comets[i].live && rand() % 4 == 1)
		{
			al_draw_filled_circle(comets[i].x, comets[i].y, 15, al_map_rgb(0, 169, 0));//orange red
		}
		if (comets[i].live && rand() % 4 == 2)
		{
			al_draw_filled_circle(comets[i].x, comets[i].y, 12, al_map_rgb(0, 169, 0));//fire brick
		}
		if (comets[i].live && rand() % 4 == 3)
		{
			al_draw_filled_circle(comets[i].x, comets[i].y, 10, al_map_rgb(0, 169, 0));//blue
		}
		if (comets[i].live && rand() % 10 == 0)
		{
			al_draw_filled_circle(comets[i].x, comets[i].y, 20, al_map_rgb(0, 169, 0));//dark red
		}
	}
}
void StartComet(Comet comets[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (!comets[i].live)
		{
			if (rand() % 500 == 0)
			{
				comets[i].live = true;
				comets[i].x = WIDTH;
				comets[i].y = 30 + rand() % (HEIGHT - 60);

				break;
			}
		}
	}
}
void UpdateComet(Comet comets[], int size)
{
	for (int i = 0; i < size; i++)
	{
		if (comets[i].live)
		{
			comets[i].x -= comets[i].speed;
		}
	}
}
void CollideComet(Comet comets[], int cSize, SpaceShip &ship)
{
	for (int i = 0; i < cSize; i++)
	{
		if (comets[i].live)
		{
			if (comets[i].x - comets[i].boundx < ship.x + ship.boundx &&
				comets[i].x + comets[i].boundx > ship.x - ship.boundx &&
				comets[i].y - comets[i].boundy < ship.y + ship.boundy &&
				comets[i].y + comets[i].boundy > ship.y - ship.boundy)
			{
				ship.lives--;
				comets[i].live = false;
			}
			else if (comets[i].x < 0)
			{
				comets[i].live = false;
				ship.lives--;
			}
		}
	}
}