// Programming assignment 8
// Hunter Ward, CPSC 121
// This program simulates battleship using the console as a UI

#ifndef BATTLESHIP_H
#define BATTLESHIP_H

#include <iostream>
#include <ctime>
#include <cstdlib>
#include <fstream>

using namespace std;

	const int NUM_SHIPS = 5;
	const string SHIP_NAMES[] = {"Carrier", "Battleship", "Cruiser", "Submarine", "Destroyer"};
	const char SHIP_SYMBOLS[] = {'c', 'b', 'r', 's', 'd'};
	const int SHIP_SIZES[] = {5, 4, 3, 3, 2};
	// declare parallel arrays to store ship info
	// data in arrays at index 0 correspond to carrier
	// at index 1 correspond to battleship
	const int BOARD_SIZE = 10;

void displayRules();
void displayGrid(char[][BOARD_SIZE]);
void initializeGrid(char[][BOARD_SIZE]);
void setupP1Board(char[][BOARD_SIZE]);
int checkCharBoard(char[][BOARD_SIZE], bool[]);
bool checkBoard(char[][BOARD_SIZE], bool[]);
int checkForSunkShip(char[][BOARD_SIZE]);
void randomizeBoard(char[][BOARD_SIZE]);
bool checkShot(char[][BOARD_SIZE], int, int);

void playerShoot(char[][BOARD_SIZE], char[][BOARD_SIZE], ofstream&, int&, int&);
void computerShoot(char[][BOARD_SIZE], ofstream&, int&, int&);
int initialTurn();
bool isGameWon(char[][BOARD_SIZE]);

#endif
