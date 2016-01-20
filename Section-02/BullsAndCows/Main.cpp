/*
This is the console executable that makes use of the FBullCowGame class.
This acts as the view in a MVC pattern, and is responsible for all
user input & output. For game logic/engine, see the FBullCowGame class.
*/
#pragma once
#include <iostream>
#include "FBullCowGame.h"

using FText = std::string;
using int32 = int;

void PlayGame();
void PrintIntro();
void PrintPhaseSummary();
void SpamNewline(int32 Repeats);
FText GetValidGuess();
bool bAskToPlayAgain();

// instantiate a new game named BCGame
FBullCowGame BCGame;

// the entry-point for the applciation
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

// core game method
void PlayGame()
{
	BCGame.Reset();
	// effectively, the tail of the PrintIntro() header:
	std::cout << BCGame.GetGameWordLength() << " Bulls\n";
	std::cout << "with one of your guesses to win. Good luck!\n";
	std::cout << "\n     | Player Level: " << BCGame.GetLevel() +1 << "\n";
	std::cout << "     | Player Score: " << BCGame.GetScore() << "\n";
	std::cout << "     | Maximum guesses this round: " << BCGame.GetMaxTries() << "\n";

	// loop: if the game is not won and turns remain
	while (!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= BCGame.GetMaxTries())
	{
		FText Guess = GetValidGuess();

		// submit valid guess to the game engine which will update Bull/Cow counts
		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);

		// display the results back to the player each turn
		std::cout << "Guess #" << BCGame.GetCurrentTry() << ": " << Guess << ": ";
		std::cout << "Bulls = " << BullCowCount.Bulls << " & ";
		std::cout << "Cows = " << BullCowCount.Cows << "\n";
		BCGame.IncrementTry();
	}
	// Win or lose, what's the result?
	PrintPhaseSummary();
	return;
}

// print game introduction text
void PrintIntro()
{
	SpamNewline(72);
	std::cout << "                      -+-=-+-=-+-=-+-=-+-=-+-=-+-=-+-\n";
	std::cout << "                       Welcome  to  Bulls  and  Cows\n";
	std::cout << "                      -+-=-+-=-+-=-+-=-+-=-+-=-+-=-+-\n\n";

	std::cout << "Your mission, should you choose to accept it, is to guess an isogram*.\n";

	std::cout << "\n * An isogram is a word comprised of unique letters, for example:\n";
	std::cout << "     - step: is an isogram, each letter is unique in the word\n";
	std::cout << "     - book: is NOT an isogram; it contains two 'o's\n";

	std::cout << "\nHow to win: After you enter a guess, you will be awarded Bulls and Cows.\n";
	std::cout << "Guessing a correct letter in the correct position is worth one Bull, while a\n";
	std::cout << "correct letter in the wrong position adds one Cow. Use these clues to help\n";
	std::cout << "determine your next guess. In other words, this round you need to earn ";
	// proceed to header-tail because we need a number, but need to call Reset() first [see PlayGame() above]
	// TODO right around here is where we're getting random errors
	return;
}

// determine if the player wants to continue playing, explicit Y/y required, or exit
bool bAskToPlayAgain()
{
	FText Responce = "";
	std::cout << std::endl << "Would you like to play again? y/N ";
	std::getline(std::cin, Responce);
	if ((Responce[0] == 'y') || (Responce[0] == 'Y'))
	{
		return true;
	}
	return false;
}

// Game Summary generated here: if won then Form-A, else if out of turns then Form-B
void PrintPhaseSummary()
{
	if (BCGame.IsGameWon())
	{
		std::cout << std::endl;
		std::cout << std::endl << "!~!~!~!~!~!~!~!";
		std::cout << std::endl << "!~!~WINNER!~!~!              Guesses: " << BCGame.GetCurrentTry() - 1 << " of " << BCGame.GetMaxTries();
		std::cout << std::endl << "!~!~!~!~!~!~!~!              Game Word: " << BCGame.GetGameWord();
		std::cout << std::endl;
	}
	else if (BCGame.GetCurrentTry() >= BCGame.GetMaxTries())
	{
		std::cout << std::endl;
		std::cout << std::endl << "!~!~!~!~!~!~!~!";
		std::cout << std::endl << "!~!~!LOSER!~!~!              Guesses: " << BCGame.GetCurrentTry() - 1 << " of " << BCGame.GetMaxTries();
		std::cout << std::endl << "!~!~!~!~!~!~!~!              Game Word : ";
		for (auto Letter : BCGame.GetGameWord())
		{
			std::cout << "#"; // TODO create a method that instead of #####.length returns with Bulls (i.e. am###s) for optional hints
		}
		std::cout << std::endl;
	}
	return;
}

// spam X newlines at the console (primarily for a 'clean  and consistent' presentation of the game interface)
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

// get a valid guess from the player, loop as needed
FText GetValidGuess()
{
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	FText Guess = "";

	do
	{
		// acquire input
		std::cout << std::endl << "Please enter guess #" << BCGame.GetCurrentTry() << " of " << BCGame.GetMaxTries() << ": ";
		std::getline(std::cin, Guess);

		// validate Guess: length, isogram, alpha...
		Status = BCGame.CheckGuessValidity(Guess);
		switch (Status)
		{
		case EGuessStatus::Length_Mismatch:
			std::cout << "\nPlease enter a " << BCGame.GetGameWordLength() << " letter isogram.\n";
			break;
		case EGuessStatus::Not_Isogram:
			std::cout << "\nAn isogram doesn't use any single letter more than once, unlike " << Guess << ", please guess again.\n";
			break;
		case EGuessStatus::Not_Alpha:
			std::cout << "\nYou've entered one or more non-alphabetic characters. Instead of " << Guess << ", please try an isogram word.\n";
			break;
		default: // i.e. case OK
			break;
		}
	} while (Status != EGuessStatus::OK); // lopp until input is validated
	return Guess;
}