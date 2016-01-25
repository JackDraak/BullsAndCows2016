/*	FBullCowGame.cpp
	created by Jack Draak
	as tutored by Ben Tristem
	Jan.2016 pre-release version 0.9.531

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

FBullCowGame::FBullCowGame()                    { FBullCowGame::Reset(); }
int32 FBullCowGame::GetWins() const             { return MyWins; }
int32 FBullCowGame::GetTurn() const             { return MyCurrentTurn; }
int32 FBullCowGame::GetCows() const             { return MyTotalCow; }
int32 FBullCowGame::GetBulls() const            { return MyTotalBull; }
int32 FBullCowGame::GetLevel() const            { return MyLevel; }
int32 FBullCowGame::GetScore() const            { return MyScore; }
int32 FBullCowGame::GetMisses() const           { return MyMisses; }
bool FBullCowGame::IsPhaseWon() const           { return bGuessMatches; }
int32 FBullCowGame::GetDefeats() const          { return MyDefeats; }
FString FBullCowGame::GetGuess() const          { return MyGuess; }
FString FBullCowGame::GetIsogram() const        { return MyIsogram; }
void FBullCowGame::ScoreUp(int32 Score)         { MyScore = MyScore + Score; return; }
void FBullCowGame::IncrementDefeats()           { MyDefeats++; return; }
void FBullCowGame::IncrementMisses()            { MyMisses++; return; }
void FBullCowGame::IncrementWins()              { MyWins++; return; }
void FBullCowGame::IncrementTry()               { MyCurrentTurn++; return; }
void FBullCowGame::LevelUp()                    { MyLevel++; return; }

// ensure the entered guess is alphabetic, correct # of letters & is an isogram
EGuessStatus FBullCowGame::CheckGuessValidity(const FString& Guess) const
{
	if (!IsWordAlpha(Guess))                       { return EGuessStatus::Not_Alpha; }
	else if (!IsWordIsogram(Guess))                { return EGuessStatus::Not_Isogram; }
	else if (Guess.length() != MyIsogram.length()) { return EGuessStatus::Length_Mismatch; }
	else                                           { return EGuessStatus::OK; }
}

// upon reciept of a valid* guess, updates Bull and Cow counts + tips
FBullCowCounts FBullCowGame::ProcessValidGuess(const FString& Guess)
{
	constexpr char LbChar = '#';
	FBullCowCounts BullCowCounts;
	FString GameWord = MyIsogram;
	int32 GameWordLength = MyIsogram.length();

	// Tally Bulls and Cows, now with tips
	BullCowCounts.Hashtips = std::string(GameWordLength, LbChar);
	for (int32 GameWordCharPosition = 0; GameWordCharPosition < GameWordLength; GameWordCharPosition++)
	{
		for (int32 GuessCharPosition = 0; GuessCharPosition < GameWordLength; GuessCharPosition++)
		{
			const char GameWordChar = MyIsogram[GameWordCharPosition];
			const char GuessWordChar = tolower(Guess[GuessCharPosition]);
			if (GameWordChar == GuessWordChar)
			{
				if (GameWordCharPosition == GuessCharPosition)
				{
					MyTotalBull++;
					BullCowCounts.Bulls++;
					BullCowCounts.Bulltips.append(1, GameWord[GameWordCharPosition]);
					BullCowCounts.Hashtips[GameWordCharPosition] = GameWord[GameWordCharPosition];
				}
				else if (GameWordCharPosition != GuessCharPosition)
				{
					MyTotalCow++;
					BullCowCounts.Cows++;
					BullCowCounts.Cowtips.append(1, GameWord[GameWordCharPosition]);
					BullCowCounts.Hashtips[GameWordCharPosition] = LbChar;
				}
			}
		}
	}

	if (BullCowCounts.Bulls == GameWordLength) 
	{
		// game [DIFFICULTY Tuning: Part A] here: higher scores = more rapid advancement in levels
		constexpr int32 SF_ONE_A = 10; // must be >0
		constexpr int32 SF_ONE_B = 2; // as exponent in Level^N
		int32 ScoreFac1 = SF_ONE_A * PositiveExponentResult(MyLevel +1, SF_ONE_B);
		int32 ScoreFac2 = FBullCowGame::GetMaxTries() - MyCurrentTurn;
		int32 Score = ScoreFac1 * (ScoreFac2 +1);
		FBullCowGame::ScoreUp(Score);

		// game [DIFFICULTY Tuning: Part B] can be acomplished here: set thresholds to gain levels
		if (MyLevel == 0 && MyScore > 100) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 1 && MyScore > 300) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 2 && MyScore > 1000) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 3 && MyScore > 5000) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 4 && MyScore > 15000) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 5 && MyScore > 100000) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 6 && MyScore > 1500000) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 7 && MyScore > 5000000) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 8 && MyScore > 20000000) { FBullCowGame::LevelUp(); }
		FBullCowGame::bGuessMatches = true;
	}
	else { FBullCowGame::IncrementMisses(); }
	return BullCowCounts;
}

// TODO return different Map depending on Player-selected difficulty setting
// game [DIFFICULTY Tuning: Part C] here: tune the number of guesses a player is given in relation to word-length
int32 FBullCowGame::GetMaxTries() const
{
	TMap<int32, int32>WordLengthToMaxTries 
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
	return WordLengthToMaxTries[MyIsogram.length()];
}

// Initialize a new game state (overloaded: if game is in-play, set-up for a new turn)
void FBullCowGame::Reset()
{
	bool bGameWordIsIsogram;
	bGuessMatches = false;
	MyCurrentTurn = 1;
	MyGuess = "";

	do
	{
		bGameWordIsIsogram = IsWordIsogram(SelectIsogramForLevel());
	} while (!bGameWordIsIsogram);

	if (!bDoneOnce)
	{
		MyWins = 0;
		MyScore = 0;
		MyLevel = 0;
		MyMisses = 0;
		MyDefeats = 0;
		MyTotalCow = 0;
		MyTotalBull = 0;
		bDoneOnce = true;
	}
	return;
}

FString FBullCowGame::SelectIsogramForLevel()
{
	constexpr int32 INDEX_DEPTH = 30;
	std::uniform_int_distribution<> IndexDist(0, INDEX_DEPTH - 1);
	std::uniform_int_distribution<> WindowDist(-1, 1);
	int32 RandomIndex = IndexDist(engine);
	int32 ThisWindow = WindowDist(engine);
	int32 ThisWordLevel = MyLevel + ThisWindow;

	FString Words_0[INDEX_DEPTH] = { 
		"sand", "pair", "raid", "care", "sock", "fair", "hair", "land", "walk", "talk",
		"same", "dart", "this", "from", "suit", "acre", "ages", "bale", "bail", "fast",
		"felt", "fawn", "nape", "army", "navy", "sold", "soda", "soup", "wave", "yarn"
	};
	FString Words_1[INDEX_DEPTH] = { 
		"toads", "brick", "stick", "roads", "stand", "trick", "thick", "loads", "talks", "locks",
		"thing", "miles", "lives", "facts", "cloth", "dwarf", "empty", "trash", "envoy", "enact",
		"faith", "farms", "farce", "fairy", "laugh", "lingo", "litre", "march", "marsh", "swift"
	};
	FString Words_2[INDEX_DEPTH] = {
		"jaunts", "abound", "tricks", "bricks", "crawls", "crowns", "around", "orgasm", "bounty", "gizmos",
		"travel", "wealth", "second", "curled", "loving", "belfry", "fables", "factor", "fairly", "famine",
		"farces", "nailed", "nebula", "nickel", "muster", "buster", "myrtle", "nachos", "mythos", "phrase" 
	};
	FString Words_3[INDEX_DEPTH] = {
		"jukebox", "ziplock", "lockjaw", "quickly", "crazily", "jaybird", "jackpot", "quicken", "quicker", "imports",
		"clothes", "polearm", "jockeys", "subject", "cliquey", "apricot", "anxiety", "domains", "dolphin", "exclaim",
		"fabrics", "factory", "haircut", "pulsing", "scourge", "schlump", "turbine", "wrongly", "wyverns", "yoghurt" 
	};
	FString Words_4[INDEX_DEPTH] = { 
		"authored", "bankrupt", "hospital", "imported", "questing", "finagled", "question", "speaking", "spectrum", "bunghole",
		"burliest", "bushland", "jockular", "gumption", "pronated", "bushmeat", "buxomest", "busywork", "butchery", "cogently",
		"exoplasm", "exploits", "explains", "exhaling", "handgrip", "hardiest", "hasteful", "megalith", "megatons", "merciful" 
	};
	FString Words_5[INDEX_DEPTH] = {
		"yachtsmen", "worshiped", "workspace", "womanizer", "wolfsbane", "windstorm", "workmates", "wordgames",
		"authorize", "waveforms", "binocular", "watchdogs", "vulgarity", "introduce", "nightmare", "vulcanism",
		"wavefront", "welcoming", "vouchsafe", "verbosity", "veracious", "uncharted", "unclamped", "traveling",
		"tribunals", "solarized", "solemnity", "revolting", "redaction", "racheting" 
	};
	FString Words_6[INDEX_DEPTH] = {
		"abductions", "hospitable", "background", "campground", "greyhounds", "infamously", "afterglows", "shockingly",
		"duplicates", "authorizes", "farsighted", "binoculars", "destroying", "subjectify", "algorithms", "nightmares",
		"aftershock", "agonizedly", "birthnames", "benchmarks", "behaviours", "background", "capsulized", "chlorinate",
		"chipboards", "chalkstone", "exhaustion", "exfoliants", "gobsmacked", "graciously"
	};
	FString Words_7[INDEX_DEPTH] = {
		"workmanship", "palindromes", "speculation", "trampolines", "personality", "sympathizer", "abolishment", "atmospheric",
		"playgrounds", "backgrounds", "countryside", "birthplaces", "precautions", "regulations", "subcategory", "documentary",
		"birthplaces", "bodysurfing", "cabinetwork", "backlighted", "decryptions", "encryptions", "designatory", "delusionary",
		"demographic", "discernably", "exculpatory", "factorylike", "flavourings", "francophile" 
	};
	FString Words_8[INDEX_DEPTH] = {
		"thunderclaps", "misconjugated", "unproblematic", "unprofitable", "questionably", "packinghouse", "upholstering",
		"lexicography", "malnourished", "subordinately", "counterplays", "multipronged", "unforgivable", "subvocalized",
		"exhaustingly", "pyromagnetic", "stenographic", "productively", "stickhandler", "subnormality", "nightwalkers",
		"overstudying", "outsparkling", "locksmithery", "discountable", "descrambling", "demonstrably", "demographics",
		"discrepantly", "considerably" 
	};
	FString Words_9[INDEX_DEPTH] = {
		"subdermatoglyphic", "uncopyrightable", "ambidextrously", "hydromagnetics", "pseudomythical", "flamethrowing",
		"unmaledictory", "ambidextrously", "undiscoverably", "dermatoglyphic", "computerizably", "muckspreading",
		"unsympathized", "unpredictably", "multibranched", "subformatively", "hydropneumatic", "consumptively",
		"metalworkings", "musicotherapy", "chimneyboards", "comsumptively", "copyrightable", "documentarily",
		"draughtswomen", "flowchartings", "lycanthropies", "pneumogastric", "salpingectomy", "subordinately" 
	};
	
	if (ThisWordLevel < 0) { ThisWordLevel = 0; }
	else if (ThisWordLevel > 9) { ThisWordLevel = 9; }
	switch (ThisWordLevel)
	{
	case 0:
		MyIsogram = Words_0[RandomIndex];	break;
	case 1:
		MyIsogram = Words_1[RandomIndex];	break;
	case 2:
		MyIsogram = Words_2[RandomIndex];	break;
	case 3:
		MyIsogram = Words_3[RandomIndex];	break;
	case 4:
		MyIsogram = Words_4[RandomIndex];	break;
	case 5:
		MyIsogram = Words_5[RandomIndex];	break;
	case 6:
		MyIsogram = Words_6[RandomIndex];	break;
	case 7:
		MyIsogram = Words_7[RandomIndex];	break;
	case 8:
		MyIsogram = Words_8[RandomIndex];	break;
	case 9:
		MyIsogram = Words_9[RandomIndex];	break;
	default:
		MyIsogram = Words_0[RandomIndex];	break;
	}
	return MyIsogram; // BREAKPOINT here to view secret game word
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

int32 FBullCowGame::PositiveExponentResult(int32 Base, int32 Exponent)
{
	if (Exponent < 1) { return 1; }
	int32 BaseCopy = Base;
	for (int32 i = 1; i < Exponent; i++) { Base = Base * BaseCopy; }
	return Base;
}
