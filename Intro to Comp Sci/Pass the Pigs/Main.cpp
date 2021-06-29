#include "PA6.h"

int main() {
	
	srand(time(0));
	int roll; // roll "amount"
	int score;
	int playerNum = 1;
	int Holdvalue = 0;
	int addvalue = 0;
	int player1score = 0;
	int player2score = 0;
	int score1 = 0;
	int score2 = 0;

	string playQ1;
	string yYes = "yes";
	string pig1;
	string pig2;
	string rollResult;
	
	bool plr1Won = 0; // used to check to see if either player has won
	bool plr2Won = 0;
	bool keepRolling = 1;
	bool ifPlay = 1;

	cout << "welcome to Pass the Pigs" << endl;
	displayGameRules();
	cout << "Would you like to play? (enter yes to play)" << endl;
	cin >> playQ1;
	
	if (playQ1 == yYes) { // checks to see if the player typed "yes" to play and assigns the ifPlay to true if they did
		ifPlay = 1;
		//score = getValidWinningScore(); 
		// Not sure why, but this keeps looping and causes a bug where when the player passes to player 2, player 1 wins automatically.
	}
	else {
		ifPlay = 0;
	}
	
	cout << endl;

	while (ifPlay) {
	
	player1score = 0;
	player2score = 0;

		do { // end of this statement is at the very bottom
		
			keepRolling = 1;
			
			while (keepRolling == 1) {
			
				cout << "Player " << playerNum << " may now roll!" << endl;
	
				pig1 = rollPig();
				pig2 = rollPig();
				rollResult = determineRollResult(pig1, pig2);
				addvalue = calculateTotalRollPoints(pig1, pig2, rollResult);
	
				cout << "Player " << playerNum << " rolled a " << pig1 << " and a " << pig2 << ", which is a " << rollResult << ", which is worth " << addvalue << " points!" << endl;
	
				if(addvalue ==  0){
					Holdvalue = 0;
					keepRolling = 0;
				}
	
				Holdvalue = Holdvalue + addvalue; // amount of current points plus the points they just earned
				cout << Holdvalue << " points are being held by the curent player." << endl;
				cout << "Player 1 is at " << player1score << " points." << endl;
				cout << "Player 2 is at " << player2score << " points." << endl << endl;
	
				if (keepRolling == 1) {
				cout << "Press 0 to pass or 1 to keep rolling" << endl;
				cin >> keepRolling;
				
					if (keepRolling == 0 && plr1Won == 0 && plr2Won == 0) {
						if (playerNum == 1) {
							player1score = player1score + Holdvalue;
							Holdvalue = 0;
						}
						else {
							player2score = player2score + Holdvalue;
							Holdvalue = 0;
						}
						if(player1score >= score) {
							plr1Won = 1;
						}
						else if(player2score >= score) {
							plr2Won = 1;
						}
					}
			
				}
				if (keepRolling == 0){ // switches between player 1 and player 2
					if (playerNum == 2){
						playerNum = 1;
					}
					else {
						playerNum = 2;
					}
				}
		
			}
			
		} while (plr1Won == 0 && plr2Won == 0); // End of Do-while loop
	
		cout << endl;
		cout << "Player" << playerNum << " has won!" << endl;
		cout << "Press 1 if you'd like to play again, or 0 if you'd like to quit." << endl;
		cin >> ifPlay; 
	}//while to play
	
	
		return 0;
}

