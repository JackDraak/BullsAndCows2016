/*	FBullCowGame.h
*	created by Jack Draak
*	as tutored by Ben Tristem
*	Jan.2016 pre-release version 0.9.43
*
*	GAME-LOGIC:
*	This is a `Mastermind`-style word guessing game using a small
*	dictionary of isograms, tied to a scoring and leveling system
*	designed to provide increasingly difficult challenges to one
*	or more players working co-oepratively.
*
*	This header file is included by both:
*		Main.cpp
*		FBullCowGame.cpp
*
*	I/O functions are handled in the Main.cpp class
*	The FBullCowGame class handles the mechanics of the Bull Cow Game
*/
#pragma once
#include <iostream>
#include <ctime>

// required for UnrealEngine-friendly syntax
using FString = std::string;
using int32 = int;

struct FBullCowCounts
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
	Length_Mismatch
};

class FBullCowGame {
public:
	//constructors
	FBullCowGame();

	FString GetRankedIsogram() const;
	FString GetGuess() const;
	int32 GetIsogramLength() const;
	int32 GetMaxTries() const;
	int32 GetTurn() const;
	int32 GetLevel() const;
	int32 GetScore() const;
	int32 GetMisses() const;
	int32 GetDefeats() const;
	int32 GetWins() const;
	int32 GetBulls() const;
	int32 GetCows() const;
	bool IsPhaseWon() const;
	EGuessStatus CheckGuessValidity(FString) const;

	FBullCowCounts ProcessValidGuess(FString);
	void TallyBullsAndCows(const int32 &GameWordLength, FString &Guess, FBullCowCounts &BullCowCounts);
	FString SelectIsogramForLevel();
	void Reset();
	void IncrementTry();
	void IncrementMisses();
	void IncrementDefeats();
	void IncrementWins();
	void BoostBullScore(int32);
	void LevelUp();
	void ScoreUp(int32);

private:
	// set values in constructor definition FBullCowGame::Reset()
	FString MyIsogram;
	FString MyGuess;
	int32 MyMaxTries;
	int32 MyCurrentTurn;
	int32 MyLevel;
	int32 MyScore;
	int32 MyMisses;
	int32 MyDefeats;
	int32 MyWins;
	int32 MyTotalBull;
	int32 MyTotalCow;
	bool bGuessMatches;
	bool bDoneOnce;

	int32 PositivePowerResult(int32 Number, int32 Exponent);
	bool IsWordIsogram(FString) const;
	bool IsWordAlpha(FString) const;
};
