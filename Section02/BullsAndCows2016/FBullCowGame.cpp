#include "FBullCowGame.h"

using int32 = int;

FBullCowGame::FBullCowGame()							{ FBullCowGame::Reset(); }
void FBullCowGame::IncrementTry()						{ MyCurrentTry++; return; }
int32 FBullCowGame::GetMaxTries() const					{ return MyMaxTries; }
int32 FBullCowGame::GetCurrentTry() const				{ return MyCurrentTry; }
int32 FBullCowGame::GetGameWordLength() const			{ return (MyGameWord.length()); }
bool FBullCowGame::IsGameWon() const					{ return false; }
bool FBullCowGame::CheckGuessValidity(FString) const	{ return false; }

// Upon reciept of a valid* guess, increments turn and returns count
FBullCowCount FBullCowGame::SubmitGuess(FString Guess)
{
	//IncrementTry();
	MyCurrentTry++;
	FBullCowCount BullCowCount;

	// loop through all letters in the guess
	int32 GameWordLength = GetGameWordLength();
	for (int32 MyGameWordChar = 0; MyGameWordChar < GameWordLength; MyGameWordChar++) 
	{	
		// compare letters against gameword
		for (int32 GuessChar = 0; GuessChar < GameWordLength; GuessChar++)
		{	
			// if match then
			char GameWordChar = MyGameWord[MyGameWordChar];
			char GuessWordChar = Guess[GuessChar];
			if (GuessWordChar == GameWordChar)
			{
				// incrememnt Bulls if they're in the corresponding position
				if (MyGameWordChar == GuessChar) { BullCowCount.Bulls++; }

				// increment Cows if they're not in the corresponding position
				else { BullCowCount.Cows++; }
			}
		}
	}
	return BullCowCount;
}
void FBullCowGame::Reset()
{
	const FString GAME_WORD = "abound";
	MyGameWord = GAME_WORD;
	
	constexpr int32 MAX_TRIES = 5;
	MyMaxTries = MAX_TRIES;
	
	MyCurrentTry = 1;
	return;
}