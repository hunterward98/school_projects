// Author: Hunter Ward
// Class: Computer Science 1
// File: battleship.cpp
// This file implements most functions that the main() function will call.
// Here we create functions relating to the game grid. This includes a function
// that lets the player/computer shoot, one that randomly sets up the game board
// for a player/computer (and checks validity)

#include "battleship.h"

void displayGrid(char grid[][BOARD_SIZE]) {
	int i = 0, j, k = 0, m = 0;
	cout << "  0 1 2 3 4 5 6 7 8 9" << endl;
	for (i = 0; i < BOARD_SIZE; i++) {
		m = 0;
   	for (j = 0; j < BOARD_SIZE; j++) {
			if (j == 0) {
				cout << i << " ";
			}
				cout << grid[i][j] << " ";
      }
      cout << endl;        
	}
}

void initializeGrid(char grid[][BOARD_SIZE]) {
	int i, j;
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			grid[i][j] = '~';
      }
   }
}

int checkCharBoard(char grid[][BOARD_SIZE], bool CORRECT[]) {
	int i, j, k;
	int correct = 0;
	int correctChar[5] = {0, 0, 0, 0, 0};
		for (k = 0; k < NUM_SHIPS; k++) {
			for (i = 0; i < BOARD_SIZE; i++) {
				for (j = 0; j < BOARD_SIZE; j++) {
					if (grid[i][j] == SHIP_SYMBOLS[k]) {
						correctChar[k] = correctChar[k] + 1;
					}
   		   }
  			}
  			if (correctChar[k] == SHIP_SIZES[k]) {
  					//cout << SHIP_NAMES[k] << " has the correct amount of characters." << endl;
  				CORRECT[k] = true;
				correct++;
	  		}
  			else {
  				//cout << SHIP_NAMES[k] << " needs to be filled." << endl;
  				CORRECT[k] = false;
  				correct = 0;
  			}
  		}
  	return correct;
}

void setupP1Board(char grid[][BOARD_SIZE]) {
	int i, j;
	int row, col;
	int temprow = -1, tempcol = -1, temprow2 = -1, tempcol2 = -1; // used to detect if the ship is placed vertically or horizontally
	char choice;
	
	for (i = 0; i < NUM_SHIPS; i++) {

		cout << "Please enter the " << SHIP_SIZES[i] << " cells you would like to place your " << SHIP_NAMES[i] << " on. (Column, Row) [or known as (x, y)]" << endl;
		cout << "Example input for a Submarine vertically placed in column 4, in rows 7-9: " << endl << "4 7 4 8 4 9" << endl;
		cout << "Remember: Ships can only be placed vertically or horizontally. Notice the pattern on the example input. Since all 'x' values are equal, the ship is placed vertically." << endl;
		for (j = 0; j < SHIP_SIZES[i]; j++) {
			cin >> col >> row;
			while ((col >= 10) || (col <= -1)) {
				cout << "Oops! Was that a typo? Re-enter the value you want in the range from [0,9] for columns." << endl;
				cin >> col;
			}
			while ((row >= 10) || (row <= -1)) {
				cout << "Oops! Was that a typo? Re-enter the value you want in the range from [0,9] for rows." << endl;
				cin >> row;
			}
			while (grid[row][col] != '~') {
				cout << "There is already a ship in (" << col << ", " << row << "). Please re-enter what value you want (x, y)" << endl;
				cin >> col >> row;
			}
			if (j == 0) {
				temprow = row;
				tempcol = col;
			}
			else if (j == 1) {
				temprow2 = row;
				tempcol2 = col;
			}
			/*
			if (temprow2 >= 0) {
				if ((temprow != temprow2) || (tempcol != tempcol2)) {
					cout << "You are not placing the ship in a vertical or horizontal orientation. Please try again." << endl;
					i--;
					break;
				}
			}
			*/
			while ((temprow == temprow2) && (tempcol == tempcol2)) {
				cout << "Your input is invalid. Would you like to re-enter a value for rows (r) or columns (c)?" << endl;
				cin >> choice;
				while ((choice != 'c') && (choice != 'r')) {
					cout << "You have a hard time entering valid inputs. Type r or c and hit enter." << endl;
					cin >> choice;
				}
				if (choice == 'c') {
					cout << "What value do you want for columns?" << endl;
					cin >> col;
					tempcol2 = col;
				}
				else if (choice == 'r') {
					cout << "What value do you want for rows?" << endl;
					cin >> row;
					temprow2 = row;
				}
				cout << "Alright, continue entering the rest of the values for your ship." << endl;
			}
			if (temprow == temprow2) { // fixes any mistypes or anything out of bounds after the first two values are entered1
			// once two values are the same, the orientation of the ship is assumed and will automatically correct user input with this statement
			// the user will still enter values, but this corrects it
				row = temprow;
			}
			else if (tempcol == tempcol2) {
				col = tempcol;
			}
			grid[row][col] = SHIP_SYMBOLS[i];
			// Personally, I found it most helpful to have the grid displayed after each character is placed as I think entering in coordinates can be confusing. I wanted someone that has no idea what this program is to open it up and use it without me explaining anything.
			cout << "Here is what you have so far: " << endl;
			displayGrid(grid);
		}
	}
	
}

