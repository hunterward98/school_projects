// Hunter Ward
// CPSC121
// Programming assignment 6, due 10/25/2018
// This program is a game of chance, known as "Pass the Pigs"

#ifndef PA6_H
#define PA6_H

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <string>

using namespace std;

void displayGameRules(); // displays the game rules to the console
int getValidWinningScore(); // prompts for a score to play to
bool isValidWinningScore(int score); // checks to see if the entered winning score is an integer
string rollPig(); // rolls the pigs and determines each of their positions
string determineRollResult(string pig1, string pig2); // determines the roll result based on pig's positions
int calculateTotalRollPoints(string pig1, string pig2, string rollResult); // determines total points roll based on pigs positions

#endif
