/**
 * Copyright 2020 Sergei Akhmatdinov
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied *.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "../rogueutil.h"
#include <stdlib.h> /* for srand() / rand() */
#include <stdio.h>

#ifndef min
#define min(a,b) (((a) < (b)) ? (a) : (b))
#endif /* min */

/* Tiles */
#define FLOOR 0
#define WALL 1
#define COIN (1 << 1)
#define STAIRS_DOWN (1 << 2)
#define TORCH (1 << 4)

#define MAPSIZE 15

#define NOCOLOR -1

void gen(int seed);
void draw(void);

/* Globals */
int x, y;
int coins = 0, moves = 0, torch = 30, level = 1;
int lvl[MAPSIZE][MAPSIZE];

/* Generates the dungeon map */
void 
gen(int seed) 
{
	srand(seed);
	int i, j;

	for (j = 0; j < MAPSIZE; j++) {
		for (i = 0; i < MAPSIZE; i++) {
			if (i == 0 || i == MAPSIZE - 1 || j == 0 ||
					j == MAPSIZE - 1 || rand() % 10 == 0) {
				lvl[i][j] = WALL;
			} else if (rand() % 20 == 0) {
				lvl[i][j] = COIN;
			} else if (rand() % 100 == 0) {
				lvl[i][j] = TORCH;
			} else {
				lvl[i][j] = FLOOR;	
			}		
		}
	}

	#define randcoord (1 + rand() % (MAPSIZE - 2))
	x = randcoord;
	y = randcoord;
	lvl[randcoord][randcoord] = STAIRS_DOWN;
	#undef randcoord
}

/* Draws the screen */
void 
draw(void) 
{
	cls()
		;
	locate(1, MAPSIZE + 1);
	colorPrint(YELLOW, NOCOLOR, "Coins: %d\n", coins);
	colorPrint(RED, NOCOLOR, "Torch: %d\n", torch);
	colorPrint(MAGENTA, NOCOLOR, "Moves: %d\n", moves);
	colorPrint(GREEN, NOCOLOR, "Level: %d\n", level);
	locate(1, 1);
	
	int i, j;
	for (j = 0; j < MAPSIZE; j++) {
		for (i = 0; i < MAPSIZE; i++) {
			if (0); /* (i == x && j == y) printf("@"); */
			else if (abs(x - i) + abs(y - j) > min(10, torch / 2)) {
				printf(" ");
			} else if (lvl[i][j] == 0) {
				colorPrint(BLUE, NOCOLOR, ".");
			} else if (lvl[i][j] & WALL) {
				colorPrint(CYAN, NOCOLOR, "#"); 
			} else if (lvl[i][j] & COIN) { 
				colorPrint(YELLOW, NOCOLOR, "o"); 
			} else if (lvl[i][j] & STAIRS_DOWN) {
				colorPrint(GREEN, NOCOLOR, "<");
			} else if (lvl[i][j] & TORCH) {
				colorPrint(RED, NOCOLOR, "f");
			}
		}

		printf("\n");
	}

	locate(x + 1, y + 1);
	colorPrint(WHITE, NOCOLOR, "@");
	fflush(stdout);
}

/* Main loop and input handling */
int 
main(void) 
{
	hidecursor();
	saveDefaultColor();

	gen(level);

	colorPrint(GREEN, NOCOLOR, "Welcome! Use WASD to move, ESC to quit.\n");

	setColor(YELLOW);
	anykey("Hit any key to start.\n");
	draw();
	for (;;) {
		/* Input */
		if (kbhit()) {
			char k = getkey();

			int oldx = x;
			int oldy = y;
			if (k == 'a') { 
				--x; ++moves; 
			} else if (k == 'd') {
				++x; ++moves; 
			} else if (k == 'w') {
				--y; ++moves; 
			} else if (k == 's') {
				++y; ++moves; 
			} else if (k == KEY_ESCAPE) {
				break;
			}

			/* Collisions */
			if (lvl[x][y] & WALL) {
				x = oldx;
				y = oldy; 
			} else if (lvl[x][y] & COIN) {
				coins++;
				lvl[x][y] ^= COIN; 
			} else if (lvl[x][y] & TORCH) {
				torch += 20;
				lvl[x][y] ^= TORCH; 
			} else if (lvl[x][y] & STAIRS_DOWN) {
				gen(++level);
			}

			/* Drawing */
			draw();
			
			/* Die */
			if (--torch <= 0)
                                break;
		}
	}

	cls();
        colorPrint(RED, NOCOLOR, "You have been eaten by a grue.\n");
	colorPrint(YELLOW, NOCOLOR, "Coins: %d\n", coins);
	anykey(NULL);
	resetColor();
	showcursor();

	return 0;
}
