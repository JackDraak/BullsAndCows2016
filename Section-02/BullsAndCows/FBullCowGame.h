/*	FBullCowGame.h
	created by Jack Draak
	as tutored by Ben Tristem
	Jan.2016 pre-release version 0.9.57

	This header file is included by both:
		Main.cpp
		FBullCowGame.cpp

	I/O functions are handled in the Main.cpp class
	The FBullCowGame class handles the mechanics of the Bull Cow Game

	See Main.cpp for further description.
*/
#pragma once
#include <iostream>
#include <random>

// required for UnrealEngine-friendly syntax
using FString = std::string;
using int32 = int;

struct FBullCowCounts
{
	int32 Bulls = 0;
	FString Bulltips = "";
	int32 Cows = 0;
	FString Cowtips = "";
	FString Hashtips = "";
};

// possible return status' when processing Guess input:
enum class EGuessStatus
{
	Invalid_Status,
	Length_Mismatch,
	Not_Alpha,
	Not_Isogram,
	OK
};

class FBullCowGame {
	std::mt19937 engine = std::mt19937{ std::random_device{}() }; //seed with `std::random_device{}()` for proper randomness

public:
	//constructors
	FBullCowGame();

	EGuessStatus CheckGuessValidity(const FString&) const;
	int32 GetBulls() const;
	int32 GetCows() const;
	int32 GetDefeats() const;
	FString GetGuess() const;
	FString GetIsogram() const;
	int32 GetLevel() const;
	int32 GetMaxTries() const;
	int32 GetMisses() const;
	int32 GetScore() const;
	int32 GetTurn() const;
	int32 GetWins() const;
	bool IsPhaseWon() const;

	void IncrementDefeats();
	void IncrementMisses();
	void IncrementTry();
	void IncrementWins();
	void LevelUp();
	void ScoreUp(int32);
	void Reset();

	FString SelectIsogramForLevel();
	FBullCowCounts ProcessValidGuess(const FString&);

private:
	// set values in constructor definition FBullCowGame::Reset()
	bool bDoneOnce;
	bool bGuessMatches;
	int32 MyCurrentTurn;
	int32 MyDefeats;
	FString MyGuess;
	FString MyIsogram;
	int32 MyLevel;
	int32 MyMaxTries;
	int32 MyMisses;
	int32 MyScore;
	int32 MyTotalBull;
	int32 MyTotalCow;
	int32 MyWins;

	bool IsWordAlpha(const FString&) const;
	bool IsWordIsogram(const FString&) const;
	int32 PositiveExponentResult(int32 Base, const int32& Exponent);
};
