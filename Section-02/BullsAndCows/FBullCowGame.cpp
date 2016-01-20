#pragma once
#include "FBullCowGame.h"
#include <map>
#include <ctime>

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
void FBullCowGame::LevelUp()                    { MyLevel++; return; }
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
		FBullCowGame::ScoreUp(FBullCowGame::GetLevel() + (MyMaxTries - MyCurrentTry) +1);
		if (MyLevel == 0 && MyScore > 3) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 1 && MyScore > 9) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 2 && MyScore > 27) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 3 && MyScore > 71) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 4 && MyScore > 223) { FBullCowGame::LevelUp(); }
		FBullCowGame::bMyWin = true;
	}
	return BullCowCount;
}

// Initialize a new game state (overloading: if game InPlay set for a new round)
void FBullCowGame::Reset()
{
	if (bNewPLay) 
	{
		MyScore = 0;
		MyLevel = 0;
	}
	else { bNewPLay = false; }
	bMyWin = false;
	MyCurrentTry = 1;
	FString MyGameWord = SelectGameWordForLevel();
	constexpr int32 DEFAULT_MAX_TRIES = 4;
	MyMaxTries = DEFAULT_MAX_TRIES;
	if (int32(MyGameWord.length()) >= DEFAULT_MAX_TRIES) { MyMaxTries = int32(MyGameWord.length()); }
	return;
}

// based on the player level, select an apropriate random word from the map
FString FBullCowGame::SelectGameWordForLevel()
{
	FString Words_0[10] = { "pat", "eat", "rat", "and", "mat", "cat", "ate", "sat", "fat", "bat" };
	FString Words_1[10] = { "sand", "pair", "raid", "care", "sock", "fair", "hair", "land", "walk", "talk" };
	FString Words_2[10] = { "toads", "brick", "stick", "roads", "stand", "trick", "thick", "loads", "talks", "locks" };
	FString Words_3[10] = { "jaunts", "abound", "tricks", "bricks", "crawls", "crowns", "around", "orgasm", "bounty", "gizmos" };
	FString Words_4[10] = { "workmanship", "palindromes", "speculation", "trampolines", "personality", "abolishment", "atmospheric", "playgrounds", "backgrounds", "countryside" };
	FString Words_5[10] = { "thunderclaps", "misconjugated", "unproblematic", "unprofitable", "questionably", "packinghouse", "upholstering", "lexicography", "malnourished", "subordinately" };

	int32 frame = rand() % 10;
	if (frame < 0) { frame = 0; } // TODO overkill much?
	if (frame > 9) { frame = 9; } // do you even code, bro?
	switch (MyLevel)
	{
	case 0:
		MyGameWord = Words_0[frame];
		break;
	case 1:
		MyGameWord = Words_1[frame];
		break;
	case 2:
		MyGameWord = Words_2[frame];
		break;
	case 3:
		MyGameWord = Words_3[frame];
		break;
	case 4:
		MyGameWord = Words_4[frame];
		break;
	case 5:
		MyGameWord = Words_5[frame];
		break;
	default:
		MyGameWord = Words_0[frame];
		break;
	}
	return MyGameWord;
}