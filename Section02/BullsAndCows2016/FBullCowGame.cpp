#include "FBullCowGame.h"

// do I format this so it looks nice in Windows or on Git? I've gone with Git, where these form two lovely collumns 

void FBullCowGame::IncrementTry()				{ MyCurrentTry++; return; }
void FBullCowGame::Reset()					{ MyCurrentTry = 1; return; }
int FBullCowGame::GetMaxTries() const				{ return MyMaxTries; }
int FBullCowGame::GetCurrentTry() const				{ return MyCurrentTry; }
bool FBullCowGame::IsGameWon() const				{ return false; }
bool FBullCowGame::CheckGuessValidity(std::string)		{ return false; }
std::string FBullCowGame::PickTargetWord()			{ return std::string(); }
