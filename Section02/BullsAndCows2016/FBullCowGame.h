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

	FString GetGameWord() const;
	int32 GetMaxTries() const;
	int32 GetCurrentTry() const;
	int32 GetGameWordLength() const;
	bool IsGameWon() const;

	EGuessStatus CheckGuessValidity(FString) const;
	
	FBullCowCount SubmitValidGuess(FString);
	void IncrementTry();
	void Reset(); // TODO rich return
	
// ignore this section for now vv work up there instead ^^
private:
	// set values in constructor definition FBullCowGame::Reset().cpp
	int32 MyCurrentTry;
	int32 MyMaxTries;
	bool bMyWin;
	FString MyGameWord;

	bool IsIsogram(FString) const;
};