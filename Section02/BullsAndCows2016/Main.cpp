/*
This is the console executable that makes use of the BullCow class.
This acts as the view in a MVC pattern, and is responsible for all
user interaction. For game logic, see the FBullCowGame class.
*/
#include <iostream>
#include "FBullCowGame.h"

using FText = std::string;
using int32 = int;

void PrintIntro();
void PlayGame();
bool bAskToPlayAgain();
void SpamNewline(int32 Repeats);
FText GetGuess();

// instantiate a new game named BCGame
FBullCowGame BCGame;

// the entry-point for our applciation
int main()
{
	do
	{
		PrintIntro();
		PlayGame();
	} while (bAskToPlayAgain());

	// "end of line." program execution complete
	std::cout << std::endl << "E n d . o f . l i n e . ." << std::endl;
	return 0;
}

// core game
void PlayGame()
{
	int32 MaxTries = BCGame.GetMaxTries();
	std::cout << "\n[MaxTries = " << MaxTries << "]\n";

	// loop through turns //TODO turn into WHILE loop once validating guesses
	for (int count = 1; count <= MaxTries; count++)
	{
		FText Guess = GetGuess();
		//check guess length is correct
		//if (Guess.length == FBullCowGame::)

		// submit valid guess to the game engine and recieve counts
		FBullCowCount BullCowCount = BCGame.SubmitGuess(Guess);

		// display results, i.e. numbers of Bulls and Cows

		int32 CurrentTry = (BCGame.GetCurrentTry()-1);
		std::cout << "Guess #" << CurrentTry << ": " << Guess << ": ";
		std::cout << "Bulls = " << BullCowCount.Bulls << " & ";
		std::cout << "Cows = " << BullCowCount.Cows << "\n";
	}
	// TODO add game summary
}

// introduce the game
void PrintIntro()
{
	constexpr int32 WORD_LENGTH = 5;

	SpamNewline(72);
	std::cout << "-+-=-+-=-+-=-+-=-+-=-+-=-+-=-+-\n";
	std::cout << " Welcome  to  Bulls  and  Cows\n";
	std::cout << "-+-=-+-=-+-=-+-=-+-=-+-=-+-=-+-\n";
	std::cout << "Your mission, should you choose to accept it,\n";
	std::cout << "is to guess an isogram* that is " << WORD_LENGTH;
	std::cout << " characters long.\n";
	std::cout << "\n * An isogram is a word comprised of unique letters, example:\n";
	std::cout << "take: is an isogram, each letter is unique in the word\n";
	std::cout << "book: is NOT an isogram; it contains two 'o's\n";
	return;
}

// get a guess from the player
FText GetGuess()
{
	FText Guess = "";
	int32 CurrentTry = BCGame.GetCurrentTry();
	int32 MaxTries = BCGame.GetMaxTries();
	std::cout << std::endl << "Please enter guess #" << CurrentTry << " of " << MaxTries << ": ";
	std::getline(std::cin, Guess);
	// BCGame.IncrementTry(); now handling this in FBullCowGame
	return Guess;
}

// determine if the player wants to continue playing
bool bAskToPlayAgain()
{
	FText Responce = "";
	std::cout << std::endl << "Would you like to play again? (Yes/No) ";
	std::getline(std::cin, Responce);
	if ((Responce[0] == 'y') || (Responce[0] == 'Y'))
	{
		BCGame.Reset();
		return true;
	}
	return false;
}

//  spam X newlines at the console (primarily for a 'clean  and consistent' presentation of the game interface)
void SpamNewline(int32 Repeats)
{
	int32 LoopNumber = 0;
	if (Repeats >= 1)
	{
		do
		{
			std::cout << std::endl;
			LoopNumber++;
		} while (LoopNumber < Repeats);
	}
	return;
}