#pragma once
#include <string>

class FBullCowGame {
public:
	//constructors
	FBullCowGame();
	void IncrementTry();
	void Reset(); // TODO make a rich return value
	int GetMaxTries() const;
	int GetCurrentTry() const;
	bool IsGameWon() const;
	bool CheckGuessValidity(std::string);

// ignore this section for now vv work up there instead ^^
private:
	// set values in constructor definition FBullCowGame::Reset().cpp
	int MyCurrentTry;
	int MyMaxTries;
};