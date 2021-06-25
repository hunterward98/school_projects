// Author: Hunter Ward
// Class: Computer Science 1
// File: main.cpp (for battleship.cpp)
// This file implements the main() function that takes care of most of the game logic.
// Function implementations and definitions can be found in the battleship.cpp and battleship.h
// files. This program also outputs game activity to a .log file.

#include "battleship.h"

int main() {
	// logic behind placing a ship on the board
	// make sure ships aren't placed on top of each other
	char p1Board[BOARD_SIZE][BOARD_SIZE], p2Board[BOARD_SIZE][BOARD_SIZE], p2VISIBLE[BOARD_SIZE][BOARD_SIZE], p1Solution[BOARD_SIZE][BOARD_SIZE], p2Solution[BOARD_SIZE][BOARD_SIZE]; // p2VISIBLE is the board that the player can see where they've taken shots and where they have hit their opponents
	char playerChoice = ' ';
	bool correctNumChar[5] = {false, false, false, false, false};
	int numChar = 0, i;
	int hit1 = 0, miss1 = 0, hit2 = 0, miss2 = 0;
	ofstream outputFile;
	
	outputFile.open("battleship.log");
	if (!outputFile.is_open()) {
		cout << "Output file was not opened." << endl;
		exit(-1);
	}
	
	srand(time(0));
	initializeGrid(p1Board);
	initializeGrid(p2Board);
	initializeGrid(p2VISIBLE);
	displayRules();
	// game logic
	cout << "Would you like to have your board randomized (recommended) for you (type r) or enter yourself (type m)" << endl;
	cin >> playerChoice;
	// lets player manually set up or randomize. recommend to randomize. while loop makes sure they entered a valid character.
	while (!((playerChoice == 'r') || (playerChoice == 'm'))) {
		cout << "Please enter 'r' to randomize or 'm' to set up the board yourself." << endl;
		cin >> playerChoice;
	}
	if (playerChoice == 'r') { //randomize player 1's board
		numChar = 0;
		while (numChar < 5) {
			randomizeBoard(p1Board);
			numChar = checkCharBoard(p1Board, correctNumChar);
			if (numChar < 5) {
				for (i = 0; i < NUM_SHIPS; i++) {
					correctNumChar[i] = false;
				}
				initializeGrid(p1Board);
			}
		}	
		cout << "Player 1's board has been randomized." << endl;
		displayGrid(p1Board);
	}
	else if (playerChoice == 'm') { // player 1 will manually set up their board
		setupP1Board(p1Board);
	}

	numChar = 0;
	for (i = 0; i < NUM_SHIPS; i++) {
		correctNumChar[i] = false;
	}
	while (numChar < 5) { // sets up p2 board
		randomizeBoard(p2Board);
		numChar = checkCharBoard(p2Board, correctNumChar);
		if (numChar < 5) {
			for (i = 0; i < NUM_SHIPS; i++) {
				correctNumChar[i] = false;
			}
			initializeGrid(p2Board);
		}
	}
	bool gameWon = false;
	int numCharComp, numCharUser, totalTurns;
	int turn = initialTurn();
	bool p1won = false, p2won = false;
	cout << "Now it's time to play." << endl;
	do {
		while (turn == 1) {
			cout << "Player 2's turn." << endl;
			computerShoot(p1Board, outputFile, hit2, miss2);
			gameWon = isGameWon(p1Board); // detects for game over
			if (gameWon) {
				p2won = true;
			}
			turn = 0; // switches turns
			totalTurns++;
			checkForSunkShip(p2Board);
		}
		while (turn == 0) {
			cout << "Your turn!" << endl;
			displayGrid(p2VISIBLE);
			playerShoot(p2Board, p2VISIBLE, outputFile, hit1, miss1);
			gameWon = isGameWon(p2Board); // detects if game is over
			if (gameWon) {
				p1won = true;
			}
			turn = 1; // switches turns
			totalTurns++;
		}
	} while (!gameWon);
	displayGrid(p2VISIBLE);
	outputFile << "P1 total hits: " << hit1 << endl;
	outputFile << "P2 total hits: " << hit2 << endl;
	outputFile << "P1 total misses: " << miss1 << endl;
	outputFile << "P2 total misses: " << miss2 << endl;
	outputFile << "P1 hit/miss ratio: " << (hit1/miss1) * 100 << "%" << endl;
	outputFile << "P2 hit/miss ratio: " << (hit2/miss2) * 100 << "%" << endl;
	outputFile << "Total hit/miss ratio: " << ((hit1 + hit2)/(miss1 + miss2)) * 100 << "%" << endl;
	outputFile << "Total turns: " << totalTurns;
	outputFile.close();
	
	return 0;
}
