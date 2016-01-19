#include "FBullCowGame.h"
#include <map>
#define TMap std::map

using int32 = int;

FBullCowGame::FBullCowGame()					{ FBullCowGame::Reset(); }
FString FBullCowGame::GetGameWord() const		{ return MyGameWord; }
int32 FBullCowGame::GetMaxTries() const			{ return MyMaxTries; }
int32 FBullCowGame::GetCurrentTry() const		{ return MyCurrentTry; }
int32 FBullCowGame::GetGameWordLength() const	{ return (MyGameWord.length()); }
bool FBullCowGame::IsGameWon() const			{ return bMyWin; }
void FBullCowGame::IncrementTry()				{ MyCurrentTry++; return; }

bool FBullCowGame::IsWordIsogram(FString Word) const
{
	// treat 0 and 1 char words as isograms, because by strict definition, they are
	if (Word.length() <= 1) { return true; }

	// if Word is 2+ characters sort into a hash table, one character at a time (converting to lower case on the fly)
	TMap<char, bool> LetterSeen; // a hashmap of Word
	for (auto Letter : Word)
	{
		Letter = tolower(Letter); // convert to lowercase for simplicity
		// return false if a character is in the hash table more than once
		if (!LetterSeen[Letter]) 
		{
			LetterSeen[Letter] = true; // if first time seen, then set to true
		}
		else if (LetterSeen[Letter]) // duplicate letter detected, therefore exit 1
		{
			return false; 
		}
	}
	// if all characters pass through the above filter, congratulations, you have an isogram
	return true;
}

// if there are characters in the Word which are not alphabetic, return false, otherwise return true
bool FBullCowGame::IsWordAlpha(FString Word) const
{
	for (int32 WordChar = 0; WordChar < Word.length(); WordChar++)
	{
		// compare letters, exit 1 on detection of non-alpha entry
		if (!isalpha(Word[WordChar])) return false;
	}
	return true; // if Word is alphabetic
}

EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	// if guess is short|long return error
	int32 GameWL = GetGameWordLength();
	int32 GuessWL = Guess.length();
	if (GuessWL != GameWL)
	{
		return EGuessStatus::Length_Mismatch;
	}

	// if guess is not isogram return error
	else if (!IsWordIsogram(Guess))
	{
		return EGuessStatus::Not_Isogram;
	}

	// if Guess is not alphabetical, return error
	else if (!IsWordAlpha(Guess))
	{
		return EGuessStatus::Not_Alpha;
	}

	// else return EGuessStatus::OK;
	else
	{
		return EGuessStatus::OK;
	}
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
			// if match then...
			char GameWordChar = MyGameWord[MyGameWordChar];
			char GuessWordChar = tolower(Guess[GuessChar]);
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

void FBullCowGame::Reset()
{
	bMyWin = false;
	MyCurrentTry = 1;
	
	constexpr int32 MAX_TRIES = 3;
	MyMaxTries = MAX_TRIES;
	
	const FString GAME_WORD = "abound";
	MyGameWord = GAME_WORD;

	return;
}
