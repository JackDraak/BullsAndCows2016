#pragma once
#include <string>

using FString = std::string;
using int32 = int;

// variables initialized to zero
struct FBullCowCount
{
	int32 Bulls = 0;
	int32 Cows = 0;
};

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
	
	// *assuming valid guess, count bulls and cows and IncrementTry()
	FBullCowCount SubmitGuess(FString);

// ignore this section for now vv work up there instead ^^
private:
	// set values in constructor definition FBullCowGame::Reset().cpp
	int32 MyCurrentTry;
	int32 MyMaxTries;
	FString MyGameWord;
};