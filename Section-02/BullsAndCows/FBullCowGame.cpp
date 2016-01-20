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
		FBullCowGame::ScoreUp(FBullCowGame::GetLevel() + (MyMaxTries - MyCurrentTry) +1);

		// game difficulty tuning can be acomplished here; the curve { 3, 9, 27, 71, 223, ... } stages leveling:
		// this could certainly be done with more eloquence., I think...
		if (MyLevel == 0 && MyScore > 2) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 1 && MyScore > 4) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 2 && MyScore > 8) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 3 && MyScore > 16) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 4 && MyScore > 32) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 5 && MyScore > 64) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 6 && MyScore > 128) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 7 && MyScore > 256) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 8 && MyScore > 512) { FBullCowGame::LevelUp(); }
		FBullCowGame::bMyWin = true;
	}
	return BullCowCount;
}

// Initialize a new game state (overloading: if game InPlay set-up for a new turn)
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
	int32 MyGameWordLength = MyGameWord.length();

	constexpr int32 DEFAULT_MAX_TRIES = 4;
	MyMaxTries = DEFAULT_MAX_TRIES;

	if (MyGameWordLength >= MyMaxTries)
	{
		if (MyGameWordLength > 10) { MyMaxTries = 10; } // TODO no word of any size is allowed more than 10 guesses; is this too dissicult? (Lecture 46)
		else { MyMaxTries = MyGameWordLength; }
	}
	return;
}

// based on the player level, select an apropriate random word from the map
FString FBullCowGame::SelectGameWordForLevel()
{
	FString Words_0[15] = { "pat", "eat", "rat", "and", "mat", "cat", "ate", "sat", "fat", "bat", "our", "the", "new", "had", "day" };
	FString Words_1[15] = { "sand", "pair", "raid", "care", "sock", "fair", "hair", "land", "walk", "talk", "same", "dart", "this", "from", "suit" };
	FString Words_2[15] = { "toads", "brick", "stick", "roads", "stand", "trick", "thick", "loads", "talks", "locks", "thing", "miles", "lives", "facts", "cloth" };
	FString Words_3[15] = { "jaunts", "abound", "tricks", "bricks", "crawls", "crowns", "around", "orgasm", "bounty", "gizmos", "travel", "wealth", "second", "curled", "loving" };
	FString Words_4[15] = { "jukebox", "ziplock", "lockjaw", "quickly", "question", "jaybird", "jackpot", "quicken", "quicker", "student", "clothes", "spectrum", "jockeys", "subject", "cliquey" };
	FString Words_5[15] = { "hospitable", "background", "campground", "greyhounds", "infamously", "slumbering", "shockingly", "duplicates", "authorizes", "farsighted", "binoculars", "destroying", "chlothespin", "introduces", "nightmares" };
	FString Words_6[15] = { "workmanship", "palindromes", "speculation", "trampolines", "personality", "abolishment", "atmospheric", "playgrounds", "backgrounds", "countryside", "birthplaces", "precautions", "regulations", "subcategory", "documentary" };
	FString Words_7[15] = { "thunderclaps", "misconjugated", "unproblematic", "unprofitable", "questionably", "packinghouse", "upholstering", "lexicography", "malnourished", "subordinately", "counterplays", "multipronged", "unforgivable", "subvocalized", "exhaustingly" };
	FString Words_8[15] = { "subdermatoglyphic", "uncopyrightable", "ambidextrously", "hydromagnetics", "pseudomythical", "flamethrowing", "unsympathized", "unpredictably", "multibranched", "subformatively", "hydropneumatic", "consumptively", "computerizably", "unmaledictory", "ambidextrously" };

	std::srand(time(NULL));
	int32 frame = rand() % 15;
	if (frame < 0) { frame = 0; } // TODO overkill much?
	if (frame > 14) { frame = 14; } // do you even code, bro?
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
	return MyGameWord;
}