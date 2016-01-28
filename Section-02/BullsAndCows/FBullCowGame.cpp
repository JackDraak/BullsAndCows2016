/*	FBullCowGame.cpp
	created by Jack Draak
	as tutored by Ben Tristem
	Jan.2016 pre-release version 0.9.7

	This class handles the game mechanics of the Bull Cow Game.
	I/O functions are handled in the Main.cpp class.

	See Main.cpp for further description.
*/
#pragma once
#include <algorithm>
#include <map>
#include "FBullCowGame.h"

// required for UnrealEngine-friendly syntax
#define TMap std::map
using int32 = int;

FBullCowGame::FBullCowGame()                   { FBullCowGame::ResetPhase(); }

int32 FBullCowGame::GetBullsNum() const        { return PlayerTotalBullNum; }
int32 FBullCowGame::GetCowsNum() const         { return PlayerTotalCowNum; }
int32 FBullCowGame::GetPhaseLossNum() const    { return PlayerPhaseLosses; }
int32 FBullCowGame::GetPhaseWinNum() const     { return PlayerPhaseWins; }
FString FBullCowGame::GetPlayerGuess() const   { return PlayerGuess; }
int32 FBullCowGame::GetPlayerLevel() const     { return PlayerLevel; }
int32 FBullCowGame::GetPlayerScore() const     { return PlayerScore; }
FString FBullCowGame::GetSecretIsogram() const { return SecretIsogram; }
int32 FBullCowGame::GetTurnLossNum() const     { return PlayerTurnLosses; }
int32 FBullCowGame::GetTurnNum() const         { return PlayerCurrentTurn; }
bool FBullCowGame::IsPhaseWon() const          { return bPlayerGuessMatches; }

void FBullCowGame::IncrementPhaseLossNum()     { PlayerPhaseLosses++; return; }
void FBullCowGame::IncrementPhaseWinNum()      { PlayerPhaseWins++; return; }
void FBullCowGame::IncrementTurn()             { PlayerCurrentTurn++; return; }
void FBullCowGame::IncrementTurnLossNum()      { PlayerTurnLosses++; return; }
void FBullCowGame::UpLevel()                   { PlayerLevel++; return; }
void FBullCowGame::UpScore(int32 Score)        { PlayerScore = PlayerScore + Score; return; }


// ensure the entered guess is alphabetic, isogram & correct # of letters
EGuessQuality FBullCowGame::CheckGuessValidity(const FString& Guess) const
{
	if (!IsWordAlpha(Guess))                           { return EGuessQuality::Not_Alpha; }
	else if (!IsWordIsogram(Guess))                    { return EGuessQuality::Not_Isogram; }
	else if (Guess.length() != SecretIsogram.length()) { return EGuessQuality::Length_Mismatch; }
	else                                               { return EGuessQuality::OK; }
}

