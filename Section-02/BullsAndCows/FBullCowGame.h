/*	FBullCowGame.h
	created by Jack Draak
	as tutored by Ben Tristem
	Jan.2016 pre-release version 0.9.7

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

struct FGuessAnalysis
{
	int32 Bulls = 0;
	FString Bulltips = "";
	int32 Cows = 0;
	FString CowTips = "";
	FString HackedCows = "";
	FString Hashtips = "";
};

// possible return status' when validating Guess input:
enum class EGuessQuality
{
	Invalid_Status,
	Length_Mismatch,
	Not_Alpha,
	Not_Isogram,
	OK
};

class FBullCowGame {
	std::mt19937 Entropy = std::mt19937{ std::random_device{}() };

public:
	//constructors
	FBullCowGame();

	EGuessQuality CheckGuessValidity(const FString&) const;
	int32 GetBullsNum() const;
	int32 GetCowsNum() const;
	int32 GetMaxTurns() const;
	int32 GetPhaseLossNum() const;
	int32 GetPhaseWinNum() const;
	FString GetPlayerGuess() const;
	int32 GetPlayerLevel() const;
	int32 GetPlayerScore() const;
	FString GetSecretIsogram() const;
	int32 GetTurnLossNum() const;
	int32 GetTurnNum() const;
	bool IsPhaseWon() const;
	void IncrementPhaseLossNum();
	void IncrementPhaseWinNum();
	void IncrementTurn();
	void IncrementTurnLossNum();
	void ResetPhase();
	void UpLevel();
	void UpScore(int32);

	FGuessAnalysis AnalyzeValidGuess(const FString&);
	FString SelectIsogramForLevel();

private:
	// set values in constructor definition FBullCowGame::Reset()
	bool bNewGameInitialized;
	bool bPlayerGuessMatches;
	int32 PlayerCurrentTurn;
	FString PlayerGuess;
	int32 PlayerLevel;
	int32 PlayerMaxTurns;
	int32 PlayerPhaseLosses;
	int32 PlayerPhaseWins;
	int32 PlayerScore;
	int32 PlayerTotalBullNum;
	int32 PlayerTotalCowNum;
	int32 PlayerTurnLosses;
	FString SecretIsogram;

	int32 CalculateExponent(int32 Base, int32 Exponent);
	bool IsWordAlpha(const FString&) const;
	bool IsWordIsogram(const FString&) const;
};
