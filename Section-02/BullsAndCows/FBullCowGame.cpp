#include "FBullCowGame.h"
#include <map>
#define TMap std::map

using int32 = int;

FBullCowGame::FBullCowGame()                    { FBullCowGame::Reset(); }
FString FBullCowGame::GetGameWord() const       { return MyGameWord; }
int32 FBullCowGame::GetMaxTries() const         { return MyMaxTries; }
int32 FBullCowGame::GetCurrentTry() const       { return MyCurrentTry; }
int32 FBullCowGame::GetGameWordLength() const   { return (MyGameWord.length()); }
bool FBullCowGame::IsGameWon() const            { return bMyWin; }
void FBullCowGame::IncrementTry()               { MyCurrentTry++; return; }

bool FBullCowGame::IsWordIsogram(FString Word) const
{
	// treat 0 and 1 char words as isograms, because clearly they don't need to be checked
	if (Word.length() <= 1) { return true; }

	// if Word is 2+ characters sort into a hash table, one character at a time (convert to lowercase on the fly)
	TMap<char, bool> LetterSeen; // a hashmap framework for Word
	for (auto Letter : Word)
	{
		Letter = tolower(Letter); // one of two places I use tolower(), see also SubmitValidGuess()

		// return false if a character is in the hash table more than once
		if (!LetterSeen[Letter])
		{
			LetterSeen[Letter] = true;	// if first time seen, then set to true
		}
		else if (LetterSeen[Letter])
		{
			return false; // duplicate letter detected, therefore exit 1
		}
	}
	return true; // if complement of characters pass through the above filter, Word is an isogram, exit 0
}

// if there are characters in the Word which are not alphabetic, return false, otherwise return true
bool FBullCowGame::IsWordAlpha(FString Word) const
{
	for (int32 WordChar = 0; WordChar < int32(Word.length()); WordChar++)
	{
//		auto testee = Word[WordChar];                   // set break-point and uncomment to watch this term
//		bool tester = (!isalpha(Word[WordChar]));       // set break-point and uncomment to watch this term

		// compare letters, exit 1 on detection of non-alpha entry
		if (!isalpha(Word[WordChar])) return false; // exit 1, non-alpha detection
	}
	return true; // if Word is alphabetic then exit 0
}

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

	// loop through each letter in the game word
	int32 GameWordLength = GetGameWordLength();
	for (int32 MyGameWordChar = 0; MyGameWordChar < GameWordLength; MyGameWordChar++)
	{
		// compare letters against guess word
		for (int32 GuessChar = 0; GuessChar < GameWordLength; GuessChar++)
		{
			char GameWordChar = MyGameWord[MyGameWordChar];
			// NOTE this is the other place I use tolower() [see also IsWordIsogram()]
			char GuessWordChar = tolower(Guess[GuessChar]);

			// if letters match:
			if (GuessWordChar == GameWordChar)
			{
				// incrememnt Bulls if they're in the corresponding position
				if (MyGameWordChar == GuessChar) { BullCowCount.Bulls++; }

				// increment Cows if they're not in the corresponding position
				else { BullCowCount.Cows++; }
			}
		}
	}

	if (BullCowCount.Bulls == GameWordLength)
	{
		FBullCowGame::bMyWin = true;
	}
	return BullCowCount;
}

// Initialize a new game state
void FBullCowGame::Reset()
{
	bMyWin = false;
	MyCurrentTry = 1;

	const FString GAME_WORD = "abound";
	MyGameWord = GAME_WORD; // TODO select randomly from an array of words

	constexpr int32 MAX_TRIES = 4;
	MyMaxTries = MAX_TRIES;

	if (int32(MyGameWord.length()) >= MAX_TRIES) { MyMaxTries = int32(MyGameWord.length()); }

	return;
}