/*	Main.cpp
*	created by Jack Draak
*	as tutored by Ben Tristem
*	Jan.2016 pre-release 0.9.1b
*
*	I/O functions are handled here in the Main.cpp class, while the
*	game mechanics are found in the FBullCowGame.cpp class.
*
*	This is the console executable that makes use of the FBullCowGame class.
*	This acts as the view in a MVC pattern, and is responsible for all
*	user input & output. For game logic/engine, see the FBullCowGame class.
*/
#pragma once
#include <iostream>
#include "FBullCowGame.h"

using FText = std::string;
using int32 = int;

void ManageGame();
void PrintTurnSummary(FText &Guess, FBullCowCount &BullCowCount);
void PrintIntroTail();
void PrintIntroHead();
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
		PrintIntroHead();
		ManageGame();
	} while (bAskToPlayAgain());

	// "end of line." program execution complete
	std::cout << std::endl << "E n d . o f . l i n e . ." << std::endl;
	return 0;
}

// core game method
void ManageGame()
{
	BCGame.Reset();
	PrintIntroTail();

	while (!BCGame.IsGameWon() && BCGame.GetCurrentTry() <= BCGame.GetMaxTries())
	{
		FText Guess = GetValidGuess();
		FBullCowCount BullCowCount = BCGame.SubmitValidGuess(Guess);
		PrintTurnSummary(Guess, BullCowCount);
	}
	PrintPhaseSummary();
	return;
}

// output: after a guess is validated, print the results for the turn
void PrintTurnSummary(FText &Guess, FBullCowCount &BullCowCount)
{
	std::cout << "Guess #" << BCGame.GetCurrentTry() << ": " << Guess << ": ";
	std::cout << "Bulls = " << BullCowCount.Bulls << " & ";
	std::cout << "Cows = " << BullCowCount.Cows << "\n";
	BCGame.IncrementTry();
}

// print game introduction text HEAD
void PrintIntroHead()
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
	return;
}

// print game introduction text TAIL
void PrintIntroTail()
{
	std::cout << BCGame.GetGameWordLength() << " Bulls\n";
	std::cout << "with one of your guesses to win. Good luck!\n";
	std::cout << "\n     | Player Level: " << BCGame.GetLevel() + 1 << "\n";
	std::cout << "     | Player Score: " << BCGame.GetScore() << "\n";
	std::cout << "     | Maximum guesses this round: " << BCGame.GetMaxTries() << "\n";
}

// determine if the player wants to continue playing, explicit Y/y required, or exit 1
bool bAskToPlayAgain()
{
	FText Responce = "";
	std::cout << std::endl << "[`Ctrl-C` to exit anytime, explicit N expected] Continue playing? Y/n ";
	std::getline(std::cin, Responce);
	if ((Responce[0] == 'n') || (Responce[0] == 'N'))
	{
		return false;
	}
	return true;
}

// Game-Phase Summary generated here: if won then Form-A, else if out of turns then Form-B
void PrintPhaseSummary()
{
	if (BCGame.IsGameWon())
	{
		std::cout << std::endl << "      !~!~!~!~!~!~!~!       Congratulations on winning this round!";
		std::cout << std::endl << "      !~!~WINNER!~!~!       Guesses: " << BCGame.GetCurrentTry() - 1 << " of " << BCGame.GetMaxTries() << " used";
		std::cout << std::endl << "      !~!~!~!~!~!~!~!       Game Word: ";
		std::cout << BCGame.GetGameWord();
		std::cout << std::endl;
	}
	else if (BCGame.GetCurrentTry() >= BCGame.GetMaxTries())
	{
		std::cout << std::endl << "      !~!~!~!~!~!~!~!       It's challenging, isn't it! Don't give up yet!";
		std::cout << std::endl << "      !~!~!LOSER!~!~!       Guesses: " << BCGame.GetCurrentTry() - 1 << " of " << BCGame.GetMaxTries() << " used";
		std::cout << std::endl << "      !~!~!~!~!~!~!~!       Game Word : ";
		// TODO create a method that instead of #####.length returns with Bulls (i.e. am###s) for optional hints
		for (auto Letter : BCGame.GetGameWord()) { std::cout << "#"; }
		std::cout << std::endl;
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
		// acquire input:
		std::cout << std::endl << "Please enter guess #" << BCGame.GetCurrentTry() << " of " << BCGame.GetMaxTries() << ": ";
		std::getline(std::cin, Guess);

		// provide helpful feedback if the guess is unexpected:
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
	} while (Status != EGuessStatus::OK);
	return Guess;
}

// spam `X` newlines at the console
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