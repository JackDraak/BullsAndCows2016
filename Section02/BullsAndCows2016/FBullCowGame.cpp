#include "FBullCowGame.h"

using int32 = int;

FBullCowGame::FBullCowGame()					{ FBullCowGame::Reset(); }
void FBullCowGame::IncrementTry()				{ MyCurrentTry++; return; }
int32 FBullCowGame::GetMaxTries() const			{ return MyMaxTries; }
int32 FBullCowGame::GetCurrentTry() const		{ return MyCurrentTry; }
bool FBullCowGame::IsGameWon() const			{ return false; }
bool FBullCowGame::CheckGuessValidity(FString)	{ return false; }
void FBullCowGame::Reset()
{
	constexpr int32 MAX_TRIES = 8;
	MyMaxTries = MAX_TRIES;
	MyCurrentTry = 1;
	return;
}