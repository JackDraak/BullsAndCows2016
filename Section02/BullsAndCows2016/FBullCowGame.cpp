#include "FBullCowGame.h"

void FBullCowGame::Reset()
{
	MyCurrentTry = 1;
	return;
}

int FBullCowGame::GetMaxTries()
{
	return MyMaxTries;
}

std::string FBullCowGame::PickTargetWord()
{
	return std::string();
}

int FBullCowGame::GetCurrentTry()
{
	return MyCurrentTry;
}

void FBullCowGame::ExpendGuess()
{
	MyCurrentTry++;
}

bool FBullCowGame::IsGameWon()
{
	return false;
}

bool FBullCowGame::CheckGuessValidity(std::string)
{
	return false;
}
