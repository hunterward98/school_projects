#include "PA6.h"

// ----------------------------------------------------------------------------------------------

void displayGameRules() { // Displays all info about the game to the console
	cout << "Welcome to Pass the Pigs." << endl << endl;
	cout << "Rules:" << endl << endl;
	cout << "#1 Each player continues rolling until they decide to stop or until the roll a Pig Out." << endl << endl;
	cout << "#2 A Pig Out is when both pigs lay on their sides with one spot facing up and the other facing down." << endl << endl;
	cout << "#3 The game ends when a player reaches the set amount of points." << endl << endl;
	cout << "#4 There are several combinations you can have to get points:" << endl << endl;
	cout << "Sider                   - 1 point" << endl;
	cout << "Double Razorback        - 20 points" << endl;
	cout << "Double Trotter          - 20 points" << endl;
	cout << "Double Snouter          - 40 points" << endl;
	cout << "Double leaning Jowler   - 60 points" << endl;
	cout << "Single Razorback        - 5 points" << endl;
	cout << "Single Trotter          - 5 points" << endl;
	cout << "Single Snouter          - 10 points" << endl;
	cout << "Leaning Jowler          - 15 points" << endl;
	cout << endl;
	cout << "#5 You can mix single combinations for points - so a Single Razorback and Single Snouter is 15 points." << endl << endl;
}

// ----------------------------------------------------------------------------------------------

int getValidWinningScore() { // this function will prompt the players for the number of points they will play until. their input must be an integer

	int score;
	int valid = 100;
	
	while (valid <= 100 && valid > 0) {
		cout << "Please enter the score that you'd like to play to: ";
		cin >> score;
	
		valid = isValidWinningScore(score);
	
		return score;
		
	}
}

// ---------------------------------------------------------------------------------------------

bool isValidWinningScore(int score) {
	
	score = getValidWinningScore();
	if (score > 0 && score <= 100) {
		cout << "Your score is within range. Looks like we can play!" << endl;
		return 1;
	}
	else {
		cout << "Your score is out of range. We can't play." << endl;
		return 0;
	}
}	

// ----------------------------------------------------------------------------------------------

string rollPig() {
	int roll;
	string pig = "";
	string tempStr = "";
	
	getline(cin, tempStr);
	cout << "Press enter to roll a pig!" << endl;
	getline(cin, tempStr);
	
	roll = rand() % 100 + 1;
	
	if (roll == 1) {
		pig = "LEAN";
	}
	else if ((roll > 1) && (roll < 5)) {
		pig = "SNOUT";
	}
	else if ((roll > 4) && (roll < 15)) {
		pig = "TROT";
	}
	else if ((roll > 14) && (roll < 38)) {
		pig = "RZR";
	}
	else if ((roll > 37) && (roll < 69)) {
		pig = "SIDE";
	}
	else if ((roll > 68) && (roll <= 100)) {
		pig = "SIDE-D";
	}
	else { // this statement is used for debugging and ensures there is a proper value
		pig = "ERROR";
		cout << "Logic error!" << endl;
		exit(-1);
	}
	
	return pig;
	
}

// ----------------------------------------------------------------------------------------------

string determineRollResult(string pig1, string pig2) {
	string result;
	
	if ((pig1 == "SIDE" && pig2 == "SIDE") || (pig1 == "SIDE-D" && pig2 == "SIDE-D")) {
		result = "SIDER";
	}
	else if (pig1 == "RZR" && pig2 == "RZR") {
		result = "DBL-RAZOR";
	}
	else if (pig1 == "TROT" && pig2 == "TROT") {
		result = "DBL-TROT";
	}
	else if (pig1 == "SNOUT" && pig2 == "SNOUT") {
		result = "DBL-SNOUT";
	}
	else if (pig1 == "LEAN" && pig2 == "LEAN") {
		result = "DBL-LEAN";
	}
	else if ((pig1 == "SIDE" && pig2 == "SIDE-D") || (pig1 == "SIDE-D" && pig2 == "SIDE")) {
		result = "PIGOUT";
	}
	else {
		result = "MIXED";
	}
	
	return result;
}

// ----------------------------------------------------------------------------------------------

int calculateTotalRollPoints(string pig1, string pig2, string rollResult) {
	int points;
	
	
	
	if (rollResult == "SIDER") {
		points = 1;
	}
	else if (rollResult == "DBL-RAZOR") {
		points = 20;
	}
	else if (rollResult == "DBL-TROT") {
		points = 20;
	}
	else if (rollResult == "DBL-SNOUT") {
		points = 40;
	}
	else if (rollResult == "DBL-LEAN") {
		points = 60;
	}
	else if (rollResult == "PIGOUT") {
		points = 0;
	}
	else if (rollResult == "MIXED") {
		if ((pig1 == "SIDE") || (pig1 == "SIDE-D")) {
			points = 0;
		}
		else if (pig1 == "RZR") {
			points = 5;
		}
		else if (pig1 == "TROT") {
			points = 5;
		}
		else if (pig1 == "SNOUT") {
			points = 10;
		}
		else if (pig1 == "LEAN") {
			points = 15;
		}
		
		if ((pig2 == "SIDE") || (pig2 == "SIDE-D")) {
			points += 0;
		}
		else if (pig2 == "RZR") {
			points += 5;
		}
		else if (pig2 == "TROT") {
			points += 5;
		}
		else if (pig2 == "SNOUT") {
			points += 10;
		}
		else if (pig2 == "LEAN") {
			points += 15;
		}
	}
	cout << "Total points: " << points << endl;

	return points;
}
