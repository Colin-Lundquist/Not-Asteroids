#include <Windows.h>
#include "stdafx.h"
#include <stdio.h>
#include <math.h>
#include <time.h>
#include "include/sl.h"

#define GRAVITY_PX 1
#define ROTATIONAL_RESISTANCE 0.9
#define LINEAR_RESISTANCE 0.93
#define PLAYER_SHIP_ACC 0.18
#define PLAYER_SHIP_ACC_REV 0.1
#define EXHAUST_OFFSET 30
#define PI 3.14159

float x = 200;
float y = 200;

float vector_acc = 0.0; // Accelleration due to up arrow

double x_vel = 0;
double y_vel = 0;

double angle = 0.0;
double angle_vel = 0.0;

double player_dir = 0.0;
double dt;

char angle_str[10] = { 0 };

time_t timeStart;
time_t timeEnd;

double timer = 0;

void drawLimb(float limbLength)
{
	slSetForeColor(1.0, 1.0, 1.0, 1.0);
	slLine(0, 0, 0, limbLength);
}

void drawJoint(float limbLength)
{
	slSetForeColor(0.0, 0.5, 0.0, 0.5);
	slCircleFill(0, 0, limbLength * 0.2, 16);

	slSetForeColor(0.0, 0.9, 0.0, 0.9);
	slCircleOutline(0, 0, limbLength * 0.2, 16);
}

void drawPlayer(int dead);

int up_key_pressed = 0;
int down_key_pressed = 0;

int player_dead = 0;
int player_lives = 3;
int player;

int exit = 0;

int main(int args, char *argv[])
{
	
	slWindow(400, 400, "NOT ASTEROIDS", 0);
	slSetFont(slLoadFont("ttf/whitrabt.ttf"), 24);
	slSetTextAlign(SL_ALIGN_CENTER);

	int tex = slLoadTexture("png/glow2.png");
	int stars = slLoadTexture("png/stars.png");
	int fire = slLoadTexture("png/rocket_exhaust.png");

	//int background = slLoadWAV("sound/backing3.wav");

	//slSoundLoop(background);

	player = slLoadTexture("sprites/spaceship1.png");
	int key;

	while (!slShouldClose() && !(key = slGetKey(SL_KEY_ESCAPE)) && player_lives )
	{
		timer += slGetDeltaTime();

		// increment the angle of the main limb
		if (timer >= 0.01) {

			double temp_x_vel;
			double temp_y_vel;

			if (slGetKey(SL_KEY_LEFT) != 0) angle_vel += 0.75;
			else if (slGetKey(SL_KEY_RIGHT) != 0) angle_vel -= 0.75;

			if (slGetKey(SL_KEY_UP) != 0)
			{
				up_key_pressed = 1;
			}
			else
				up_key_pressed = 0;


			if (slGetKey(SL_KEY_DOWN) != 0)
			{
				down_key_pressed = 1;
			}
			else
				down_key_pressed = 0;

		  
		    angle += angle_vel;				  // Apply angular velocity
			
			if (angle > 360.0 ) angle -= 360; // Keep angle in range
			if (angle < 0.0) angle += 360;

			angle_vel *= ROTATIONAL_RESISTANCE;     // Apply angular Resistance
		

			if (up_key_pressed) {
				temp_x_vel = cos(angle * PI / 180.0) * PLAYER_SHIP_ACC;
				temp_y_vel = sin(angle * PI / 180.0) * PLAYER_SHIP_ACC;

				x_vel += temp_x_vel;
				y_vel += temp_y_vel;
			}

			if (down_key_pressed) {
				temp_x_vel = cos(angle * PI / 180.0) * PLAYER_SHIP_ACC_REV;
				temp_y_vel = sin(angle * PI / 180.0) * PLAYER_SHIP_ACC_REV;

				x_vel -= temp_x_vel;
				y_vel -= temp_y_vel;
			}

			x += x_vel;
			y += y_vel;

			x_vel *= LINEAR_RESISTANCE;
			y_vel *= LINEAR_RESISTANCE;

			timer = 0.0;

			if (x >= 400 || x <= 0 || y >= 400 || y <= 0) player_dead = 1;

			if (player_dead) {

				int key_pressed = 0;
				char lives_str[2] = { 0 };

				player = slLoadTexture("png/explosion1.png");
				tex = slLoadTexture("png/glow3.png");
				
				slPush();
				slTranslate(x, y);					
				drawPlayer(1);							
				slPop();

				
				slSprite(tex, 200, 200, 400, 400); // Death Foreground
				slSprite(stars, 200, 200, 400, 400); // Stars

				angle = 0.0;
				x = 200;
				y = 200;

				player_dead = 0;
				player_lives--;

				lives_str[0] = player_lives + 0x30;


				slSetTextAlign(SL_ALIGN_LEFT);
				slText(200, 200, lives_str);
				slText(230, 200, " Tries Left!");

				slRender();
				tex = slLoadTexture("png/glow2.png");
				player = slLoadTexture("sprites/spaceship1.png");

				timeStart = time(NULL);

				while ((time(NULL) - timeStart) < 2) {}

			}

			printf("X: %f\n", x);
			printf("Y: %f\n", y);
			printf("%f   \n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n\n", angle);
		}

		slSprite(tex, 200, 200, 400, 400); // Stars
		slSprite(stars, 200, 200, 400, 400); // Background

		// rotate the entire limb system around the center of the screen
		slPush();
		slTranslate(x, y);				
		slRotate(angle);					
		drawPlayer(0);							
		slPop();
			if (up_key_pressed) {
				


				slPush();
				slTranslate( ((EXHAUST_OFFSET  * cos(angle)) + x), ((EXHAUST_OFFSET * sin(angle)) + y));
				slRotate(angle);
				slSprite(fire, 0, 0, 40, 40);
				slPop();
			}
		

		slRender();

		if (player_lives == 0) exit = 1;
	}

	key = 0;

	slClose();
	return 0;
}

void drawPlayer(int dead)
{
	if (dead) slSprite(player, 0, 0, 160, 160);
	else slSprite(player, 0, 0, 40, 40);

}