void displayRules() {
	cout << "Welcome to Battleship. Here are the rules:" << endl;
	cout << "1. This is a two player game." << endl;
	cout << "2. You will play as Player 1 and the computer is Player 2." << endl;
	cout << "3. Players alternate turns and guess the coordinates of their opponent's ship." << endl;
	cout << "4. The player that gets all their ships \"hit\" loses." << endl;
	cout << "5. And uh, good luck." << endl;
}

int checkForSunkShip(char grid[][BOARD_SIZE]) {
	int i, j, k;
	int numSunk = 0, numFloat = 0;
	char sunkShip;
	for (k = 0; k < NUM_SHIPS; k++) {
		for (i = 0; i < BOARD_SIZE; i++) {
			for (j = 0; j < BOARD_SIZE; j++) {
				if (grid[i][j] == SHIP_SYMBOLS[k]) {
					numFloat++;
				}
			}
		}
		if (numFloat == 0) {
			sunkShip = SHIP_SYMBOLS[k];
		}
	}
	return numSunk;
}

void playerShoot(char p2Board[][BOARD_SIZE], char visibleBoard[][BOARD_SIZE], ofstream& outputFile, int& hit1, int& miss1) {
	cout << "Enter the coordinates you want to shoot (x,y)" << endl;
	
	int coordX, coordY;
	cin >> coordX >> coordY;
	while (p2Board[coordY][coordX] == 'X') {
		cout << "You already took a shot there. Please re-enter coordinates." << endl;
		cin >> coordX >> coordY;
	}
	if (p2Board[coordY][coordX] == '~') {
		cout << "You missed!" << endl;
		visibleBoard[coordY][coordX] = 'm';
		outputFile << "P1: miss" << endl;
		miss1++;
	}
	else {
		cout << "DIRECT HIT!" << endl;
		visibleBoard[coordY][coordX] = 'X';
		p2Board[coordY][coordX] = 'X';
		outputFile << "P1: hit" << endl;
		hit1++;
	}
}

void computerShoot(char grid[][BOARD_SIZE], ofstream& outputFile, int& hit2, int& miss2) {
	int randX, randY;
	
	randX = rand() % 10;
	randY = rand() % 10;
	while (grid[randX][randY] == '*') {
		randX = rand() % 10;
		randY = rand() % 10;
	}
	if (grid[randX][randY] != '~') {
		cout << "YOU'VE BEEN HIT. DEAR GOD." << endl << endl;
		grid[randX][randY] = '0';
		outputFile << "P2: hit" << endl;
		hit2++;
	}
	else {
		cout << "The computer missed. Thank the lord." << endl;
		outputFile << "P2: miss" << endl;
		miss2++;
	}
}

void randomizeBoard(char grid[][BOARD_SIZE]) {
	int randX, randY, orientation, coordX = 0, coordY = 0, i, j, k;
		for (i = 0; i < NUM_SHIPS; i++) {
			randX = rand() % 10;
			randY = rand() % 10;
			orientation = rand() % 2; // if 0, horizontal. if 1, vertical
			for (j = 0; j < SHIP_SIZES[i]; j++) {
				if (orientation == 1) { // HORIZONTAL ORIENTATION
					while ((randX - SHIP_SIZES[i]) > -1) {
						randX = rand() % 10;
					}
					for (k = 0; k < SHIP_SIZES[i]; k++) {
						if (grid[randX - k][randY] == '~') {
							grid[randX - k][randY] = SHIP_SYMBOLS[i];
						}
						else {
							break;
						}
					}
				}
				else if (orientation == 0) { // VERTICAL ORIENTATION
					while ((randY - SHIP_SIZES[i]) < -1) {
						randY = rand() % 10;
					}
					for (k = 0; k < SHIP_SIZES[i]; k++) {
						if (grid[randX][randY - k] == '~') {
							grid[randX][randY - k] = SHIP_SYMBOLS[i];
						}
						else {
							break;
						}
					}
				}
			}
		}
}

int initialTurn() {
	int randNum;
	randNum = rand() % 2;
	return randNum;
}
void makeSolution(char grid[][BOARD_SIZE]) {
	
}
bool isGameWon(char grid[][BOARD_SIZE]) {
	bool allSank;
	int i, j, totalSank = 0;
	for (i = 0; i < BOARD_SIZE; i++) {
		for (j = 0; j < BOARD_SIZE; j++) {
			if (grid[i][j] == 'X') {
				totalSank++;
			}
		}
	}
	if (totalSank == 17) {
		allSank = true;
	}
	else {
		allSank = false;
	}
	return allSank;
}

