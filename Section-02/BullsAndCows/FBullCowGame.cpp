/*	FBullCowGame.cpp
*	created by Jack Draak
*	as tutored by Ben Tristem
*	Jan.2016 pre-release version 0.9.4b
*
*	This class handles the game mechanics of the Bull Cow Game
*	I/O functions are handled in the Main.cpp class
*
*	see FBullCowGame.h for further description
*/
#pragma once
#include "FBullCowGame.h"
#include <ctime>
#include <map>

// required for UnrealEngine-friendly syntax
#define TMap std::map
using int32 = int;

FBullCowGame::FBullCowGame()                    { FBullCowGame::Reset(); }
FString FBullCowGame::GetRankedIsogram() const  { return MyIsogram; }
int32 FBullCowGame::GetIsogramLength() const    { return (MyIsogram.length()); }
int32 FBullCowGame::GetScore() const            { return MyScore; }
int32 FBullCowGame::GetTurn() const             { return MyCurrentTurn; }
int32 FBullCowGame::GetLevel() const            { return MyLevel; }
bool FBullCowGame::IsPhaseWon() const           { return bGuessMatches; }
void FBullCowGame::IncrementTry()               { MyCurrentTurn++; return; }
void FBullCowGame::LevelUp()                    { MyLevel++; return; }
void FBullCowGame::ScoreUp(int32 Score)         { MyScore = MyScore + Score; }

// ensure the entered guess is alphabetic, correct # of letters & is an isogram
EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	if (Guess.length() != GetIsogramLength())   { return EGuessStatus::Length_Mismatch; }
	else if (!IsWordIsogram(Guess))             { return EGuessStatus::Not_Isogram; }
	else if (!IsWordAlpha(Guess))               { return EGuessStatus::Not_Alpha; }
	else                                        { return EGuessStatus::OK; }
}

// upon reciept of a valid* guess, increments turn and returns count
FBullCowCounts FBullCowGame::ProcessValidGuess(FString Guess)
{
	// instantiate a new container named BullCowCounts of type FBullCowCounts, which is recycled through each turn and round or phase(?):
	FBullCowCounts BullCowCounts;

	int32 GameWordLength = GetIsogramLength();
	for (int32 MyGameWordChar = 0; MyGameWordChar < GameWordLength; MyGameWordChar++)
	{
		for (int32 GuessChar = 0; GuessChar < GameWordLength; GuessChar++)
		{
			char GameWordChar = MyIsogram[MyGameWordChar];
			char GuessWordChar = tolower(Guess[GuessChar]); // NOTE this is the other call to use tolower() [see also IsWordIsogram()]
			if (GuessWordChar == GameWordChar)
			{
				if (MyGameWordChar == GuessChar) { BullCowCounts.Bulls++; }
				else { BullCowCounts.Cows++; }
			}
		}
	}
	if (BullCowCounts.Bulls == GameWordLength) 
	{
		// game [DIFFICULTY Tuning: Part A] because the quicker a player score goes up, the more quickly difficulty goes up
		FBullCowGame::ScoreUp((MyLevel +1) + (FBullCowGame::GetMaxTries() - MyCurrentTurn));

		// game [DIFFICULTY Tuning: Part B] can be acomplished here; the curve { 3, 9, 27, 71, 223, ... } stages leveling:
		if (MyLevel == 0 && MyScore > 3) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 1 && MyScore > 6) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 2 && MyScore > 12) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 3 && MyScore > 24) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 4 && MyScore > 48) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 5 && MyScore > 96) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 6 && MyScore > 192) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 7 && MyScore > 384) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 8 && MyScore > 768) { FBullCowGame::LevelUp(); }
		FBullCowGame::bGuessMatches = true;
	}
	return BullCowCounts;
}

// use a map to correlate Word.length and MaxTries [DIFFICULTY Tuning: Part C]
int32 FBullCowGame::GetMaxTries() const
{
	TMap<int32, int32>WordLengthToMaxTries 
	{
		{ 3, 6 },   { 11, 21 },
		{ 4, 7 },   { 12, 16 },
		{ 5, 9 },   { 13, 12 },
		{ 6, 12 },  { 14, 9 },
		{ 7, 16 },  { 15, 7 },
		{ 8, 21 },  { 16, 6 },
		{ 9, 27 },  { 17, 5 },
		{ 10, 34 }, { 18, 4 }
	};
	return WordLengthToMaxTries[MyIsogram.length()];
}

// Initialize a new game state (overloaded: if game InPlay set-up for a new turn)
void FBullCowGame::Reset()
{
	if (!bDoneOnce)
	{
		MyScore = 0;
		MyLevel = 0;
		bDoneOnce = true;
	}
	bGuessMatches = false;
	MyCurrentTurn = 1;

	// The following Do-While code validates dictionary entries before putting them in play...
	// it will silently select a new word if it first selects a non isogram word from the dictionary.
	// WARNING If the dictionary is absent or overly corrupted, this could result in an infinte loop
	// TODO apply this to a Dictionary of size S
		// select random member from pool
		// size-compare, is it level-appropriate
		// then vv check for isogram... serve the word when all tests pass(?)
	bool bSafeWord;
	do
	{
		FString MyGameWord = SelectIsogramForLevel();
		bSafeWord = IsWordIsogram(MyGameWord);
	} while (!bSafeWord);
	return;
}

