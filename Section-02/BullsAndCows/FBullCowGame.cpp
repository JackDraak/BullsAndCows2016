/*	FBullCowGame.cpp
*	created by Jack Draak
*	as tutored by Ben Tristem
*	Jan.2016 pre-release version 0.9.43
*
*	This class handles the game mechanics of the Bull Cow Game
*	I/O functions are handled in the Main.cpp class
*
*	see FBullCowGame.h for further description
*/
#pragma once
#include "FBullCowGame.h"
#include <ctime>
#include <map>

// required for UnrealEngine-friendly syntax
#define TMap std::map
using int32 = int;

FBullCowGame::FBullCowGame()                    { FBullCowGame::Reset(); }
FString FBullCowGame::GetRankedIsogram() const  { return MyIsogram; }
int32 FBullCowGame::GetIsogramLength() const    { return (MyIsogram.length()); }
int32 FBullCowGame::GetScore() const            { return MyScore; }
int32 FBullCowGame::GetTurn() const             { return MyCurrentTurn; }
int32 FBullCowGame::GetLevel() const            { return MyLevel; }
int32 FBullCowGame::GetMisses() const           { return MyMisses; }
int32 FBullCowGame::GetDefeats() const          { return MyDefeats; }
int32 FBullCowGame::GetWins() const             { return MyWins; }
int32 FBullCowGame::GetBulls() const            { return MyTotalBull; }
int32 FBullCowGame::GetCows() const             { return MyTotalCow; }
bool FBullCowGame::IsPhaseWon() const           { return bGuessMatches; }
void FBullCowGame::IncrementTry()               { MyCurrentTurn++; return; }
void FBullCowGame::IncrementMisses()            { MyMisses++; return; }
void FBullCowGame::IncrementDefeats()           { MyDefeats++; return; }
void FBullCowGame::IncrementWins()              { MyWins++; return; }
void FBullCowGame::BoostBullScore(int32 Bulls)  { MyTotalBull = MyTotalBull + Bulls; return; }
void FBullCowGame::LevelUp()                    { MyLevel++; return; }
void FBullCowGame::ScoreUp(int32 Score)         { MyScore = MyScore + Score; return; }

// ensure the entered guess is alphabetic, correct # of letters & is an isogram
EGuessStatus FBullCowGame::CheckGuessValidity(FString Guess) const
{
	if (Guess.length() != GetIsogramLength())   { return EGuessStatus::Length_Mismatch; }
	else if (!IsWordIsogram(Guess))             { return EGuessStatus::Not_Isogram; }
	else if (!IsWordAlpha(Guess))               { return EGuessStatus::Not_Alpha; }
	else                                        { return EGuessStatus::OK; }
}

// upon reciept of a valid* guess, increments turn and returns count
FBullCowCounts FBullCowGame::ProcessValidGuess(FString Guess)
{
	FBullCowCounts BullCowCounts;
	int32 GameWordLength = GetIsogramLength();

	TallyBullsAndCows(GameWordLength, Guess, BullCowCounts);
	if (BullCowCounts.Bulls == GameWordLength) 
	{
		// game [DIFFICULTY Tuning: Part A] higher scores basically equate to higher difficulty
		int32 ScoreFac1 = 10 * PositivePowerResult(MyLevel +1, 2);
		int32 ScoreFac2 = FBullCowGame::GetMaxTries() - MyCurrentTurn;
		int32 Score = ScoreFac1 * (ScoreFac2 +1);
		FBullCowGame::ScoreUp(Score);

		// game [DIFFICULTY Tuning: Part B] can be acomplished here: set thresholds to gain levels
		if (MyLevel == 0 && MyScore > 50) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 1 && MyScore > 250) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 2 && MyScore > 1250) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 3 && MyScore > 6750) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 4 && MyScore > 20250) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 5 && MyScore > 60750) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 6 && MyScore > 182250) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 7 && MyScore > 546750) { FBullCowGame::LevelUp(); }
		else if (MyLevel == 8 && MyScore > 1640250) { FBullCowGame::LevelUp(); }
		FBullCowGame::BoostBullScore(MyIsogram.length());
		FBullCowGame::bGuessMatches = true;
	}
	else { FBullCowGame::IncrementMisses(); }
	return BullCowCounts;
}

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

