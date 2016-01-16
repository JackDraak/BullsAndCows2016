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

enum class EGuessStatus
{
	Invalid_Status,
	OK,
	Not_Isogram,
	Not_Lowercase,
	Length_Mismatch,
};

class FBullCowGame {
public:
	//constructors
	FBullCowGame();

	int32 GetMaxTries() const;
	int32 GetCurrentTry() const;
	int32 GetGameWordLength() const;

	EGuessStatus CheckGuessValidity(FString) const;
	bool IsGameWon() const;

	void IncrementTry();
	void Reset(); // TODO rich return
	
	FBullCowCount SubmitGuess(FString);

// ignore this section for now vv work up there instead ^^
private:
	// set values in constructor definition FBullCowGame::Reset().cpp
	int32 MyCurrentTry;
	int32 MyMaxTries;
	FString MyGameWord;
};