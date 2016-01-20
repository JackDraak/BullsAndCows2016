/*	FBullCowGame.cpp
*	created by Jack Draak
*	as tutored by Ben Tristem
*	Jan.2016 pre-release 0.9.1b
*
*	This class handles the mechanics of the Bull Cow Game, i.e.
*		GetGameWord,         IsWordAlpha,
*		GetScore,            LevelUp,
*		GetCurrentTry,       ScoreUp,
*		GetLevel,            CheckGuessValidity,
*		GetGameWordLength,   SubmitValidGuess,
*		IsGameWon,           Reset,
*		IncrementTry,        GuessMaxTries,
*		IsWordIsogram,       SelectGameWordForLevel,
*
*	I/O functions are handled in the Main.cpp class
*/
#pragma once
#include "FBullCowGame.h"
#include <map>
#include <ctime>

#define TMap std::map

using int32 = int;

FBullCowGame::FBullCowGame()                    { FBullCowGame::Reset(); }
FString FBullCowGame::GetGameWord() const       { return MyGameWord; }
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

// True/False: Alphabetic
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

// upon reciept of a valid* guess, increments turn and returns count
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
		// game [DIFFICULTY Tuning: Part C] because the quicker a player score goes up, the more quickly difficulty goes up
		FBullCowGame::ScoreUp(MyLevel + (FBullCowGame::GetMaxTries() - MyCurrentTry));
		// breakpoint here to see the result of the score calulation

		// game [DIFFICULTY Tuning: Part B] can be acomplished here; the curve { 3, 9, 27, 71, 223, ... } stages leveling:
		// this could certainly be done with more eloquence., I think...
		if (MyLevel == 0 && MyScore > 3) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 1 && MyScore > 6) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 2 && MyScore > 12) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 3 && MyScore > 24) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 4 && MyScore > 48) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 5 && MyScore > 96) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 6 && MyScore > 192) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 7 && MyScore > 384) { FBullCowGame::LevelUp(); }
		FBullCowGame::bMyWin = true;
	}
	return BullCowCount;
}

// Initialize a new game state (overloaded: if game InPlay set-up for a new turn)
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
	return;
}

// use a map to correlate Word.length and MaxTries [DIFFICULTY Tuning: Part A]
int32 FBullCowGame::GetMaxTries() const
{
	TMap<int32, int32>WordLengthToMaxTries 
	{
		{ 3, 5 },   { 11, 24 },
		{ 4, 6 },   { 12, 20 },
		{ 5, 8 },   { 13, 16 },
		{ 6, 11 },  { 14, 12 },
		{ 7, 15 },  { 15, 10 },
		{ 8, 20 },  { 16, 8 },
		{ 9, 26 },  { 17, 6 },
		{ 10, 32 }, { 18, 3 }
	};
	return WordLengthToMaxTries[MyGameWord.length()];
}

// based on the player level, select an apropriate random word from the map
FString FBullCowGame::SelectGameWordForLevel()
{
	FString Words_0[15] = { "pat", "eat", "rat", "and", "mat", "cat", "ate", "sat", "fat", "bat", "our", "the", "new", "had", "day" };
	FString Words_1[15] = { "sand", "pair", "raid", "care", "sock", "fair", "hair", "land", "walk", "talk", "same", "dart", "this", "from", "suit" };
	FString Words_2[15] = { "toads", "brick", "stick", "roads", "stand", "trick", "thick", "loads", "talks", "locks", "thing", "miles", "lives", "facts", "cloth" };
	FString Words_3[15] = { "jaunts", "abound", "tricks", "bricks", "crawls", "crowns", "around", "orgasm", "bounty", "gizmos", "travel", "wealth", "second", "curled", "loving" };
	FString Words_4[15] = { "jukebox", "ziplock", "lockjaw", "quickly", "question", "jaybird", "jackpot", "quicken", "quicker", "imports", "clothes", "spectrum", "jockeys", "subject", "cliquey" };
	FString Words_5[15] = { "hospitable", "background", "campground", "greyhounds", "infamously", "slumbering", "shockingly", "duplicates", "authorizes", "farsighted", "binoculars", "destroying", "chlothespin", "introduces", "nightmares" };
	FString Words_6[15] = { "workmanship", "palindromes", "speculation", "trampolines", "personality", "abolishment", "atmospheric", "playgrounds", "backgrounds", "countryside", "birthplaces", "precautions", "regulations", "subcategory", "documentary" };
	FString Words_7[15] = { "thunderclaps", "misconjugated", "unproblematic", "unprofitable", "questionably", "packinghouse", "upholstering", "lexicography", "malnourished", "subordinately", "counterplays", "multipronged", "unforgivable", "subvocalized", "exhaustingly" };
	FString Words_8[15] = { "subdermatoglyphic", "uncopyrightable", "ambidextrously", "hydromagnetics", "pseudomythical", "flamethrowing", "unsympathized", "unpredictably", "multibranched", "subformatively", "hydropneumatic", "consumptively", "computerizably", "unmaledictory", "ambidextrously" };

	std::srand(time(NULL)); // this may appear extraneous, but seems to be required for proper random results in `frame` below
	int32 frame = rand() % 15;
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
	case 6:
		MyGameWord = Words_6[frame];
		break;
	case 7:
		MyGameWord = Words_7[frame];
		break;
	case 8:
		MyGameWord = Words_8[frame];
		break;
	default:
		MyGameWord = Words_0[frame];
		break;
	}
	return MyGameWord; // breakpoint here to cheat and glimpse the next game word
}