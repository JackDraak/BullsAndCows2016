#pragma once
#include "FBullCowGame.h"
#include <map>
#define TMap std::map

using int32 = int;

FBullCowGame::FBullCowGame()                    { FBullCowGame::Reset(); }
FString FBullCowGame::GetGameWord() const       { return MyGameWord; }
int32 FBullCowGame::GetMaxTries() const         { return MyMaxTries; }
int32 FBullCowGame::GetScore() const            { return MyScore; }
int32 FBullCowGame::GetCurrentTry() const       { return MyCurrentTry; }
int32 FBullCowGame::GetLevel() const            { return MyLevel; }
int32 FBullCowGame::GetGameWordLength() const   { return (MyGameWord.length()); }
bool FBullCowGame::IsGameWon() const            { return bMyWin; }
void FBullCowGame::IncrementTry()               { MyCurrentTry++; return; }
void FBullCowGame::ScoreUp(int32 Score)         { MyScore = MyScore + Score; }

// True/False: Isogram
bool FBullCowGame::IsWordIsogram(FString Word) const
{
	if (Word.length() <= 1) { return true; }
	TMap<char, bool> LetterSeen;
	for (auto Letter : Word)
	{
		Letter = tolower(Letter); // one of two places tolower() is called, see also SubmitValidGuess()
		if (!LetterSeen[Letter])
		{
			LetterSeen[Letter] = true;
		}
		else { return false; }
	}
	return true;
}

// if there are characters in the Word which are not alphabetic, return false, otherwise return true
bool FBullCowGame::IsWordAlpha(FString Word) const
{
	for (int32 WordChar = 0; WordChar < int32(Word.length()); WordChar++)
	{
		if (!isalpha(Word[WordChar])) return false;
	}
	return true; 
}

// ensure the entered guess is alphabetic, correct # of letters, etc...
EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	if (Guess.length() != GetGameWordLength())  { return EGuessStatus::Length_Mismatch; }
	else if (!IsWordIsogram(Guess))             { return EGuessStatus::Not_Isogram; }
	else if (!IsWordAlpha(Guess))               { return EGuessStatus::Not_Alpha; }
	else                                        { return EGuessStatus::OK; }
}

// Upon reciept of a valid* guess, increments turn and returns count
FBullCowCount FBullCowGame::SubmitValidGuess(FString Guess)
{
	FBullCowCount BullCowCount;

	int32 GameWordLength = GetGameWordLength();
	for (int32 MyGameWordChar = 0; MyGameWordChar < GameWordLength; MyGameWordChar++)
	{
		for (int32 GuessChar = 0; GuessChar < GameWordLength; GuessChar++)
		{
			char GameWordChar = MyGameWord[MyGameWordChar];
			char GuessWordChar = tolower(Guess[GuessChar]); // NOTE this is the other place I use tolower() [see also IsWordIsogram()]
			if (GuessWordChar == GameWordChar)
			{
				if (MyGameWordChar == GuessChar) { BullCowCount.Bulls++; }
				else { BullCowCount.Cows++; }
			}
		}
	}
	if (BullCowCount.Bulls == GameWordLength) 
	{
		FBullCowGame::ScoreUp(FBullCowGame::GetLevel() +2);
		FBullCowGame::bMyWin = true;
	}
	return BullCowCount;
}

// Initialize a new game state (overloading for each new game word selection?)
void FBullCowGame::Reset()
{
	bMyWin = false;
	if (bNewPLay)
	{
		MyScore = 0;
		MyLevel = 0;
	}
	else { bNewPLay = false; }

	MyCurrentTry = 1;
	constexpr int32 MAX_TRIES = 4;
	const FString GAME_WORD = "abound";
	MyGameWord = GAME_WORD; // TODO select randomly from an array of words, remove when complete

	MyGameWord = SelectGameWordForLevel();

	MyMaxTries = MAX_TRIES;
	if (int32(MyGameWord.length()) >= MAX_TRIES) { MyMaxTries = int32(MyGameWord.length()); }

	return;
}

// based on the player level, select an apropriate random word from the map
FString FBullCowGame::SelectGameWordForLevel()
{
	FString MAGIC_WORD = "string"; // TODO make this obsolete

	// setup a map of int:string pairs, where int is the level of the game word
	TMap<int32, FString> LevelWordIndex
	{ 
		{ 0, "pat" }, {1, "sand"}, {2, "toads"}, {3, "jaunts"}, {4, "workmanship"},
		{ 0, "eat" }, {1, "pair"}, {2, "brick"}, {3, "abound"}, {4, "palindromes"},
		{ 0, "rat" }, {1, "rare"}, {2, "stick"}, {3, "tricks"}, {4, "speculation"},
		{ 0, "and" }, {1, "care"}, {2, "roads"}, {3, "bricks"}, {4, "tramploines"},
		{ 0, "mat" }, {1, "sock"}, {2, "stand"}, {3, "crawls"}, {4, "personality"},
		{ 0, "cat" }, {1, "fair"}, {2, "trick"}, {3, "crowns"}, {4, "abolishment"},
		{ 0, "ate" }, {1, "hair"}, {2, "thick"}, {3, "around"}, {4, "atmospheric"},
		{ 0, "sat" }, {1, "land"}, {2, "loads"}, {3, "orgasm"}, {4, "playgrounds"},
		{ 0, "fat" }, {1, "walk"}, {2, "talks"}, {3, "bounty"}, {4, "backgrounds"},
		{ 0, "bat" }, {1, "talk"}, {2, "locks"}, {3, "gizmos"}, {4, "countryside"},
	};

	// select a random word from the player level to return
	return MAGIC_WORD;
}