void FBullCowGame::TallyBullsAndCows(const int32 &GameWordLength, FString &Guess, FBullCowCounts &BullCowCounts)
{
	for (int32 MyGameWordChar = 0; MyGameWordChar < GameWordLength; MyGameWordChar++)
	{
		for (int32 GuessChar = 0; GuessChar < GameWordLength; GuessChar++)
		{
			char GameWordChar = MyIsogram[MyGameWordChar];
			char GuessWordChar = tolower(Guess[GuessChar]); // NOTE this is the other call to use tolower() [see also IsWordIsogram()]
			if (GuessWordChar == GameWordChar)
			{
				if (MyGameWordChar == GuessChar) 
				{ 
					BullCowCounts.Bulls++; 
					MyTotalBull++;
				}
				else 
				{ 
					BullCowCounts.Cows++;
					MyTotalCow++;
				}
			}
		}
	}
}

// Initialize a new game state (overloaded: if game InPlay set-up for a new turn)
void FBullCowGame::Reset()
{
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
	bGuessMatches = false;
	MyCurrentTurn = 1;

	bool bGameWordIsIsogram;
	do
	{
		FString MyGameWord = SelectIsogramForLevel();
		bGameWordIsIsogram = IsWordIsogram(MyGameWord);
	} while (!bGameWordIsIsogram);
	return;
}

