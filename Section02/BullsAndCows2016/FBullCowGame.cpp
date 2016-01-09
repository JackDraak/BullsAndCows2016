#include "FBullCowGame.h"

void FBullCowGame::ExpendGuess()					{ MyCurrentTry++; return; }
void FBullCowGame::Reset()						{ MyCurrentTry = 1;	return; }
int FBullCowGame::GetMaxTries()						{ return MyMaxTries; }
int FBullCowGame::GetCurrentTry()					{ return MyCurrentTry; }
bool FBullCowGame::CheckGuessValidity(std::string)			{ return false; }
bool FBullCowGame::IsGameWon()						{ return false; }
std::string FBullCowGame::PickTargetWord()				{ return std::string(); }