// select an apropriate random *isogram from the map, based on the player level
FString FBullCowGame::SelectIsogramForLevel()
{
	constexpr int32 INDEX_DEPTH = 15; // TODO if the dictionary is expanded, UPDATE. Each row must grow at the same time to length INDEX_DEPTH

	FString Words_0[INDEX_DEPTH] = { "pat", "eat", "rat", "and", "mat", "cat", "ate", "sat", "fat", "bat", "our", "the", "new", "had", "day" };
	FString Words_1[INDEX_DEPTH] = { "sand", "pair", "raid", "care", "sock", "fair", "hair", "land", "walk", "talk", "same", "dart", "this", "from", "suit" };
	FString Words_2[INDEX_DEPTH] = { "toads", "brick", "stick", "roads", "stand", "trick", "thick", "loads", "talks", "locks", "thing", "miles", "lives", "facts", "cloth" };
	FString Words_3[INDEX_DEPTH] = { "jaunts", "abound", "tricks", "bricks", "crawls", "crowns", "around", "orgasm", "bounty", "gizmos", "travel", "wealth", "second", "curled", "loving" };
	FString Words_4[INDEX_DEPTH] = { "jukebox", "ziplock", "lockjaw", "quickly", "crazily", "jaybird", "jackpot", "quicken", "quicker", "imports", "clothes", "polearm", "jockeys", "subject", "cliquey" };
	FString Words_5[INDEX_DEPTH] = { "slumbering", "ziplocks", "hospital", "imported", "questing", "finagled", "question", "speaking", "spectrum", "imports", "clothes", "spectrum", "jockular", "gumption", "pronated" };
	FString Words_6[INDEX_DEPTH] = { "hospitable", "background", "campground", "greyhounds", "infamously", "sympathizer", "shockingly", "duplicates", "authorizes", "farsighted", "binoculars", "destroying", "subjectify", "introduces", "nightmares" };
	FString Words_7[INDEX_DEPTH] = { "workmanship", "palindromes", "speculation", "trampolines", "personality", "abolishment", "atmospheric", "playgrounds", "backgrounds", "countryside", "birthplaces", "precautions", "regulations", "subcategory", "documentary" };
	FString Words_8[INDEX_DEPTH] = { "thunderclaps", "misconjugated", "unproblematic", "unprofitable", "questionably", "packinghouse", "upholstering", "lexicography", "malnourished", "subordinately", "counterplays", "multipronged", "unforgivable", "subvocalized", "exhaustingly" };
	FString Words_9[INDEX_DEPTH] = { "subdermatoglyphic", "uncopyrightable", "ambidextrously", "hydromagnetics", "pseudomythical", "flamethrowing", "unsympathized", "unpredictably", "multibranched", "subformatively", "hydropneumatic", "consumptively", "computerizably", "unmaledictory", "ambidextrously" };

	// validate dictionary once --
	// bad words are sent to std::cout, 
	// so scroll up in the console to see if any were flagged before the first header was printed
	// Yes, it's bootleg, but so is running a dictionary with non-isogram members! :)
	//
	// REVISED: before a word is put in play it is validated as an isogram or replaced, but let's leave 
	// this here as an easy way to validate updated dictionaries.... Once you do remove it:
	// ^^ that would allow you to remove #include<stdio> from FBullCowGame.h and put it back in Main.cpp
	// (where it belongs!) 
	if (!DoneOnce) 
	{
		int32 Index = 1;
		do
		{
			if (!IsWordIsogram(Words_0[Index])) { std::cout << Words_0[Index] << std::endl; }
			if (!IsWordIsogram(Words_1[Index])) { std::cout << Words_1[Index] << std::endl; }
			if (!IsWordIsogram(Words_2[Index])) { std::cout << Words_2[Index] << std::endl; }
			if (!IsWordIsogram(Words_3[Index])) { std::cout << Words_3[Index] << std::endl; }
			if (!IsWordIsogram(Words_4[Index])) { std::cout << Words_4[Index] << std::endl; }
			if (!IsWordIsogram(Words_5[Index])) { std::cout << Words_5[Index] << std::endl; }
			if (!IsWordIsogram(Words_6[Index])) { std::cout << Words_6[Index] << std::endl; }
			if (!IsWordIsogram(Words_7[Index])) { std::cout << Words_7[Index] << std::endl; }
			if (!IsWordIsogram(Words_8[Index])) { std::cout << Words_8[Index] << std::endl; }
			if (!IsWordIsogram(Words_9[Index])) { std::cout << Words_9[Index] << std::endl; }
			Index++;
		} while (Index < 15);
		DoneOnce = true;
	}

	std::srand(time(NULL)); // REQUIRED this re-populates the SEED for the rand() number generator
	int32 Index = rand() % INDEX_DEPTH; 

	switch (MyLevel)
	{
	case 0:
		MyIsogram = Words_0[Index];	break;
	case 1:
		MyIsogram = Words_1[Index];	break;
	case 2:
		MyIsogram = Words_2[Index];	break;
	case 3:
		MyIsogram = Words_3[Index];	break;
	case 4:
		MyIsogram = Words_4[Index];	break;
	case 5:
		MyIsogram = Words_5[Index];	break;
	case 6:
		MyIsogram = Words_6[Index];	break;
	case 7:
		MyIsogram = Words_7[Index];	break;
	case 8:
		MyIsogram = Words_8[Index];	break;
	case 9:
		MyIsogram = Words_9[Index];	break;
	default:
		MyIsogram = Words_0[Index];	break;
	}
	return MyIsogram; // BREAKPOINT here to view secret game word
}

// True/False: Isogram
bool FBullCowGame::IsWordIsogram(FString Word) const
{
	if (Word.length() <= 1) { return true; }
	TMap<char, bool> LetterSeen;
	for (auto Letter : Word)
	{
		Letter = tolower(Letter); // one of two places tolower() is called, see also ProcessValidGuess()
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
