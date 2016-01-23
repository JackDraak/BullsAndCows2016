/*	Main.cpp
*	created by Jack Draak
*	as tutored by Ben Tristem
*	Jan.2016 pre-release version 0.9.43
*
*	This is the console executable that makes use of the FBullCowGame class.
*	This acts as the view in a MVC pattern, and is responsible for all I/O functions.
*	The game mechanics operate in the FBullCowGame.cpp class.
*/
#pragma once
#include "FBullCowGame.h"
#include <string>

// required for UnrealEngine-friendly syntax:
using FText = std::string;
using int32 = int;

// function prototypes, as outside class:
void ManageGame();
void PrintIntro();
FText GetValidGuess();
void PrintTurnSummary(FText &Guess, FBullCowCounts &BullCowCount);
void PrintPhaseSummary();
bool bAskToPlayAgain();
void SpamNewline(int32 Repeats);
void PrintHint();
bool bHints = false;

// instantiate a new game named BCGame, which is recycled through each turn and round (or phase):
FBullCowGame BCGame;

// the entry-point for the applciation:
int main()
{
	do { ManageGame(); } while (bAskToPlayAgain());

	// "end of line." program execution complete:
	std::cout << std::endl << "E n d . o f . l i n e . ." << std::endl;
	return 0;
}

// core game I/O handler method:
void ManageGame()
{
	BCGame.Reset();
	PrintIntro();
	while (!BCGame.IsPhaseWon() && BCGame.GetTurn() <= BCGame.GetMaxTries())
	{
		FText Guess = GetValidGuess();
		FBullCowCounts BullCowCounts = BCGame.ProcessValidGuess(Guess);
		PrintTurnSummary(Guess, BullCowCounts);
		BCGame.IncrementTry();
	}
	PrintPhaseSummary();
	return;
}

// output - print game introduction, instruction and status text
void PrintIntro()
{
	std::cout << "Version 0.9.43";
	SpamNewline(72);	
	std::cout << "                      -+-=-+-=-+-=-+-=-+-=-+-=-+-=-+-\n";
	std::cout << "                       Welcome  to  Bulls  and  Cows\n";
	std::cout << "                      -+-=-+-=-+-=-+-=-+-=-+-=-+-=-+-\n\n";
	std::cout << "Your mission, should you choose to accept it, is to guess an isogram*.\n\n";
	std::cout << " * An isogram is a word comprised of unique letters, for example:\n";
	std::cout << "     - step: is an isogram, each letter is unique in the word\n";
	std::cout << "     - book: is NOT an isogram; it contains two 'o's\n\n";
	std::cout << "How to win: After you enter a guess, you will be awarded Bulls and Cows.\n";
	std::cout << "Guessing a correct letter in the correct position is worth one Bull, while a\n";
	std::cout << "correct letter in the wrong position adds one Cow. Use these clues to help\n";
	std::cout << "determine your next guess. In other words, this round you need to earn ";
	std::cout << BCGame.GetIsogramLength() << " Bulls\n" << "with one of your guesses to win. Good luck!\n\n";
	std::cout << "     |  Words Lassoed : Words Butchered : " << BCGame.GetWins() << " : " << BCGame.GetDefeats() << "\n";
	std::cout << "     |      Total Awards -- Bulls, Cows : " << BCGame.GetBulls() << ", " << BCGame.GetCows() << "\n";
	std::cout << "     |                      Near Misses : " << BCGame.GetMisses() << "\n";
	std::cout << "     | (Current Diffuculty Level) Score : (" << BCGame.GetLevel() +1 << ") " << BCGame.GetScore() << "\n";
	std::cout << "     |       Maximum guesses this round : " << BCGame.GetMaxTries() << "\n";
}

// I/O - get a valid guess from the player, loop until satisfied
FText GetValidGuess()
{
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	FText Guess = "";
	do
	{
		// acquire input:
		std::cout << std::endl << "Please enter guess #" << BCGame.GetTurn();
		std::cout << " of " << BCGame.GetMaxTries() << ": ";
		std::getline(std::cin, Guess);

		// provide helpful feedback if the guess is unexpected:
		Status = BCGame.CheckGuessValidity(Guess);
		switch (Status)
		{
		case EGuessStatus::Length_Mismatch:
			std::cout << "\nPlease enter a " << BCGame.GetIsogramLength() << " letter isogram.\n";
			break;
		case EGuessStatus::Not_Isogram:
			std::cout << "\nAn isogram doesn't use any single letter more than once, rather than using\n";
			std::cout << Guess << ", please try guessing again.\n";
			break;
		case EGuessStatus::Not_Alpha:
			std::cout << "\nYou've entered one or more non-alphabetic characters. Instead of using\n";
			std::cout << Guess << ", please try an English isogram word.\n";
			break;
		default: // i.e. case OK
			break;
		}
	} while (Status != EGuessStatus::OK);
	return Guess;
}

// TODO create a method that instead of #####.length returns with Bulls (i.e. ab###f) for optional hints, i.e. run-in hint-mode
void PrintHint()
{
	std::cout << "#@#@#@#@#@#\n"; // TODO complete form here
	return;
}

// print-back user guess, i.e.  ab#### cd


// output - after a guess is validated, print the results: Guess# of #, Bull# Cow#
void PrintTurnSummary(FText &Guess, FBullCowCounts &BullCowCount) 
{
	std::cout << "Guess #" << BCGame.GetTurn() << ": " << Guess << ": ";
	std::cout << "Bulls = " << BullCowCount.Bulls << " & ";
	std::cout << "Cows = " << BullCowCount.Cows << "\n";
	if (bHints) { PrintHint(); }
}

// output - Game-Phase (Round) Summary generated here: if phase is won then use Form-A, else if out of turns then use Form-B
void PrintPhaseSummary()
{
	if (BCGame.IsPhaseWon())
	{
		std::cout << std::endl << "      !~!~!~!~!~!~!~!       Congratulations on winning this round!";
		std::cout << std::endl << "      !~!~WINNER!~!~!       Guesses: " << BCGame.GetTurn() - 1 << " of " << BCGame.GetMaxTries() << " used";
		std::cout << std::endl << "      !~!~!~!~!~!~!~!       Game Word: ";
		std::cout << BCGame.GetRankedIsogram();
		std::cout << std::endl;
		BCGame.IncrementWins();
	}
	else if (BCGame.GetTurn() >= BCGame.GetMaxTries())
	{
		std::cout << std::endl << "      !~!~!~!~!~!~!~!       It's challenging, isn't it! Don't give up yet!";
		std::cout << std::endl << "      !~!~!LOSER!~!~!       Guesses: " << BCGame.GetTurn() - 1 << " of " << BCGame.GetMaxTries() << " used";
		std::cout << std::endl << "      !~!~!~!~!~!~!~!       Game Word : ";
		for (auto Letter : BCGame.GetRankedIsogram()) { std::cout << "#"; }
		std::cout << std::endl;
		BCGame.IncrementDefeats();
	}
	return;
}

// I/O - determine if the player wants to continue playing, or toggle hints
bool bAskToPlayAgain()
{
	FText Responce = "";
	std::cout << std::endl << "[h to toggle hints] Continue playing? Y/n ";
	std::getline(std::cin, Responce);
	if ((Responce[0] == 'n') || (Responce[0] == 'N')) { return false; }
	else if ((Responce[0] == 'h') || (Responce[0] == 'H')) { bHints = !bHints; }
	return true;
}

// output - spam `X` newlines at the console
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