// upon reciept of a valid* guess, updates Bull and Cow counts + tips
FGuessAnalysis FBullCowGame::AnalyzeValidGuess(const FString& PlayerGuess)
{
	constexpr char LbChar = '#';
	FGuessAnalysis GuessAnalysis;
	FString SecretWord = SecretIsogram;
	int32 SecretWordLength = SecretIsogram.length();
	GuessAnalysis.Hashtips = std::string(SecretWordLength, LbChar);

	for (int32 SecretWordCharPosition = 0; SecretWordCharPosition < SecretWordLength; SecretWordCharPosition++)
	{
		for (int32 GuessCharPosition = 0; GuessCharPosition < SecretWordLength; GuessCharPosition++)
		{
			const char SecretWordChar = SecretIsogram[SecretWordCharPosition];
			const char GuessWordChar = tolower(PlayerGuess[GuessCharPosition]);
			if (SecretWordChar == GuessWordChar)
			{
				if (SecretWordCharPosition == GuessCharPosition)
				{
					PlayerTotalBullNum++;
					GuessAnalysis.Bulls++;
					GuessAnalysis.Bulltips.append(1, SecretWord[SecretWordCharPosition]);
					GuessAnalysis.Hashtips[SecretWordCharPosition] = SecretWord[SecretWordCharPosition];
				}
				else if (SecretWordCharPosition != GuessCharPosition)
				{
					PlayerTotalCowNum++;
					GuessAnalysis.Cows++;
					GuessAnalysis.Cowtips.append(1, SecretWord[SecretWordCharPosition]);
				//	GuessAnalysis.Hashtips[SecretWordCharPosition] = LbChar;
				}
			}
		}
	}
	// TODO ?add a difficulty setting to allow player to see un-shuffled Cows?
	// shuffle Cows because they're presently sorted to the secret word order:
	std::shuffle(GuessAnalysis.Cowtips.begin(), GuessAnalysis.Cowtips.end(), Entropy);

	if (GuessAnalysis.Bulls == SecretWordLength) 
	{
		// game [DIFFICULTY Tuning: Part A] here: higher scores = more rapid advancement in levels
		constexpr int32 SF_ONE_A = 10; // must be >0
		constexpr int32 SF_ONE_B = 2; // as exponent in Level^N
		int32 ScoreFac1 = SF_ONE_A * CalculateExponent(PlayerLevel +1, SF_ONE_B);
		int32 ScoreFac2 = FBullCowGame::GetMaxTurns() - PlayerCurrentTurn +1;
		int32 Score = ScoreFac1 * ScoreFac2;
		FBullCowGame::UpScore(Score);

		// game [DIFFICULTY Tuning: Part B] here: set benchmarks to gain levels
		if (PlayerLevel == 0 && PlayerScore > 100) { FBullCowGame::UpLevel(); }
		else if (PlayerLevel == 1 && PlayerScore > 300) { FBullCowGame::UpLevel(); }
		else if (PlayerLevel == 2 && PlayerScore > 1000) { FBullCowGame::UpLevel(); }
		else if (PlayerLevel == 3 && PlayerScore > 5000) { FBullCowGame::UpLevel(); }
		else if (PlayerLevel == 4 && PlayerScore > 15000) { FBullCowGame::UpLevel(); }
		else if (PlayerLevel == 5 && PlayerScore > 100000) { FBullCowGame::UpLevel(); }
		else if (PlayerLevel == 6 && PlayerScore > 1500000) { FBullCowGame::UpLevel(); }
		else if (PlayerLevel == 7 && PlayerScore > 5000000) { FBullCowGame::UpLevel(); }
		else if (PlayerLevel == 8 && PlayerScore > 20000000) { FBullCowGame::UpLevel(); }
		FBullCowGame::bPlayerGuessMatches = true;
	}
	else { FBullCowGame::IncrementTurnLossNum(); }
	return GuessAnalysis;
}

// game [DIFFICULTY Tuning: Part C] here: tune the number of guesses a player is given in relation to word-length
int32 FBullCowGame::GetMaxTurns() const
{
	TMap<int32, int32>WordLengthToMaxTurns 
	{
		{ 4, 8 }, 
		{ 5, 10 },
		{ 6, 12 },
		{ 7, 14 }, 
		{ 8, 16 }, 
		{ 9, 18 },
		{ 10, 20 },
		{ 11, 18 },
		{ 12, 16 },
		{ 13, 14 },
		{ 14, 11 },
		{ 15, 9 },
		{ 16, 7 },
		{ 17, 5 }
	};
	return WordLengthToMaxTurns[SecretIsogram.length()];
}

// Initialize a new game state (overloaded: if game is in-play, set-up for a new turn)
void FBullCowGame::ResetPhase()
{
	bool bSecretWordIsIsogram;
	bPlayerGuessMatches = false;
	PlayerCurrentTurn = 1;
	PlayerGuess = "";

	do
	{
		bSecretWordIsIsogram = IsWordIsogram(SelectIsogramForLevel());
	} while (!bSecretWordIsIsogram);

	if (!bNewGameInitialized)
	{
		bNewGameInitialized = true;
		PlayerLevel = 0;
		PlayerPhaseLosses = 0;
		PlayerPhaseWins = 0;
		PlayerScore = 0;
		PlayerTotalBullNum = 0;
		PlayerTotalCowNum = 0;
		PlayerTurnLosses = 0;
	}
	return;
}

