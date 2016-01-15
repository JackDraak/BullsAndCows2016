#pragma once
#include <string>

using FString = std::string;
using int32 = int;

class FBullCowGame {
public:
	//constructors
	FBullCowGame();
	void IncrementTry();
	void Reset(); // TODO make a rich return value
	int32 GetMaxTries() const;
	int32 GetCurrentTry() const;
	bool IsGameWon() const;
	bool CheckGuessValidity(FString); // TODO rich return

// ignore this section for now vv work up there instead ^^
private:
	// set values in constructor definition FBullCowGame::Reset().cpp
	int32 MyCurrentTry;
	int32 MyMaxTries;
};