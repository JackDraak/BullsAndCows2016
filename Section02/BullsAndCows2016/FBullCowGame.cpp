#include "FBullCowGame.h"

void FBullCowGame::ExpendGuess()						{ MyCurrentTry++; return; }
void FBullCowGame::Reset()								{ MyCurrentTry = 1; return; }
int FBullCowGame::GetMaxTries() const					{ return MyMaxTries; }
int FBullCowGame::GetCurrentTry() const				{ return MyCurrentTry; }
bool FBullCowGame::IsGameWon() const					{ return false; }
bool FBullCowGame::CheckGuessValidity(std::string)		{ return false; }
std::string FBullCowGame::PickTargetWord()				{ return std::string(); }
