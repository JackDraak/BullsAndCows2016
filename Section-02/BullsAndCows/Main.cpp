/*	Main.cpp
	created by Jack Draak
	as tutored by Ben Tristem
	Jan.2016 pre-release version 0.9.7

	This is the console executable that makes use of the FBullCowGame class.
	This acts as the view in a MVC pattern, and is responsible for all I/O 
    functions. The game mechanics operate in the FBullCowGame.cpp class.

	GAME-LOGIC:
	This is a `Mastermind`-style word guessing game using a small
	dictionary of isograms, tied to a scoring and leveling system
	designed to provide increasingly difficult challenges to one
	or more players working co-oepratively.

	TODO: head-to-head or other competetive modes

	Special Features Include:

		300 word isogram dictionary 

			- Split into 10 levels of difficulty
			- New "Word-Windowing" technology to keep the player on their toes
			  (challenge-word level = player level, window: +/- 1)

		Interactive hint system

			- detailed help page
			- toggle specific or general Bulltips (hints as letters or numbers)
			- toggle specific or general Cowtips (hints as letters or numbers)
			- toggle Hashtips (shows Hash visual-aid for Bulls)

		Stats Galore

			- #'s of Cow and Bull awards
			- Hits, Misses & Near Misses
			- Player rank, from level 1-10
			- Get awarded massive arbitrary "Points" for each win!
			- 518 [C++] Functions behind the scenes!

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
bool bAskToPlayAgain();
bool bBullHints = true;
bool bCowHints = true;
bool bHackedCows = false;
bool bHashHints = false;
FText GetValidGuessFromPlayer();
void MasterControlProgram();
void PrintHelp();
void PrintPhaseIntro();
void PrintPhaseSummary();
void PrintStats();
void PrintTurnSummary(FGuessAnalysis BullCowCounts, const FString& Guess);
void PrintWelcome();
void SpamNewline(int32 Repeats);

// Instantiate a new game named BCGame, which is recycled through each turn and round (or phase):
FBullCowGame BCGame;

// The entry-point for the applciation:
int main()
{
	PrintWelcome();
	do { MasterControlProgram(); } while (bAskToPlayAgain());

	// "End of line." -- program execution complete --
	std::cout << std::endl << "E n d . o f . l i n e . ." << std::endl;
	return 0;
}

// Core game I/O handler method:
void MasterControlProgram()
{
	BCGame.ResetPhase();
	PrintPhaseIntro();
	while (!BCGame.IsPhaseWon() && BCGame.GetTurnNum() <= BCGame.GetMaxTurns())
	{
		FText PlayerGuess = GetValidGuessFromPlayer();
		FGuessAnalysis GuessAnalysis = BCGame.AnalyzeValidGuess(PlayerGuess);
		PrintStats();
		PrintTurnSummary(GuessAnalysis, PlayerGuess);
		BCGame.IncrementTurn();
	}
	PrintPhaseSummary();
	return;
}

// Output - Print game introduction & instruction
void PrintWelcome()
{
	constexpr int32 SPAM_SPAN = 72;
	std::cout << "Version 0.9.7";
	SpamNewline(SPAM_SPAN);
	std::cout << "                      -+-=-+-=-+-=-+-=-+-=-+-=-+-=-+-\n";
	std::cout << "                       Welcome  to  Bulls  and  Cows\n";
	std::cout << "                      -+-=-+-=-+-=-+-=-+-=-+-=-+-=-+-\n\n";
	std::cout << "Your mission, should you choose to accept it, is to guess a secret isogram*.\n\n";
	std::cout << "How to win: After you enter a guess, you will be awarded Bulls and Cows.\n";
	std::cout << "Guessing a correct letter in the correct position is worth one Bull, while a\n";
	std::cout << "correct letter in the wrong position adds one Cow. Use these clues to help\n";
	std::cout << "determine your next guess.\n\n";
	std::cout << "Example: secret word: `veil`, Guess: `vane`, Result: 1 Bull (v) & 1 Cow (e).\n";
	std::cout << "Your hint from the game will be either the numbers of Bulls and Cows, or if you\n";
	std::cout << "use the default 'improved' hints, the game will show you the letters: -v-, -e-\n\n";
	std::cout << " * An isogram is a word comprised of unique letters, for example:\n";
	std::cout << "     - step: is an isogram, each letter is unique in the word\n";
	std::cout << "     - book: is NOT an isogram; it contains two 'o's\n";
	return;
}

// Output - Print new phase intro:
void PrintPhaseIntro()
{
	std::cout << "\nA secret " << BCGame.GetSecretIsogram().length() << "-letter isogram has been selected just for you... Therefore,";
	std::cout << "\nThis round you'll need to earn " << BCGame.GetSecretIsogram().length() << " Bulls in one guess to win. Good luck!\n";
	return;
}

// Output - Print game status (scores) text:
void PrintStats()
{
	std::cout << "\n  |   /\\   /\\   |  Words Lassoed : Words Butchered : " << BCGame.GetPhaseWinNum() << " : " << BCGame.GetPhaseLossNum() << "\n";
	std::cout << "  |   \\ \\_/ /   |      Total Awards -- Bulls, Cows : " << BCGame.GetBullsNum() << ", " << BCGame.GetCowsNum() << "\n";
	std::cout << "  |   ( .^. )   |                      Near Misses : " << BCGame.GetTurnLossNum() << "\n";
	std::cout << "  |    \\ _ /    | (Current Diffuculty Level) Score : (" << BCGame.GetPlayerLevel() + 1 << ") " << BCGame.GetPlayerScore() << "\n";
	std::cout << "  |     (_)     |       Maximum guesses this round : " << BCGame.GetMaxTurns() << "\n";
	return;
}

// I/O - Get a valid guess from the player, loop until satisfied:
FText GetValidGuessFromPlayer()
{
	EGuessQuality Status = EGuessQuality::Invalid_Status;
	FText Guess = "";
	do
	{
		// Acquire input:
		std::cout << std::endl << "Please enter a " << BCGame.GetSecretIsogram().length() << " letter guess, #" << BCGame.GetTurnNum();
		std::cout << " of " << BCGame.GetMaxTurns() << ": ";
		std::getline(std::cin, Guess);

		// Provide meaningful feedback if the guess is invalid:
		Status = BCGame.CheckGuessValidity(Guess);
		switch (Status)
		{
		case EGuessQuality::Length_Mismatch:
			std::cout << "\nOops, that won't work! `" << Guess << "` has " << Guess.length() << " letters.\n";
			break;
		case EGuessQuality::Not_Isogram:
			std::cout << "\nAn isogram doesn't use any single letter more than once, rather than using\n";
			std::cout << "`" << Guess << "`, please try guessing again.\n";
			break;
		case EGuessQuality::Not_Alpha:
			std::cout << "\nYou've entered one or more non-alphabetic characters. Instead of using\n";
			std::cout << "`" << Guess << "`, please try an English isogram word.\n";
			break;
		default: // i.o.w.: case EGuessStatus::OK:
			break;
		}
	} while (Status != EGuessQuality::OK);
	return Guess;
}

// Output - After a guess is validated, print the results: Guess# of #, Bull# Cow#
void PrintTurnSummary(FGuessAnalysis GuessAnalysis, const FString& PlayerGuess)
{
	std::cout << "\nGuess Result " << BCGame.GetTurnNum() << "/" << BCGame.GetMaxTurns() << ": " << PlayerGuess << ", has:\n";
	if (!bBullHints) { std::cout << "Bulltips: " << GuessAnalysis.Bulls << "\n"; }
	else if (bBullHints) { std::cout << "Bulltips: " << GuessAnalysis.Bulltips << "\n"; }
	
	if (!bCowHints) { std::cout << " Cowtips: " << GuessAnalysis.Cows << "\n"; }
	else if (bCowHints && bHackedCows) { std::cout << " Cowtips: " << GuessAnalysis.HackedCows << " (hacked:sorted)\n"; }
	else if (bCowHints && !bHackedCows) { std::cout << " Cowtips: " << GuessAnalysis.CowTips << "\n"; }

	if (bHashHints) { std::cout << "Hashtips: " << GuessAnalysis.Hashtips << "\n"; }
	return;
}

// Output - Game-Phase (Round) Summary generated here:
void PrintPhaseSummary()
{
	if (BCGame.IsPhaseWon())
	{
		std::cout << std::endl << "      !~!~!~!~!~!~!~!       Congratulations on winning this round!"; // TODO Say something random here after 1st print
		std::cout << std::endl << "      !~!~WINNER!~!~!       Guesses: " << BCGame.GetTurnNum() - 1 << " of " << BCGame.GetMaxTurns() << " used";
		std::cout << std::endl << "      !~!~!~!~!~!~!~!       Game Word: ";
		std::cout << BCGame.GetSecretIsogram();
		std::cout << std::endl;
		BCGame.IncrementPhaseWinNum();
	}
	else if (BCGame.GetTurnNum() >= BCGame.GetMaxTurns())
	{
		std::cout << std::endl << "      !~!~!~!~!~!~!~!       It's challenging, isn't it? Don't give up yet!"; // TODO Say something random here after 1st print
		std::cout << std::endl << "      !~!~!LOSER!~!~!       Guesses: " << BCGame.GetTurnNum() - 1 << " of " << BCGame.GetMaxTurns() << " used";
		std::cout << std::endl << "      !~!~!~!~!~!~!~!       Game Word : ";
		for (auto Letter : BCGame.GetSecretIsogram()) { std::cout << "#"; }
		std::cout << std::endl;
		BCGame.IncrementPhaseLossNum();
	}
	return;
}

// I/O - Determine if the player wants to continue playing, and/or toggle options:
bool bAskToPlayAgain()
{
	FText Responce = "";
	std::cout << "\nOptions: You can select a single option this turn --\nHint Options: ";

	if (bBullHints)	{ std::cout << std::endl << "(B)ulls as numbers, "; }
	else { std::cout << std::endl << "(B)ulls as letters, "; }

	if (bCowHints) { std::cout << "(C)ows as numbers, "; }
	else { std::cout << "(C)ows as letters, "; }

	if (bHashHints)	{std::cout << "chuck (H)ashtips.\n"; }
	else { std::cout << "add meaty (H)ashtips.\n"; }

	if (bHackedCows) { std::cout << "Don't <S>how hacked (sorted) Cow letters.\n"; }
	else { std::cout << "<S>how hacked (sorted) Cow letters.\n"; }

	std::cout << "Game Options: (Q)uit, (R)epeat instructions, (D)etailed help\n<Enter> to continue ";

	std::getline(std::cin, Responce);
	if ((Responce[0] == 'q') || (Responce[0] == 'Q')) { return false; }
	else if ((Responce[0] == 'b') || (Responce[0] == 'B')) { bBullHints = !bBullHints; }
	else if ((Responce[0] == 'h') || (Responce[0] == 'H')) { bHashHints = !bHashHints; }
	else if ((Responce[0] == 'r') || (Responce[0] == 'R')) { PrintWelcome(); }
	else if ((Responce[0] == 'd') || (Responce[0] == 'D')) { PrintHelp(); }
	
	else if ((Responce[0] == 'c') || (Responce[0] == 'C')) {
		// C1: True hints then false hints && false hacks
		if (bCowHints) { bCowHints = !bCowHints; bHackedCows = bCowHints; }
		// C2: false hints then true hints // dont care about hacks state
		else { bCowHints = !bCowHints; }
	}
	
	else if ((Responce[0] == 's') || (Responce[0] == 'S')) {
		// H1: true hacks then false hacks // dont care about hint state
		if (bHackedCows) { bHackedCows = !bHackedCows; }
		// H2: false hacks then true hacks && hints
		else { bCowHints = bHackedCows; bHackedCows = !bHackedCows; }
	}
	return true;
}

// Output - Print extended help information:
void PrintHelp ()
{
	//presently covers the interactive help system
	std::cout << "\n";
	std::cout << "The 'interactive hint system' has been designed to try to give you help the way\n";
	std::cout << "you want it, because each player will desire a different level of difficulty.\n";
	std::cout << "The default is to provide no `Hashtip`, but Bull and Cow hints are provided as\n";
	std::cout << "specific letters. The meaning of these hints is illustrated here:\n";
	std::cout << "\n";
	std::cout << "Secret word: `veil`, If you guess: `vane', the hint responces will be:\n"; 
	std::cout << "Bulls -v- and Cows -e-, indicating the V of the guess was a correct letter in\n";
	std::cout << "the correct position (1st letter) while the E does appear in the secret word,\n";
	std::cout << "but *not* in the 4th position and is therefore counted a Cow.\n";
	std::cout << "\n";
	std::cout << "You may chose to make the game more difficult by getting either or both of the\n";
	std::cout << "Bull and Cow hints as only numbers. In other words, given the secret and guess\n"; 
	std::cout << "above, the hint is Bulls:1, Cows:1.  Helpful, but much less than the letters.\n"; 
	std::cout << "\n";
	std::cout << "You may also select the `Hashtips` when options are available between words,\n";
	std::cout << "to give an additional line of output with your result. In the example above,\n";
	std::cout << "the Hastip would be `v###`. Perhaps seeing the letters in their proper places\n";
	std::cout << "will help you think of a next guess! Good luck, and Thank You for playing!\n";
	return;
}

// Output - Repeat `X` newlines out to the console:
void SpamNewline(const int32 Repeats)
{
	int32 LoopNumber = 0;
	if (Repeats >= 1)
	{
		do { std::cout << std::endl; LoopNumber++; } while (LoopNumber < Repeats);
	}
	return;
}
