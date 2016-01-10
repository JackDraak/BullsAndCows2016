#include "FBullCowGame.h"

FBullCowGame::FBullCowGame()					{ FBullCowGame::Reset(); }
void FBullCowGame::IncrementTry()				{ MyCurrentTry++; return; }
int FBullCowGame::GetMaxTries() const				{ return MyMaxTries; }
int FBullCowGame::GetCurrentTry() const				{ return MyCurrentTry; }
bool FBullCowGame::IsGameWon() const				{ return false; }
bool FBullCowGame::CheckGuessValidity(std::string)		{ return false; }
void FBullCowGame::Reset()
{
	constexpr int MAX_TRIES = 8;
	MyMaxTries = MAX_TRIES;
	MyCurrentTry = 1;
	return;
}