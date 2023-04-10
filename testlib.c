#include "primlib.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

#define WIDTH_OF_MAP 10
#define HEIGHT_OF_MAP 20
#define SIZE_OF_SQUARE (gfx_screenHeight() / HEIGHT_OF_MAP)
#define FLOOR gfx_screenHeight() - (SIZE_OF_SQUARE * HEIGHT_OF_MAP)
#define START_OF_MAP (gfx_screenWidth() / 2 - WIDTH_OF_MAP * SIZE_OF_SQUARE / 2)
#define WIDTH_OF_MAP_IN_PIXELS (WIDTH_OF_MAP * SIZE_OF_SQUARE)
#define START_OF_MAP_FOR_NEXT_BLOCK (START_OF_MAP + WIDTH_OF_MAP_IN_PIXELS + 50)
#define HEIGHT_OF_MAP_FOR_NEXT_BLOCK 100
#define Y_OF_SPAWNING_BLOCK 2
#define X_OF_SPAWNING_BLOCK (WIDTH_OF_MAP / 2 - 1)

int gamemap[HEIGHT_OF_MAP][WIDTH_OF_MAP] = {0};

char pieces[7 /*kind */][4 /* rotation */][4][4] = {
	/* square */
	{{{2, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	 {{2, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	 {{2, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	 {{2, 1, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
	/* I */

	{{{1, 2, 1, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	 {{1, 0, 0, 0}, {2, 0, 0, 0}, {1, 0, 0, 0}, {1, 0, 0, 0}},
	 {{1, 1, 2, 1}, {0, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	 {{1, 0, 0, 0}, {1, 0, 0, 0}, {2, 0, 0, 0}, {1, 0, 0, 0}}},
	/* L */
	{{{1, 0, 0, 0}, {2, 0, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
	 {{1, 2, 1, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	 {{1, 1, 0, 0}, {0, 2, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
	 {{0, 0, 1, 0}, {1, 2, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
	/* L mirrored */
	{{{0, 1, 0, 0}, {0, 2, 0, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}},
	 {{1, 0, 0, 0}, {1, 2, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	 {{1, 1, 0, 0}, {2, 0, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}},
	 {{1, 2, 1, 0}, {0, 0, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
	/* N */
	{{{0, 1, 0, 0}, {2, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}},
	 {{1, 2, 0, 0}, {0, 1, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	 {{0, 1, 0, 0}, {1, 2, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}},
	 {{1, 1, 0, 0}, {0, 2, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
	/* N mirrored */
	{{{1, 0, 0, 0}, {2, 1, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
	 {{0, 2, 1, 0}, {1, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	 {{1, 0, 0, 0}, {1, 2, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
	 {{0, 1, 1, 0}, {1, 2, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}},
	/* T */
	{{{1, 0, 0, 0}, {2, 1, 0, 0}, {1, 0, 0, 0}, {0, 0, 0, 0}},
	 {{1, 2, 1, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}},
	 {{0, 1, 0, 0}, {1, 2, 0, 0}, {0, 1, 0, 0}, {0, 0, 0, 0}},
	 {{0, 1, 0, 0}, {1, 2, 1, 0}, {0, 0, 0, 0}, {0, 0, 0, 0}}}};

void drawMap(int gamemap[HEIGHT_OF_MAP][WIDTH_OF_MAP])
{
	gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);
	gfx_line(START_OF_MAP, 0, START_OF_MAP, gfx_screenHeight() - 1, WHITE);
	gfx_line(START_OF_MAP + WIDTH_OF_MAP_IN_PIXELS, 0,
			 START_OF_MAP + WIDTH_OF_MAP_IN_PIXELS, gfx_screenHeight() - 1,
			 WHITE);
	gfx_filledRect(START_OF_MAP, SIZE_OF_SQUARE * HEIGHT_OF_MAP + 1, START_OF_MAP + WIDTH_OF_MAP_IN_PIXELS, gfx_screenHeight(),GREEN);
	gfx_line(START_OF_MAP, 6 * SIZE_OF_SQUARE,
			 START_OF_MAP + WIDTH_OF_MAP_IN_PIXELS, 6 * SIZE_OF_SQUARE, RED);
	for (int i = 0; i < HEIGHT_OF_MAP; ++i) {
		for (int j = 0; j < WIDTH_OF_MAP; ++j) {
			/*
			if (gamemap[i][j] == 0) {
				gfx_rect(START_OF_MAP + j * SIZE_OF_SQUARE, i * SIZE_OF_SQUARE,
						 START_OF_MAP + (j + 1) * SIZE_OF_SQUARE,
						 (i + 1) * SIZE_OF_SQUARE, WHITE);
			}
			*/
			if (gamemap[i][j] == 1) {
				gfx_filledRect(START_OF_MAP + j * SIZE_OF_SQUARE,
							   i * SIZE_OF_SQUARE,
							   START_OF_MAP + (j + 1) * SIZE_OF_SQUARE,
							   (i + 1) * SIZE_OF_SQUARE, WHITE);
			}
			if (gamemap[i][j] == 2) {
				gfx_filledRect(START_OF_MAP + j * SIZE_OF_SQUARE,
							   i * SIZE_OF_SQUARE,
							   START_OF_MAP + (j + 1) * SIZE_OF_SQUARE,
							   (i + 1) * SIZE_OF_SQUARE, YELLOW);
			}
			if (gamemap[i][j] == 3) {
				gfx_filledRect(START_OF_MAP + j * SIZE_OF_SQUARE,
							   i * SIZE_OF_SQUARE,
							   START_OF_MAP + (j + 1) * SIZE_OF_SQUARE,
							   (i + 1) * SIZE_OF_SQUARE, RED);
			}
		}
	}
}
void drawNextPiece(int block)
{

	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {

			if (pieces[block][0][i][j] == 1) {
				gfx_filledRect(
					START_OF_MAP_FOR_NEXT_BLOCK + j * SIZE_OF_SQUARE,
					i * SIZE_OF_SQUARE + HEIGHT_OF_MAP_FOR_NEXT_BLOCK,
					START_OF_MAP_FOR_NEXT_BLOCK + (j + 1) * SIZE_OF_SQUARE,
					(i + 1) * SIZE_OF_SQUARE + HEIGHT_OF_MAP_FOR_NEXT_BLOCK,
					WHITE);
			}
			if (pieces[block][0][i][j] == 2) {
				gfx_filledRect(
					START_OF_MAP_FOR_NEXT_BLOCK + j * SIZE_OF_SQUARE,
					i * SIZE_OF_SQUARE + HEIGHT_OF_MAP_FOR_NEXT_BLOCK,
					START_OF_MAP_FOR_NEXT_BLOCK + (j + 1) * SIZE_OF_SQUARE,
					(i + 1) * SIZE_OF_SQUARE + HEIGHT_OF_MAP_FOR_NEXT_BLOCK,
					YELLOW);
			}
		}
	}
}

void showNewBlock(int gamemap[HEIGHT_OF_MAP][WIDTH_OF_MAP], int randomblock,
				  int randomposition, int arr[2])
{
	for (int j = 0; j < 4; ++j) {
		for (int i = 0; i < 4; ++i) {
			gamemap[Y_OF_SPAWNING_BLOCK - arr[0] + j]
				   [X_OF_SPAWNING_BLOCK - arr[1] + i] =
					   pieces[randomblock][randomposition][j][i];
		}
	}
}
void findYellowInGamemap(int arr[2])
{
	for (int i = 0; i < HEIGHT_OF_MAP; ++i) {
		for (int j = 0; j < WIDTH_OF_MAP; ++j) {
			if (gamemap[i][j] == 2) {
				arr[0] = i;
				arr[1] = j;
			}
		}
	}
}
void findYellowInBlock(int arr[2], int block, int position)
{
	for (int i = 0; i < 4; ++i) {
		for (int j = 0; j < 4; ++j) {
			if (pieces[block][position][i][j] == 2) {
				arr[0] = i;
				arr[1] = j;
			}
		}
	}
}

void makeAllRed()
{
	for (int i = 0; i < HEIGHT_OF_MAP; ++i) {
		for (int j = 0; j < WIDTH_OF_MAP; ++j) {
			if (gamemap[i][j] == 1 || gamemap[i][j] == 2) {
				gamemap[i][j] = 3;
			}
		}
	}
}
void makeAllWhite()
{
	for (int i = 0; i < HEIGHT_OF_MAP; ++i) {
		for (int j = 0; j < WIDTH_OF_MAP; ++j) {
			if (gamemap[i][j] == 1 || gamemap[i][j] == 2) {
				gamemap[i][j] = 0;
			}
		}
	}
}
void moveDown()
{
	for (int i = HEIGHT_OF_MAP - 1; i >= 0; --i) {
		for (int j = 0; j < WIDTH_OF_MAP; ++j) {
			if (gamemap[i][j] == 1) {
				gamemap[i + 1][j] = 1;
				gamemap[i][j] = 0;
			}
			if (gamemap[i][j] == 2) {
				gamemap[i + 1][j] = 2;
				gamemap[i][j] = 0;
			}
		}
	}
}

void checkLastRow()
{
	for (int j = 0; j < WIDTH_OF_MAP; ++j) {
		if (gamemap[HEIGHT_OF_MAP - 1][j] == 1 ||
			gamemap[HEIGHT_OF_MAP - 1][j] == 2) {
			makeAllRed();
		}
	}
}
void moveLeft()
{
	for (int i = 0; i < HEIGHT_OF_MAP; ++i) {
		for (int j = 0; j < WIDTH_OF_MAP; ++j) {
			if (gamemap[i][j] == 1) {
				gamemap[i][j - 1] = 1;
				gamemap[i][j] = 0;
			}
			if (gamemap[i][j] == 2) {
				gamemap[i][j - 1] = 2;
				gamemap[i][j] = 0;
			}
		}
	}
}
void moveRight()
{
	for (int i = 0; i < HEIGHT_OF_MAP; ++i) {
		for (int j = WIDTH_OF_MAP - 1; j >= 0; --j) {
			if (gamemap[i][j] == 1) {
				gamemap[i][j + 1] = 1;
				gamemap[i][j] = 0;
			}
			if (gamemap[i][j] == 2) {
				gamemap[i][j + 1] = 2;
				gamemap[i][j] = 0;
			}
		}
	}
}
int checkIfCanMoveRight()
{
	int check = 0;
	for (int i = 0; i < HEIGHT_OF_MAP; ++i) {
		for (int j = WIDTH_OF_MAP - 1; j >= 0; --j) {
			if (gamemap[i][j] == 1 && gamemap[i][j + 1] == 3) {
				check = 1;
				break;
			}
			if (gamemap[i][j] == 2 && gamemap[i][j + 1] == 3) {
				check = 1;
				break;
			}
		}
	}
	return check;
}
int checkIfCanMoveLeft()
{
	int check = 0;
	for (int i = 0; i < HEIGHT_OF_MAP; ++i) {
		for (int j = 0; j < WIDTH_OF_MAP; ++j) {
			if (gamemap[i][j] == 1 && gamemap[i][j - 1] == 3) {
				check = 1;
				break;
			}
			if (gamemap[i][j] == 2 && gamemap[i][j - 1] == 3) {
				check = 1;
				break;
			}
		}
	}
	return check;
}

int checkSideLeft()
{
	int check = 0;
	for (int i = 0; i < HEIGHT_OF_MAP; ++i) {
		for (int j = 0; j < WIDTH_OF_MAP; ++j) {
			if (gamemap[i][0] == 1 || gamemap[i][0] == 2) {
				check = 1;
				break;
			}
		}
	}
	return check;
}
int checkSideRight()
{
	int check = 0;
	for (int i = 0; i < HEIGHT_OF_MAP; ++i) {
		if (gamemap[i][WIDTH_OF_MAP - 1] == 1 ||
			gamemap[i][WIDTH_OF_MAP - 1] == 2) {
			check = 1;
			break;
		}
	}
	return check;
}
int checkIfBlockIsActive()
{
	int check = 0;
	for (int i = 0; i < HEIGHT_OF_MAP; ++i) {
		for (int j = 0; j < WIDTH_OF_MAP; ++j) {
			if (gamemap[i][j] == 1 || gamemap[i][j] == 2) {
				check = 1;
				break;
			}
		}
	}
	return check;
}
void checkIfBlockUnder()
{
	for (int i = 0; i < HEIGHT_OF_MAP; ++i) {
		for (int j = 0; j < WIDTH_OF_MAP; ++j) {
			if (gamemap[i][j] == 1 && gamemap[i + 1][j] == 3) {
				makeAllRed();
			}
			if (gamemap[i][j] == 2 && gamemap[i + 1][j] == 3) {
				makeAllRed();
			}
		}
	}
}
void moveDownRedBlocks(int x)
{
	for (int i = x; i >= 0; --i) {
		for (int j = 0; j < WIDTH_OF_MAP; ++j) {
			if (gamemap[i][j] == 0 && gamemap[i - 1][j] == 3) {
				gamemap[i][j] = 3;
				gamemap[i - 1][j] = 0;
			}
		}
	}
}
void checkIfWin()
{
	for (int i = HEIGHT_OF_MAP - 1; i >= 0; --i) {
		int check = 0;
		for (int j = 0; j < WIDTH_OF_MAP; ++j) {
			if (gamemap[i][j] != 3) {
				check = 1;
				break;
			}
		}
		if (check == 0) {
			for (int j = 0; j < WIDTH_OF_MAP; ++j) {
				gamemap[i][j] = 0;
			}
			moveDownRedBlocks(i);
		}
	}
}
void goDown()
{
	while (checkIfBlockIsActive() == 1) {
		for (int i = HEIGHT_OF_MAP - 1; i >= 0; --i) {
			for (int j = 0; j < WIDTH_OF_MAP; ++j) {
				if (gamemap[i][j] == 1) {
					gamemap[i + 1][j] = 1;
					gamemap[i][j] = 0;
				}
				if (gamemap[i][j] == 2) {
					gamemap[i + 1][j] = 2;
					gamemap[i][j] = 0;
				}
			}
		}
		checkLastRow();
		checkIfBlockUnder();
	}
}
int checkToRotate(int arr1[2], int arr2[2], int randomblock, int randomposition)
{

	findYellowInBlock(arr2, randomblock, randomposition);
	findYellowInGamemap(arr1);
	int check = 0;
	for (int j = 0; j < 4; ++j) {
		for (int i = 0; i < 4; ++i) {
			if ((gamemap[arr1[0] - arr2[0] + j][arr1[1] - arr2[1] + i] == 3) &&
				(pieces[randomblock][randomposition][j][i] == 1)) {
				check = 1;
				break;
			}
			if ((gamemap[arr1[0] - arr2[0] + j][arr1[1] - arr2[1] + i] == 2) &&
				((arr1[1] - arr2[1] + i) >= (WIDTH_OF_MAP - 1))) {
				check = 1;
				break;
			}
			if ((gamemap[arr1[0] - arr2[0] + j][arr1[1] - arr2[1] + i] == 2) &&
				((arr1[1] - arr2[1] + i) <= 0)) {
				check = 1;
				break;
			}
			if ((gamemap[arr1[0] - arr2[0] + j][arr1[1] - arr2[1] + i] == 2) &&
				((arr1[1] - arr2[1] + i) <= 0)) {
				check = 1;
				break;
			}
			if ((gamemap[arr1[0] - arr2[0] + j][arr1[1] - arr2[1] + i] == 2) &&
				((arr1[1] - arr2[1] + i) <= 1) && randomblock == 1) {
				check = 1;
				break;
			}
			if ((gamemap[arr1[0] - arr2[0] + j][arr1[1] - arr2[1] + i] == 2) &&
				((arr1[1] - arr2[1] + i) >= (WIDTH_OF_MAP - 2)) &&
				randomblock == 1) {
				check = 1;
				break;
			}
		}
	}
	return check;
}
int changePosition(int randomposition)
{
	if (randomposition <= 2) {
		randomposition++;
	}
	else {
		randomposition = 0;
	}
	return randomposition;
}
void rotate(int arr1[2], int arr2[2], int randomblock, int randomposition)
{
	makeAllWhite();
	for (int j = 0; j < 4; ++j) {
		for (int i = 0; i < 4; ++i) {
			if (gamemap[arr1[0] - arr2[0] + j][arr1[1] - arr2[1] + i] != 3) {
				gamemap[arr1[0] - arr2[0] + j][arr1[1] - arr2[1] + i] =
					pieces[randomblock][randomposition][j][i];
			}
		}
	}
}

int checkLoss()
{
	int check = 0;
	for (int i = 0; i < HEIGHT_OF_MAP; ++i) {
		for (int j = 0; j < WIDTH_OF_MAP; ++j) {
			if (gamemap[i][j] == 3 && i == 5) {
				check = 1;
				break;
			}
		}
	}
	return check;
}
void showLossScreen()
{
	gfx_filledRect(0, 0, gfx_screenWidth() - 1, gfx_screenHeight() - 1, BLACK);
	gfx_textout(gfx_screenWidth() / 2, gfx_screenHeight() / 2, "GAME OVER",
				RED);
	gfx_updateScreen();
}
int main(int argc, char* argv[])
{
	if (gfx_init()) {
		exit(3);
	}
	srand(time(NULL));
	int randomblock = 0;
	int randomposition = 0;
	int nextrandomblock = 0;
	int nextrandomposition = 0;
	int arr1[2];
	int arr2[2];
	int time = 0;
	int key;

	randomblock = rand() % 7;
	randomposition = rand() % 4;
	nextrandomblock = rand() % 7;
	nextrandomposition = rand() % 4;

	while (1) {
		while (checkIfBlockIsActive() == 1) {
			if (time % 120 == 0) {
				moveDown();
			}
			key = gfx_pollkey();
			if (key == SDLK_LEFT) {
				if (checkSideLeft() == 0 && checkIfCanMoveLeft() == 0) {
					moveLeft();
				}
			}
			if (key == SDLK_RIGHT) {
				if (checkSideRight() == 0 && checkIfCanMoveRight() == 0) {
					moveRight();
				}
			}
			if (key == SDLK_DOWN) {
				goDown();
			}
			if (key == SDLK_SPACE) {
				randomposition = changePosition(randomposition);
				if (checkToRotate(arr1, arr2, randomblock, randomposition) ==
					0) {
					rotate(arr1, arr2, randomblock, randomposition);
				}
			}
			if (checkLoss() == 1) {
				while (1) {
					showLossScreen();
					if (gfx_isKeyDown(SDLK_ESCAPE) == 1) {
						return 0;
					}
				}
			}
			checkLastRow();
			checkIfBlockUnder();
			checkIfWin();
			
			time++;
			drawMap(gamemap);
			drawNextPiece(nextrandomblock);
			gfx_updateScreen();
		}
		randomblock = nextrandomblock;
		randomposition = nextrandomposition;
		nextrandomblock = rand() % 7;
		nextrandomposition = rand() % 4;
		findYellowInBlock(arr2, randomblock, randomposition);
		showNewBlock(gamemap, randomblock, randomposition, arr2);
	}

	return 0;
}
