#pragma once
#include <string>

class FBullCowGame {
public:
	void ExpendGuess();
	void Reset(); // TODO make a rich return value
	int GetMaxTries() const;
	int GetCurrentTry() const;
	bool IsGameWon() const;
	bool CheckGuessValidity(std::string);	// could be const, methinks, but we have yet to work out what it's doing
	std::string PickTargetWord();


// ignore this section for now vv work up there instead ^^
private:
	int MyCurrentTry = 1;
	int MyMaxTries = 5;
};