FString FBullCowGame::SelectIsogramForLevel()
{
	constexpr int32 INDEX_DEPTH = 30; // UPDATE: Each row must grow at the same time to length INDEX_DEPTH

	FString Words_0[INDEX_DEPTH] = { "sand", "pair", "raid", "care", "sock", "fair", "hair", "land", "walk", "talk",
	                                 "same", "dart", "this", "from", "suit", "acre", "ages", "bale", "bail", "fast",
	                                 "felt", "fawn", "nape", "army", "navy", "sold", "soda", "soup", "wave", "yarn" };

	FString Words_1[INDEX_DEPTH] = { "toads", "brick", "stick", "roads", "stand", "trick", "thick", "loads", "talks", "locks",
                                     "thing", "miles", "lives", "facts", "cloth", "dwarf", "empty", "trash", "envoy", "enact", 
	                                 "faith", "farms", "farce", "faery", "laugh", "lingo", "litre", "march", "marsh", "swift" };

	FString Words_2[INDEX_DEPTH] = { "jaunts", "abound", "tricks", "bricks", "crawls", "crowns", "around", "orgasm", "bounty", "gizmos",
		                             "travel", "wealth", "second", "curled", "loving", "belfry", "fables", "factor", "fairly", "famine",
	                                 "farces", "nailed", "nebula", "nickel", "muster", "buster", "myrtle", "nachos", "mythos", "phrase" };

	FString Words_3[INDEX_DEPTH] = { "jukebox", "ziplock", "lockjaw", "quickly", "crazily", "jaybird", "jackpot", "quicken", "quicker", "imports",
		                             "clothes", "polearm", "jockeys", "subject", "cliquey", "apricot", "anxiety", "domains", "dolphin", "exclaim",
	                                 "fabrics", "factory", "haircut", "pulsing", "scourge", "schlump", "turbine", "wrongly", "wyverns", "yoghurt" };

	FString Words_4[INDEX_DEPTH] = { "authored", "bankrupt", "hospital", "imported", "questing", "finagled", "question", "speaking", "spectrum", "bunghole", 
		                             "burliest", "bushland", "jockular", "gumption", "pronated", "bushmeat", "buxomest", "busywork", "butchery", "cogently",
	                                 "exoplasm", "exploits", "explains", "exhaling", "handgrip", "hardiest", "hasteful", "megalith", "megatons", "merciful" };
	
	FString Words_5[INDEX_DEPTH] = { "yachtsmen", "worshiped", "workspace", "womanizer", "wolfsbane", "windstorm", "workmates", "wordgames",
									 "authorize", "waveforms", "binocular", "watchdogs", "vulgarity", "introduce", "nightmare", "vulcanism",
									 "wavefront", "welcoming", "vouchsafe", "verbosity", "veracious", "uncharted", "unclamped", "traveling", 
	                                 "tribunals", "solarized", "solemnity", "revolting", "redaction", "racheting" };

	FString Words_6[INDEX_DEPTH] = { "abductions", "hospitable", "background", "campground", "greyhounds", "infamously", "afterglows", "shockingly", 
		                             "duplicates", "authorizes", "farsighted", "binoculars", "destroying", "subjectify", "algorithms", "nightmares",
	                                 "aftershock", "agonizedly", "birthnames", "benchmarks", "behaviours", "background", "capsulized", "chlorinate",
	                                 "chipboards", "chalkstone", "exhaustion", "exfoliants", "gobsmacked", "graciously" };

	FString Words_7[INDEX_DEPTH] = { "workmanship", "palindromes", "speculation", "trampolines", "personality", "sympathizer", "abolishment", "atmospheric",
									 "playgrounds", "backgrounds", "countryside", "birthplaces", "precautions", "regulations", "subcategory", "documentary",
									 "birthplaces", "bodysurfing", "cabinetwork", "backlighted", "decryptions", "encryptions", "designatory", "delusionary",
									 "demographic", "discernably", "exculpatory", "factorylike", "flavourings", "francophile" };
	
	FString Words_8[INDEX_DEPTH] = { "thunderclaps", "misconjugated", "unproblematic", "unprofitable", "questionably", "packinghouse", "upholstering", 
		                             "lexicography", "malnourished", "subordinately", "counterplays", "multipronged", "unforgivable", "subvocalized", 
		                             "exhaustingly", "pyromagnetic", "stenographic", "productively", "stickhandler", "subnormality", "nightwalkers", 
                                     "overstudying", "outsparkling", "locksmithery", "discountable", "descrambling", "demonstrably", "demographics", 
	                                 "discrepantly", "considerably" };
	
	FString Words_9[INDEX_DEPTH] = { "subdermatoglyphic", "uncopyrightable", "ambidextrously", "hydromagnetics", "pseudomythical", "flamethrowing", 
	                                 "unsympathized", "unpredictably", "multibranched", "subformatively", "hydropneumatic", "consumptively", 
		                             "unmaledictory", "ambidextrously", "undiscoverably", "dermatoglyphic", "computerizably", "muckspreading", 
	                                 "metalworkings", "musicotherapy", "chimneyboards", "comsumptively", "copyrightable", "documentarily", 
	                                 "draughtswomen", "flowchartings", "lycanthropies", "pneumogastric", "salpingectomy", "subordinately" };
	
	// pick random INDEX frame for a game word
	std::srand((unsigned)time(NULL));
	int32 Index = rand() % INDEX_DEPTH; 
	
	// window (or bracket) word-in-play to being +/- 1 from current player level
	constexpr int32 WINDOW = 3;
	int32 Window = (rand() % WINDOW);
	int32 MyWordLevel = MyLevel + Window - 1;
	if (MyWordLevel < 0) { MyWordLevel = 0; }
	else if (MyWordLevel > 9) { MyWordLevel = 9; }
	switch (MyWordLevel)
	{
	case 0:
		MyIsogram = Words_0[Index];	break;
	case 1:
		MyIsogram = Words_1[Index];	break;
	case 2:
		MyIsogram = Words_2[Index];	break;
	case 3:
		MyIsogram = Words_3[Index];	break;
	case 4:
		MyIsogram = Words_4[Index];	break;
	case 5:
		MyIsogram = Words_5[Index];	break;
	case 6:
		MyIsogram = Words_6[Index];	break;
	case 7:
		MyIsogram = Words_7[Index];	break;
	case 8:
		MyIsogram = Words_8[Index];	break;
	case 9:
		MyIsogram = Words_9[Index];	break;
	default:
		MyIsogram = Words_0[Index];	break;
	}
	return MyIsogram; // BREAKPOINT here to view secret game word
}

bool FBullCowGame::IsWordIsogram(FString Word) const
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

bool FBullCowGame::IsWordAlpha(FString Word) const
{
	for (int32 WordChar = 0; WordChar < int32(Word.length()); WordChar++)
	{
		if (!isalpha(Word[WordChar])) return false;
	}
	return true; 
}

int32 FBullCowGame::PositivePowerResult(int32 Base, int32 Exponent)
{
	if (Exponent <= 0) { return 1; }
	if (Exponent == 1) { return Base; }
	int32 n = Base;
	for (int32 i = 1; i < Exponent; i++) { Base = Base * n; }
	return Base;
}
