﻿/*	Main.cpp
	created by Jack Draak
	as tutored by Ben Tristem
	Jan.2016 pre-release version 0.9.552a

	This is the console executable that makes use of the FBullCowGame class.
	This acts as the view in a MVC pattern, and is responsible for all I/O functions.
	The game mechanics operate in the FBullCowGame.cpp class.

	GAME-LOGIC:
	This is a `Mastermind`-style word guessing game using a small
	dictionary of isograms, tied to a scoring and leveling system
	designed to provide increasingly difficult challenges to one
	or more players working co-oepratively.

	Special Features Include:
		300 word isogram dictionary 
			- Split into 10 levels of difficulty
			- New "Word-Windowing" technology to keep the player on their toes
			  (challenge-word level = player level, window: +/- 1)
		Customizable help levels
			- toggle specific or general Bulltips
			- toggle specific or general Cowtips
			- Work in Progress: Hashtips
		Stats Galore
			- #'s of Cow and Bull awards
			- Hits, Misses & Near Misses
			- Player rank, from level 1-10
			- Get awarded arbitrary "Points" for each win!
			- 515 Functions behind the scenes!

		Feedback Requests:
			- Game tuning
				- word variety (good? bad?)
				- # of guesses (Too few? too many?)
				- rate of advancement (too slow? too fast?)
				- are 4-5 character words easier or harder than 6-8'ish character words?
			- Misc. feedback

		Known Bugs:
			THANKS SuperWig for helping with the random issue and the Hashtips!
*/
#pragma once
#include <algorithm>
#include <string>
#include "FBullCowGame.h"

// Required for UnrealEngine-friendly syntax:
using FText = std::string;
using int32 = int;

// Function prototypes, as outside class:
void PrintRoundIntro();
FText GetValidGuess();
bool bCowHints = true;
bool bBullHints = true;
bool bHashHints = true; // TODO set this to false for release
bool bAskToPlayAgain();
void PrintPhaseSummary();
void MasterControlProgram();
void SpamNewline(int32 Repeats);
void PrintTurnSummary(FBullCowCounts, FString Guess);

// Instantiate a new game named BCGame, which is recycled through each turn and round (or phase):
FBullCowGame BCGame;

// The entry-point for the applciation:
int main()
{
	constexpr int32 SPAM_SPAN = 72;
	std::cout << "Version 0.9.552";
	SpamNewline(SPAM_SPAN);
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
	std::cout << "determine your next guess.\n";

	do { MasterControlProgram(); } while (bAskToPlayAgain());

	// "End of line." -- program execution complete --
	std::cout << std::endl << "E n d . o f . l i n e . ." << std::endl;
	return 0;
}

// Core game I/O handler method:
void MasterControlProgram()
{
	BCGame.Reset();
	PrintRoundIntro();
	while (!BCGame.IsPhaseWon() && BCGame.GetTurn() <= BCGame.GetMaxTries())
	{
		FText Guess = GetValidGuess();
		FBullCowCounts BullCowCounts = BCGame.ProcessValidGuess(Guess);
		PrintTurnSummary(BullCowCounts, Guess);
		BCGame.IncrementTry();
	}
	PrintPhaseSummary();
	return;
}

// Output - Print game introduction, instruction and status text:
void PrintRoundIntro()
{
	std::cout << "\nA secret " << BCGame.GetIsogram().length() << "-letter isogram has been selected just for you... Therefore,";
	std::cout << "\nThis round you'll need to earn " << BCGame.GetIsogram().length() << " Bulls in one guess to win. Good luck!\n\n";
	std::cout << " |  /\\   /\\  |  Words Lassoed : Words Butchered : " << BCGame.GetWins() << " : " << BCGame.GetDefeats() << "\n";
	std::cout << " |  \\ \\_/ /  |      Total Awards -- Bulls, Cows : " << BCGame.GetBulls() << ", " << BCGame.GetCows() << "\n";
	std::cout << " |  ( .^. )  |                      Near Misses : " << BCGame.GetMisses() << "\n";
	std::cout << " |   \\ _ /   | (Current Diffuculty Level) Score : (" << BCGame.GetLevel() +1 << ") " << BCGame.GetScore() << "\n";
	std::cout << " |    (_)    |       Maximum guesses this round : " << BCGame.GetMaxTries() << "\n";
}

