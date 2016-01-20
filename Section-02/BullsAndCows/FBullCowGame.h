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

// possible return status' when processing Guess input:
enum class EGuessStatus
{
	Invalid_Status,
	OK,
	Not_Isogram,
	Not_Alpha,
	Length_Mismatch,
};

class FBullCowGame {
public:
	//constructors
	FBullCowGame();

	FString GetGameWord() const;
	int32 GetMaxTries() const;
	int32 GetScore() const;
	int32 GetCurrentTry() const;
	int32 GetLevel() const;
	int32 GetGameWordLength() const;
	bool IsGameWon() const;

	EGuessStatus CheckGuessValidity(FString) const;

	FBullCowCount SubmitValidGuess(FString);
	FString SelectGameWordForLevel();
	void IncrementTry();
	void LevelUp();
	void ScoreUp(int32);
	void Reset();

private:
	// set values in constructor definition FBullCowGame::Reset() %0.cpp
	FString MyGameWord;
	int32 MyCurrentTry;
	int32 MyScore;
	int32 MyLevel;
	int32 MyMaxTries;
	bool bMyWin;
	bool bNewPLay;

	bool IsWordIsogram(FString) const;
	bool IsWordAlpha(FString) const;
};