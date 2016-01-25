/*	FBullCowGame.h
	created by Jack Draak
	as tutored by Ben Tristem
	Jan.2016 pre-release version 0.9.52

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
	int32 Cows = 0;
	int32 Bulls = 0;
	FString Cowtips = "";
	FString Bulltips = "";
	FString Hashtips = "";
};

// possible return status' when processing Guess input:
enum class EGuessStatus
{
	OK,
	Not_Alpha,
	Not_Isogram,
	Invalid_Status,
	Length_Mismatch
};

class FBullCowGame {
	std::mt19937 engine = std::mt19937{ 1729 }; //seeded using number for debugging. Expected results - bale, locks....
												//seed with std::random_device{}() for proper randomness
public:
	//constructors
	FBullCowGame();

	EGuessStatus CheckGuessValidity(const FString&) const;
	FString GetIsogram() const;
	int32 GetMaxTries() const;
	FString GetGuess() const;
	int32 GetDefeats() const;
	int32 GetMisses() const;
	bool IsPhaseWon() const;
	int32 GetLevel() const;
	int32 GetScore() const;
	int32 GetBulls() const;
	int32 GetTurn() const;
	int32 GetWins() const;
	int32 GetCows() const;

	FBullCowCounts ProcessValidGuess(const FString&);
	FString SelectIsogramForLevel();
	void IncrementDefeats();
	void IncrementMisses();
	void IncrementWins();
	void IncrementTry();
	void ScoreUp(int32);
	void LevelUp();
	void Reset();

private:
	// set values in constructor definition FBullCowGame::Reset()
	int32 MyWins;
	int32 MyLevel;
	int32 MyScore;
	int32 MyMisses;
	bool bDoneOnce;
	FString MyGuess;
	int32 MyDefeats;
	int32 MyMaxTries;
	int32 MyTotalCow;
	int32 MyTotalBull;
	FString MyIsogram;
	bool bGuessMatches;
	int32 MyCurrentTurn;

	bool IsWordAlpha(const FString&) const;
	bool IsWordIsogram(const FString&) const;
	int32 PositiveExponentResult(int32 Base, int32 Exponent);
};