// I/O - Get a valid guess from the player, loop until satisfied:
FText GetValidGuess()
{
	EGuessStatus Status = EGuessStatus::Invalid_Status;
	FText Guess = "";
	do
	{
		// Acquire input:
		std::cout << std::endl << "Please enter a " << BCGame.GetIsogram().length() << " letter guess, #" << BCGame.GetTurn();
		std::cout << " of " << BCGame.GetMaxTries() << ": ";
		std::getline(std::cin, Guess);

		// Provide helpful feedback if the guess is unexpected:
		Status = BCGame.CheckGuessValidity(Guess);
		switch (Status)
		{
		case EGuessStatus::Length_Mismatch:
			std::cout << "\nOops, that won't work! `" << Guess << "` has " << Guess.length() << " letters.\n";
			break;
		case EGuessStatus::Not_Isogram:
			std::cout << "\nAn isogram doesn't use any single letter more than once, rather than using\n";
			std::cout << "`" << Guess << "`, please try guessing again.\n";
			break;
		case EGuessStatus::Not_Alpha:
			std::cout << "\nYou've entered one or more non-alphabetic characters. Instead of using\n";
			std::cout << "`" << Guess << "`, please try an English isogram word.\n";
			break;
		default: // i.o.w.: case OK
			break;
		}
	} while (Status != EGuessStatus::OK);
	return Guess;
}

// Output - After a guess is validated, print the results: Guess# of #, Bull# Cow#
void PrintTurnSummary(FBullCowCounts BullCowCounts, FString Guess)
{

	std::cout << "\nGuess Result " << BCGame.GetTurn() << "/" << BCGame.GetMaxTries() << ": " << Guess << ", has:\n";
	if (!bBullHints) { std::cout << "Bulltips: " << BullCowCounts.Bulls << "\n"; }
	else if (bBullHints) { std::cout << "Bulltips: -" << BullCowCounts.Bulltips << "-\n"; }

	std::random_shuffle(BullCowCounts.Cowtips.begin(), BullCowCounts.Cowtips.end()); // shuffle Cows

	if (!bCowHints) { std::cout << "Cowtips: " << BullCowCounts.Cows << "\n"; }
	else if (bCowHints) { std::cout << "Cowtips: -" << BullCowCounts.Cowtips << "-\n"; }

	if (bHashHints) { std::cout << "Hashtips: `" << BullCowCounts.Hashtips << "`\n"; }
}

// Output - Game-Phase (Round) Summary generated here:
void PrintPhaseSummary()
{
	if (BCGame.IsPhaseWon())
	{
		std::cout << std::endl << "      !~!~!~!~!~!~!~!       Congratulations on winning this round!"; // TODO Say something random here after 1st print
		std::cout << std::endl << "      !~!~WINNER!~!~!       Guesses: " << BCGame.GetTurn() - 1 << " of " << BCGame.GetMaxTries() << " used";
		std::cout << std::endl << "      !~!~!~!~!~!~!~!       Game Word: ";
		std::cout << BCGame.GetIsogram();
		std::cout << std::endl;
		BCGame.IncrementWins();
	}
	else if (BCGame.GetTurn() >= BCGame.GetMaxTries())
	{
		std::cout << std::endl << "      !~!~!~!~!~!~!~!       It's challenging, isn't it! Don't give up yet!"; // TODO Say something random here after 1st print
		std::cout << std::endl << "      !~!~!LOSER!~!~!       Guesses: " << BCGame.GetTurn() - 1 << " of " << BCGame.GetMaxTries() << " used";
		std::cout << std::endl << "      !~!~!~!~!~!~!~!       Game Word : ";
		for (auto Letter : BCGame.GetIsogram()) { std::cout << "#"; }
		std::cout << std::endl;
		BCGame.IncrementDefeats();
	}
	return;
}

// I/O - Determine if the player wants to continue playing, and/or toggle hints:
bool bAskToPlayAgain()
{
	FText Responce = "";

	if (bBullHints)
	{
		std::cout << std::endl << "[Options: B -#- Bulls, ";
	}
	else
	{
		std::cout << std::endl << "[Options: B abc Bulls, ";
	}

	if (bCowHints)
	{
		std::cout << "C -#- Cows, ";
	}
	else
	{
		std::cout << "C abc Cows, ";
	}

	if (bHashHints)
	{
		std::cout << "H chuck Hashtips] Continue playing? Y/n ";
	}
	else
	{
		std::cout << "H add meaty Hashtips] Continue playing? Y/n ";
	}

	std::getline(std::cin, Responce);
	if ((Responce[0] == 'n') || (Responce[0] == 'N')) { return false; }
	else if ((Responce[0] == 'b') || (Responce[0] == 'B')) { bBullHints = !bBullHints; }
	else if ((Responce[0] == 'c') || (Responce[0] == 'C')) { bCowHints = !bCowHints; }
	else if ((Responce[0] == 'h') || (Responce[0] == 'H')) { bHashHints = !bHashHints; }
	return true;
}

// Output - Spam `X` newlines at the console. Go figure:
void SpamNewline(int32 Repeats)
{
	int32 LoopNumber = 0;
	if (Repeats >= 1)
	{
		do { std::cout << std::endl; LoopNumber++; } while (LoopNumber < Repeats);
	}
	return;
}
