#include <iostream>
#include <string>

void PrintIntro();
void PlayGame();
bool bAskToPlayAgain();
void SpitNewline(int Repeats);
std::string GetGuess();

constexpr int MAX_GUESSES = 5;

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

// core game loop
void PlayGame()
{
	// loop for MAX_GUESSES logic
	for (int count = 1; count <= MAX_GUESSES; count++)
	{
		std::string Guess = GetGuess();
		std::cout << "Your guess was: " << Guess << std::endl;
	}
}

// introduce the game
void PrintIntro()
{
	constexpr int WORD_LENGTH = 5;

	SpitNewline(72);
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
	std::cout << std::endl << "Please enter your guess: ";
	std::getline(std::cin, Guess);
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
		return true;
	}
	return false;
}

//  spit X newlines at the console (primarily for a 'clean  and consistent' presentation of the game interface)
void SpitNewline(int Repeats)
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