FString FBullCowGame::SelectIsogramForLevel()
{
	constexpr int32 INDEX_DEPTH = 30;
	constexpr int32 MAX_PLAYER_LEVEL = 10;
	std::uniform_int_distribution<> IndexDist(0, INDEX_DEPTH - 1);
	std::uniform_int_distribution<> WindowDist(-1, 1);
	int32 ThisIndex = IndexDist(Entropy);
	int32 ThisWindow = WindowDist(Entropy);
	int32 ThisWordLevel = PlayerLevel + ThisWindow;
	if (ThisWordLevel < 0) { ThisWordLevel = 0; }
	else if (ThisWordLevel > MAX_PLAYER_LEVEL -1) { ThisWordLevel = MAX_PLAYER_LEVEL -1; }

	FString SecretWords_0[INDEX_DEPTH] = { 
		"sand", "pair", "raid", "care", "sock", "fair", "hair", "land", "walk", "talk",
		"same", "dart", "this", "from", "suit", "acre", "ages", "bale", "bail", "fast",
		"felt", "fawn", "nape", "army", "navy", "sold", "soda", "soup", "wave", "yarn"
	};
	FString SecretWords_1[INDEX_DEPTH] = {
		"toads", "brick", "stick", "roads", "stand", "trick", "thick", "loads", "talks", "locks",
		"thing", "miles", "lives", "facts", "cloth", "dwarf", "empty", "trash", "envoy", "enact",
		"faith", "farms", "farce", "fairy", "laugh", "lingo", "litre", "march", "marsh", "swift"
	};
	FString SecretWords_2[INDEX_DEPTH] = {
		"jaunts", "abound", "tricks", "bricks", "crawls", "crowns", "around", "orgasm", "bounty", "gizmos",
		"travel", "wealth", "second", "curled", "loving", "belfry", "fables", "factor", "fairly", "famine",
		"farces", "nailed", "nebula", "nickel", "muster", "buster", "myrtle", "nachos", "mythos", "phrase" 
	};
	FString SecretWords_3[INDEX_DEPTH] = {
		"jukebox", "ziplock", "lockjaw", "quickly", "crazily", "jaybird", "jackpot", "quicken", "quicker", "imports",
		"clothes", "polearm", "jockeys", "subject", "cliquey", "apricot", "anxiety", "domains", "dolphin", "exclaim",
		"fabrics", "factory", "haircut", "pulsing", "scourge", "schlump", "turbine", "wrongly", "wyverns", "yoghurt" 
	};
	FString SecretWords_4[INDEX_DEPTH] = {
		"authored", "bankrupt", "hospital", "imported", "questing", "finagled", "question", "speaking", "spectrum", "bunghole",
		"burliest", "bushland", "jockular", "gumption", "pronated", "bushmeat", "buxomest", "busywork", "butchery", "cogently",
		"exoplasm", "exploits", "explains", "exhaling", "handgrip", "hardiest", "hasteful", "megalith", "megatons", "merciful" 
	};
	FString SecretWords_5[INDEX_DEPTH] = {
		"yachtsmen", "worshiped", "workspace", "womanizer", "wolfsbane", "windstorm", "workmates", "wordgames",
		"authorize", "waveforms", "binocular", "watchdogs", "vulgarity", "introduce", "nightmare", "vulcanism",
		"wavefront", "welcoming", "vouchsafe", "verbosity", "veracious", "uncharted", "unclamped", "traveling",
		"tribunals", "solarized", "solemnity", "revolting", "redaction", "racheting" 
	};
	FString SecretWords_6[INDEX_DEPTH] = {
		"abductions", "hospitable", "background", "campground", "greyhounds", "infamously", "afterglows", "shockingly",
		"duplicates", "authorizes", "farsighted", "binoculars", "destroying", "subjectify", "algorithms", "nightmares",
		"aftershock", "agonizedly", "birthnames", "benchmarks", "behaviours", "background", "capsulized", "chlorinate",
		"chipboards", "chalkstone", "exhaustion", "exfoliants", "gobsmacked", "graciously"
	};
	FString SecretWords_7[INDEX_DEPTH] = {
		"workmanship", "palindromes", "speculation", "trampolines", "personality", "sympathizer", "abolishment", "atmospheric",
		"playgrounds", "backgrounds", "countryside", "birthplaces", "precautions", "regulations", "subcategory", "documentary",
		"birthplaces", "bodysurfing", "cabinetwork", "backlighted", "decryptions", "encryptions", "designatory", "delusionary",
		"demographic", "discernably", "exculpatory", "factorylike", "flavourings", "francophile" 
	};
	FString SecretWords_8[INDEX_DEPTH] = {
		"thunderclaps", "misconjugated", "unproblematic", "unprofitable", "questionably", "packinghouse", "upholstering",
		"lexicography", "malnourished", "subordinately", "counterplays", "multipronged", "unforgivable", "subvocalized",
		"exhaustingly", "pyromagnetic", "stenographic", "productively", "stickhandler", "subnormality", "nightwalkers",
		"overstudying", "outsparkling", "locksmithery", "discountable", "descrambling", "demonstrably", "demographics",
		"discrepantly", "considerably" 
	};
	FString SecretWords_9[INDEX_DEPTH] = {
		"subdermatoglyphic", "uncopyrightable", "ambidextrously", "hydromagnetics", "pseudomythical", "flamethrowing",
		"unmaledictory", "ambidextrously", "undiscoverably", "dermatoglyphic", "computerizably", "muckspreading",
		"unsympathized", "unpredictably", "multibranched", "subformatively", "hydropneumatic", "consumptively",
		"metalworkings", "musicotherapy", "chimneyboards", "comsumptively", "copyrightable", "documentarily",
		"draughtswomen", "flowchartings", "lycanthropies", "pneumogastric", "salpingectomy", "subordinately" 
	};
	
	switch (ThisWordLevel)
	{
	case 0:
		SecretIsogram = SecretWords_0[ThisIndex];	break;
	case 1:
		SecretIsogram = SecretWords_1[ThisIndex];	break;
	case 2:
		SecretIsogram = SecretWords_2[ThisIndex];	break;
	case 3:
		SecretIsogram = SecretWords_3[ThisIndex];	break;
	case 4:
		SecretIsogram = SecretWords_4[ThisIndex];	break;
	case 5:
		SecretIsogram = SecretWords_5[ThisIndex];	break;
	case 6:
		SecretIsogram = SecretWords_6[ThisIndex];	break;
	case 7:
		SecretIsogram = SecretWords_7[ThisIndex];	break;
	case 8:
		SecretIsogram = SecretWords_8[ThisIndex];	break;
	case 9:
		SecretIsogram = SecretWords_9[ThisIndex];	break;
	default:
		SecretIsogram = SecretWords_0[ThisIndex];	break;
	}
	return SecretIsogram; // BREAKPOINT here to view secret game word
}


bool FBullCowGame::IsWordIsogram(const FString& Word) const
{
	if (Word.length() <= 1) { return true; }
	TMap<char, bool> LetterSeen;
	for (auto Letter : Word)
	{
		Letter = tolower(Letter);
		if (!LetterSeen[Letter])
		{
			LetterSeen[Letter] = true;
		}
		else { return false; }
	}
	return true;
}

bool FBullCowGame::IsWordAlpha(const FString& Word) const
{
	for (int32 WordChar = 0; WordChar < int32(Word.length()); WordChar++)
	{
		if (!isalpha(Word[WordChar])) return false;
	}
	return true; 
}

int32 FBullCowGame::CalculateExponent(int32 Base, int32 Exponent)
{
	if (Exponent < 1) { return 1; }
	int32 BaseCopy = Base;
	for (int32 i = 1; i < Exponent; i++) { Base = Base * BaseCopy; }
	return Base;
}
