#pragma once
#include <string>

class FBullCowGame {
public:
	void Reset(); // TODO make a rich return value
	int GetMaxTries();
	std::string PickTargetWord();
	int GetCurrentTry();
	void ExpendGuess();
	bool IsGameWon();
	bool CheckGuessValidity(std::string);


// ignore this section for now vv work up there instead ^^
private:
	int MyCurrentTry = 1;
	int MyMaxTries = 5;
};