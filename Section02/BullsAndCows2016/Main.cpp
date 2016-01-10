#include <iostream>
#include "FBullCowGame.h"

void PrintIntro();
void PlayGame();
bool bAskToPlayAgain();
void SpamNewline(int Repeats);
std::string GetGuess();

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
	int MaxTries = BCGame.GetMaxTries();
	std::cout << "\n[MaxTries = " << MaxTries << "]\n";

	// loop through turns
	for (int count = 1; count <= MaxTries; count++)
	{
		std::string Guess = GetGuess();
		std::cout << "Your guess was: " << Guess << std::endl;
	}
}

// introduce the game
void PrintIntro()
{
	constexpr int WORD_LENGTH = 5;

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
std::string GetGuess()
{
	std::string Guess = "";
	int CurrentTry = BCGame.GetCurrentTry();
	int MaxTries = BCGame.GetMaxTries();
	std::cout << std::endl << "Please enter guess #" << CurrentTry << " of " << MaxTries << ": ";
	std::getline(std::cin, Guess);
	BCGame.IncrementTry();
	return Guess;
}

// determine if the player wants to continue playing
bool bAskToPlayAgain()
{
	std::string Responce = "";
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
void SpamNewline(int Repeats)
{
	int LoopNumber = 0;
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