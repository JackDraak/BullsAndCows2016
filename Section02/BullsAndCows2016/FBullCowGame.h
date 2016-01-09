#pragma once
#include <string>

class FBullCowGame {
public:
	void ExpendGuess();
	void Reset(); // TODO make a rich return value
	int GetMaxTries();
	int GetCurrentTry();
	bool CheckGuessValidity(std::string);
	bool IsGameWon();
	std::string PickTargetWord();


// ignore this section for now vv work up there instead ^^
private:
	int MyCurrentTry = 1;
	int MyMaxTries = 5;
};