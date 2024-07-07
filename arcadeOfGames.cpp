#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>
#include <vector>
#include <algorithm>
#include <ctime>
#include <chrono>
#include <thread>
#include <iomanip>
#define ANSI_COLOR_BLACK        "\x1b[30m"
#define ANSI_COLOR_RED          "\x1b[31m"
#define ANSI_COLOR_GREEN        "\x1b[32m"
#define ANSI_COLOR_YELLOW       "\x1b[33m"
#define ANSI_COLOR_BLUE         "\x1b[34m"
#define ANSI_COLOR_MAGENTA      "\x1b[35m"
#define ANSI_COLOR_CYAN         "\x1b[36m"
#define ANSI_COLOR_LIGHT_GRAY   "\x1b[37m"
#define ANSI_COLOR_DARK_GRAY    "\x1b[90m"
#define ANSI_COLOR_LIGHT_RED    "\x1b[91m"
#define ANSI_COLOR_LIGHT_GREEN  "\x1b[92m"
#define ANSI_COLOR_LIGHT_YELLOW "\x1b[93m"
#define ANSI_COLOR_LIGHT_BLUE   "\x1b[94m"
#define ANSI_COLOR_LIGHT_MAGENTA "\x1b[95m"
#define ANSI_COLOR_LIGHT_CYAN   "\x1b[96m"
#define ANSI_COLOR_WHITE        "\x1b[97m"
#define ANSI_COLOR_RESET        "\x1b[0m"

using namespace std;
using namespace std::chrono;

const int wordCount = 2315;
const int wordSize = 5;
const int numTries = 6;
class Wordle
{
private:
    string gameWord, userWord;
    string wordList[wordCount]; // the list of possible words
	int wordCheck[wordSize]; /* checks each letter as the user guesses, setting index to 2 if letter is correct,
	1 if it is in the word but in the wrong place, and 0 otherwise */

public:
    Wordle()
    {
       gameWord="";
       userWord="";
    }
    void WelcomeMessage()
    {
	cout << "Welcome to Wordle! You have six tries to guess the word" << endl;
	cout << "Each guess must be a valid five-letter word. Hit enter to submit." << endl;
	cout << "After each guess, the colors of the letters will change to show how close your guess was to the word." << endl;
	cout << "If the color of the letter is green, that means it is the right letter in the right position." << endl;
	cout << "If the color of the letter is yellow, that means it is a correct letter in the wrong position." << endl;
	cout << "If the color of the letter is grey, that means it is incorrect." << endl;
	cout << "That's it! Good luck and have fun!" << endl;
    }
    void initializeArray()
    {
	    for (int i = 0; i < wordSize; i++)
		wordCheck[i] = 0;
    }
    void pickWord()
    {
	srand(time(NULL));
	int randIndex = rand() % wordCount;  // generates random number from 0 to 2315
	gameWord = wordList[randIndex];
    }
    void ReadWords()
    {
	ifstream file("words.txt");
	if (file.is_open())
    {
		for (int i = 0; i < wordCount; i++)
			file >> wordList[i];
	}
	else
		cout << "Unable to open file" << endl;
	return;
    }
    void PrintWord()
    {
	cout << gameWord;
    }
    void getInput()
    {
	cout << endl;
	cin >> userWord;
	checkInput();
    }
    void checkInput()
    {
	for (int i = 0; i < wordSize; i++)
    {
		if (isupper(userWord[i]))
			userWord[i] = tolower(userWord[i]);
	}
	for (int i = 0; i < wordCount; i++)
    {
		if (userWord == wordList[i])
			return;
	}
	cout << "Bad input, try again.";
	getInput();
    }
    void PrintCheck()
    {
	for (int i = 0; i < wordSize; i++)
	{
		if(wordCheck[i] == 0)
        {
            cout << ANSI_COLOR_DARK_GRAY;
            cout << userWord[i] << " ";
            cout << ANSI_COLOR_RESET;
        }
        else if(wordCheck[i] == 1)
        {
            cout << ANSI_COLOR_YELLOW;
            cout << userWord[i] << " ";
            cout << ANSI_COLOR_RESET;
        }
        else if(wordCheck[i] == 2)
        {
            cout << ANSI_COLOR_GREEN;
            cout << userWord[i] << " ";
            cout << ANSI_COLOR_RESET;
        }
	}
	cout << endl;
    }
    bool checkAnswer()
    {
	int cnt = 0;
	//check each letter and compare
	for (int i = 0; i < wordSize; i++)
    {
		if (userWord[i] == gameWord[i])
			wordCheck[i] = 2;
		else
        {
			for (int j = 0; j < wordSize; j++)
            {
				if (userWord[i] == gameWord[j])
					wordCheck[i] = 1;
		    }
        }
	}
	// check wordCheck to see if it is correct
	for (int i = 0; i < wordSize; i++)
	{
		if (wordCheck[i] == 2)
			cnt++;
	}
	//return 1 if the word is correct, 0 otherwise
	if (cnt == 5)
		return 1;
	else
		return 0;
    }
};
// Function to check if a word is valid based on the given letters
bool isValidWord(const string& word, const string& letters, char center)
{
    if (word.length() < 4)
        return false; // Words must be at least 4 letters long
    if (word.find(center) == string::npos)
        return false; // Center letter must be included

    for (char letter : word)
    {
        if (letters.find(letter) == string::npos)
            return false; // Word must be composed of given letters
    }
    return true;
}

// Function to check if a word is unique in a vector
bool isUnique(const string& word, const vector<string>& words)
{
    return find(words.begin(), words.end(), word) == words.end();
}

// Function to display game instructions
void displayInstructions()
{
    cout << "Welcome to the New York Times Spelling Bee!" << endl;
    cout << "Try to make as many words as you can using the given letters." << endl;
    cout << "The center letter must be included in each word." << endl;
    cout << "Enter 'q' to quit." << endl << endl;
}

// Function to display the list of valid words entered by the user along with points
void displayWords(const vector<string>& words, const vector<int>& points)
{
    cout << ANSI_COLOR_LIGHT_BLUE;
    cout << "Your words and points:" << endl;
    cout << ANSI_COLOR_RESET;
    for (size_t i = 0; i < words.size(); ++i)
    {
        cout << setw(15) << left << words[i] << " - " << points[i] << " points" << endl;
    }
}

// Function to check if a word is present in the dictionary
bool isWordInDictionary(const string& word, const vector<string>& dictionary)
{
    return find(dictionary.begin(), dictionary.end(), word) != dictionary.end();
}

// Function to calculate points based on word length
int calculatePoints(const string& word)
{
    int length = word.length();
    if (length <= 4)
        return 1;
    else if (length == 5)
        return 2;
    else if (length == 6)
        return 3;
    else if (length == 7)
        return 5;
    else
        return 11; // For words longer than 7 letters
}

// Function to display remaining time
void displayRemainingTime(int secondsLeft)
{
    cout << ANSI_COLOR_LIGHT_GREEN;
    cout << "Time left: " << secondsLeft << " seconds" << endl;
    cout << ANSI_COLOR_RESET;
}

// Function to display final message based on the number of words entered
void displayFinalMessage(int totalWords)
{
    if (totalWords == 0)
    {
        cout << "You didn't enter any words. Better luck next time!" << endl;
    }
    else if (totalWords == 1)
    {
        cout << "You entered 1 word. Well done!" << endl;
    }
    else
    {
        cout << "You entered " << totalWords << " words. Great job!" << endl;
    }
}
string scrambleWord(const string& word) {
    string scrambledWord = word;
    random_shuffle(scrambledWord.begin(), scrambledWord.end());
    return scrambledWord;
}
void printBlankLine()
{
    cout << "           " << endl;
}
void printWelcome()
{
    cout << ANSI_COLOR_LIGHT_CYAN;

    printBlankLine();
    printBlankLine();
    printBlankLine();
    printBlankLine();

    cout << "\t\t\t\t\t __      __       .__                               " << endl;
    cout << "\t\t\t\t\t/  \\    /  \\ ____ |  |   ____  ____   _____   ____  " << endl;
    cout << "\t\t\t\t\t\\   \\/\\/   // __ \\|  | _/ ___\\/  _ \\ /     \\_/ __ \\ " << endl;
    cout << "\t\t\t\t\t \\        /\\  ___/|  |_\\  \\__(  <_> )  Y Y  \\  ___/ " << endl;
    cout << "\t\t\t\t\t  \\__/\\  /  \\___  >____/\\___  >____/|__|_|  /\\___  >" << endl;
    cout << "\t\t\t\t\t       \\/       \\/          \\/            \\/     \\/ " << endl;

    cout << ANSI_COLOR_RESET;
}
void printWordle()
{
    cout << ANSI_COLOR_LIGHT_GREEN;

    printBlankLine();
    printBlankLine();
    printBlankLine();
    printBlankLine();

    cout << "\t\t\t\t\t __      __       .__                                ___________     " << endl;
    cout << "\t\t\t\t\t/  \\    /  \\ ____ |  |   ____  ____   _____   ____   \\__    ___/___  " << endl;
    cout << "\t\t\t\t\t\\   \\/\\/   // __ \\|  | _/ ___\\/  _ \\ /     \\_/ __ \\    |    | /  _ \\ " << endl;
    cout << "\t\t\t\t\t \\        /\\  ___/|  |_\\  \\__(  <_> )  Y Y  \\  ___/    |    |(  <_> )" << endl;
    cout << "\t\t\t\t\t  \\__/\\  /  \\___  >____/\\___  >____/|__|_|  /\\___  >   |____| \\____/ " << endl;
    cout << "\t\t\t\t\t       \\/       \\/          \\/            \\/     \\/                  " << endl;

    cout << "\n\n\n\n\n";

    cout << "\t\t\t\t\t\t__      __                   .___.__            " << endl;
    cout << "\t\t\t\t\t\t/  \\    /  \\ ____ _______   __| _/|  |    ____   " << endl;
    cout << "\t\t\t\t\t\t\\   \\/\\/   //  _ \\_  __ \\ / __ | |  |  _/ __ \\  " << endl;
    cout << "\t\t\t\t\t\t \\        /(  <_> )|  | \\/ /_/ | |  |__\\  ___/  " << endl;
    cout << "\t\t\t\t\t\t  \\__/\\  /  \\____/ |__|   \\____ | |____/ \\___  > " << endl;
    cout << "\t\t\t\t\t\t       \\/                      \\/            \\/  " << endl;

    cout << ANSI_COLOR_RESET;

}
void printSpellBee()
{
    cout << ANSI_COLOR_YELLOW;

    cout << "\n\n\n\n";
    cout << "\t\t\t\t\t __      __       .__                                ___________     " << endl;
    cout << "\t\t\t\t\t/  \\    /  \\ ____ |  |   ____  ____   _____   ____   \\__    ___/___  " << endl;
    cout << "\t\t\t\t\t\\   \\/\\/   // __ \\|  | _/ ___\\/  _ \\ /     \\_/ __ \\    |    | /  _ \\ " << endl;
    cout << "\t\t\t\t\t \\        /\\  ___/|  |_\\  \\__(  <_> )  Y Y  \\  ___/    |    |(  <_> )" << endl;
    cout << "\t\t\t\t\t  \\__/\\  /  \\___  >____/\\___  >____/|__|_|  /\\___  >   |____| \\____/ " << endl;
    cout << "\t\t\t\t\t       \\/       \\/          \\/            \\/     \\/                  " << endl;

    cout << "\n\n\n\n";

    cout << "\t\t\t\t\t\t  _________             .__  .__ ___.                  " << endl;
    cout << "\t\t\t\t\t\t /   _____/_____   ____ |  | |  |\\_ |__   ____   ____  " << endl;
    cout << "\t\t\t\t\t\t \\_____  \\\\____ \\_/ __ \\|  | |  | | __ \\_/ __ \\_/ __ \\ " << endl;
    cout << "\t\t\t\t\t\t /        \\  |_> >  ___/|  |_|  |_| \\_\\ \\  ___/\\  ___/ " << endl;
    cout << "\t\t\t\t\t\t/_______  /   __/ \\___  >____/____/___  /\\___  >\\___  >" << endl;
    cout << "\t\t\t\t\t\t        \\/|__|        \\/              \\/     \\/     \\/ " << endl;

    cout << ANSI_COLOR_RESET;
}
void printAnagram()
{
    cout << ANSI_COLOR_LIGHT_CYAN;

    cout << "\n\n\n\n";
    cout << "\t\t\t\t\t __      __       .__                                ___________     " << endl;
    cout << "\t\t\t\t\t/  \\    /  \\ ____ |  |   ____  ____   _____   ____   \\__    ___/___  " << endl;
    cout << "\t\t\t\t\t\\   \\/\\/   // __ \\|  | _/ ___\\/  _ \\ /     \\_/ __ \\    |    | /  _ \\ " << endl;
    cout << "\t\t\t\t\t \\        /\\  ___/|  |_\\  \\__(  <_> )  Y Y  \\  ___/    |    |(  <_> )" << endl;
    cout << "\t\t\t\t\t  \\__/\\  /  \\___  >____/\\___  >____/|__|_|  /\\___  >   |____| \\____/ " << endl;
    cout << "\t\t\t\t\t       \\/       \\/          \\/            \\/     \\/                  " << endl;

    cout << "\n\n\n\n";

    cout << "\t\t\t\t\t  _____                      ____                            " << endl;
    cout << "\t\t\t\t\t /  _  \\    ____  _____     / ___\\ _______ _____     _____   " << endl;
    cout << "\t\t\t\t\t/  /_\\  \\  /    \\ \\__  \\   / /_/  >\\_  __ \\\\__  \\   /     \\  " << endl;
    cout << "\t\t\t\t\t/    |    \\|   |  \\ / __ \\_ \\___  /  |  | \\/ / __ \\_|  Y Y  \\ " << endl;
    cout << "\t\t\t\t\t\\____|__  /|___|  /(__  ) /_____/   |__|   (____  /|__|_|  / " << endl;
    cout << "\t\t\t\t\t        \\/      \\/     \\/                      \\/       \\/  " << endl;

    cout << ANSI_COLOR_RESET;
}
void selectGame()
{
        cout<<"\n\x1B[35m     .d8888b.  8888888888 888      8888888888 .d8888b. 88888888888       .d8888b.         d8888 888b     d888 8888888888 ";
        cout<<"\n\x1B[35m    d88P  Y88b 888        888      888       d88P  Y88b    888          d88P  Y88b       d88888 8888b   d8888 888        ";
        cout<<"\n\x1B[35m    Y88b.      888        888      888       888    888    888          888    888      d88P888 88888b.d88888 888        ";
        cout<<"\n\x1B[35m     \"Y888b.   8888888    888      8888888   888           888          888            d88P 888 888Y88888P888 8888888   ";
        cout<<"\n\x1B[35m        \"Y88b. 888        888      888       888           888          888  88888    d88P  888 888 Y888P 888 888       ";
        cout<<"\n\x1B[35m          \"888 888        888      888       888    888    888          888    888   d88P   888 888  Y8P  888 888       ";
        cout<<"\n\x1B[35m    Y88b  d88P 888        888      888       Y88b  d88P    888          Y88b  d88P  d8888888888 888   \"   888 888       ";
        cout<<"\n\x1B[35m     \"Y8888P\"  8888888888 88888888 8888888888 \"Y8888P\"     888           \"Y8888P88 d88P     888 888       888 8888888888  ";
        cout<<"\n\n\n\n\n";

}
int main()
{
     // Words to be written into the file
    std::vector<std::string> words = {
        "aback", "abase", "abate", "abbey", "abbot", "abhor", "abide", "abled", "abode", "abort",
        "about", "above", "abuse", "abyss", "acorn", "acrid", "actor", "acute", "adage", "adapt",
        "adept", "admin", "admit", "adobe", "adopt", "adore", "adorn", "adult", "affix", "afire",
        "afoot", "afoul", "after", "again", "agape", "agate", "agent", "agile", "aging", "aglow",
        "agony", "agora", "agree", "ahead", "aider", "aisle", "alarm", "album", "alert", "algae",
        "alibi", "alien", "align", "alike", "alive", "allay", "alley", "allot", "allow", "alloy",
        "aloft", "alone", "along", "aloof", "aloud", "alpha", "altar", "alter", "amass", "amaze",
        "amber", "amble", "amend", "amiss", "amity", "among", "ample", "amply", "amuse", "angel",
        "anger", "angle", "angry", "angst", "anime", "ankle", "annex", "annoy", "annul", "anode",
        "antic", "anvil", "aorta", "apart", "aphid", "aping", "apnea", "apple", "apply", "apron",
        "aptly", "arbor", "ardor", "arena", "argue", "arise", "armor", "aroma", "arose", "array",
        "arrow", "arson", "artsy", "ascot", "ashen", "aside", "askew", "assay", "asset", "atoll",
        "atone", "attic", "audio", "audit", "augur", "aunty", "avail", "avert", "avian", "avoid",
        "await", "awake", "award", "aware", "awash", "awful", "awoke", "axial", "axiom", "axion",
        "azure", "bacon", "badge", "badly", "bagel", "baggy", "baker", "baler", "balmy", "banal",
        "banjo", "barge", "baron", "basal", "basic", "basil", "basin", "basis", "baste", "batch",
        "bathe", "baton", "batty", "bawdy", "bayou", "beach", "beady", "beard", "beast", "beech",
        "beefy", "befit", "began", "begat", "beget", "begin", "begun", "being", "belch", "belie",
        "belle", "belly", "below", "bench", "beret", "berry", "berth", "beset", "betel", "bevel",
        "bezel", "bible", "bicep", "biddy", "bigot", "bilge", "billy", "binge", "bingo", "biome",
        "birch", "birth", "bison", "bitty", "black", "blade", "blame", "bland", "blank", "blare",
        "blast", "blaze", "bleak", "bleat", "bleed", "bleep", "blend", "bless", "blimp", "blind",
        "blink", "bliss", "blitz", "bloat", "block", "bloke", "blond", "blood", "bloom", "blown",
        "bluer", "bluff", "blunt", "blurb", "blurt", "blush", "board", "boast", "bobby", "boney",
        "bongo", "bonus", "booby", "boost", "booth", "booty", "booze", "boozy", "borax", "borne",
        "bosom", "bossy", "botch", "bough", "boule", "bound", "bowel", "boxer", "brace", "braid",
        "brain", "brake", "brand", "brash", "brass", "brave", "bravo", "brawl", "brawn", "bread",
        "break", "breed", "briar", "bribe", "brick", "bride", "brief", "brine", "bring", "brink",
        "briny", "brisk", "broad", "broil", "broke", "brood", "brook", "broom", "broth", "brown",
        "brunt", "brush", "brute", "buddy", "budge", "buggy", "bugle", "build", "built", "bulge",
        "bulky", "bully", "bunch", "bunny", "burly", "burnt", "burst", "bused", "bushy", "butch",
        "butte", "buxom", "buyer", "bylaw", "cabal", "cabby", "cabin", "cable", "cacao", "cache",
        "cacti", "caddy", "cadet", "cagey", "cairn", "camel", "cameo", "canal", "candy", "canoe",
        "canon", "canto", "caper", "carat", "carbo", "cards", "caret", "cargo", "carob", "carol",
        "carry", "carte", "carve", "cased", "caste", "catch", "cater", "catty", "caulk", "cause",
        "cavil", "cease", "cedar", "cello", "cento", "ceres", "chafe", "chaff", "chain", "chair",
        "chalk", "champ", "chant", "chaos", "chard", "charm", "chart", "chase", "chasm", "cheap",
        "cheat", "check", "cheek", "cheep", "cheer", "chert", "chess", "chest", "chewy", "chide",
        "chief", "child", "chile", "chill", "chimp", "china", "chine", "chink", "chino", "chirp",
        "chive", "chock", "choir", "choke", "chomp", "chord", "chore", "chose", "chuck", "chuff",
        "chump", "chunk", "churl", "churn", "chute", "cider", "cigar", "cilia", "cinch", "circa",
        "cirri", "civet", "civic", "civil", "civvy", "claim", "clamp", "clang", "clank", "clash",
        "clasp", "class", "clean", "clear", "cleat", "cleek", "clef", "cleft", "clerk", "click",
        "cliff", "climb", "cling", "clink", "clip", "cloak", "clock", "clomp", "clone", "close",
        "cloth", "cloud", "clout", "clove", "clown", "cloy", "club", "cluck", "clump", "clung",
        "clunk", "coach", "coast", "cobra", "cocky", "cocoa", "codex", "codon", "cogent", "cogito",
        "coif", "coil", "coin", "coke", "colby", "colon", "color", "comer", "comma", "comte", "conic",
        "copra", "coral", "corgi", "cotta", "couch", "coupe", "court", "couth", "cower", "coyly",
        "crack", "craft", "crane", "crank", "crass", "crate", "crave", "crawl", "craze", "crazy",
        "creak", "cream", "credo", "creep", "creme", "crepe", "crept", "crest", "crier", "crimp",
        "crisp", "croft", "crook", "croon", "cross", "crowd", "crown", "cruel", "crumb", "crush",
        "crust", "crypt", "cubic", "cubit", "cupid", "cure", "curry", "curse", "curve", "curvy",
        "cushy", "cutie", "cycle", "cygnet", "cynic", "cyst", "czar", "daddy", "daily", "dairy",
        "daisy", "dance", "dandy", "dazed", "dealt", "death", "debit", "debut", "decal", "decor",
        "decry", "decoy", "dedal", "deedy", "deign", "delay", "delta", "delve", "demon", "depot",
        "depth", "derby", "deter", "devil", "devon", "dewey", "dhole", "diary", "diced", "diced",
        "dicta", "diddy", "diker", "dilly", "dimer", "diner", "dingo", "dingy", "dinky", "diode",
        "dirge", "disco", "dishy", "divan", "dived", "diver", "divot", "dizzy", "dodge", "doing",
        "dolce", "dolly", "dolor", "dolts", "donna", "donor", "dotty", "douse", "dowel", "dower",
        "dowry", "downy", "dowry", "dozed", "dozen", "draft", "drain", "drake", "drama", "drank",
        "drape", "drawl", "drawn", "dread", "dream", "dreck", "dress", "dried", "drift", "drill",
        "drink", "dripp", "drive", "droll", "drool", "droop", "drove", "druid", "drunk", "dryly",
        "dryad", "ducat", "duchy", "duffel", "duffle", "dulia", "dully", "dumbo", "dumps", "dumpy",
        "dunce", "dunes", "dunks", "duomo", "duple", "dural", "durst", "dusky", "dusts", "dusty",
        "dutch", "dwell", "dyads", "dynes", "eagle", "early", "earth", "easel", "eaten", "eater",
        "ebony", "echos", "edged", "edict", "edify", "edits", "eerie", "egged", "egret", "eight",
        "eject", "eland", "elate", "elbow", "elder", "elect", "elite", "elope", "elses", "elude",
        "elver", "elves", "embed", "ember", "emcee", "emery", "emote", "empty", "ended", "endow",
        "endue", "enema", "enemy", "enjoy", "enorm", "enter", "entry", "envoy", "epoxy", "equal",
        "equip", "erase", "erect", "ergot", "erode", "error", "erupt", "essay", "ester", "ether",
        "ethic", "ethyl", "etude", "evade", "evens", "event", "every", "evict", "evils", "evoke",
        "exact", "exalt", "exams", "excel", "exert", "exile", "exist", "exits", "expel", "exude",
        "exult", "exurb", "fable", "faced", "facet", "facts", "faded", "fades", "fagot", "fails",
        "faint", "fairs", "fairy", "faith", "faked", "fakes", "fakir", "falls", "false", "famed",
        "fames", "fancy", "fangs", "fanny", "farce", "fared", "fares", "farms", "fasts", "fatal",
        "fated", "fates", "fauna", "favor", "fawns", "faxed", "faxes", "fears", "feast", "feats",
        "fecal", "feces", "feeds", "feels", "feign", "feint", "feist", "felon", "felts", "femur",
        "fence", "fends", "feral", "ferns", "ferry", "fetal", "fetch", "feted", "fetes", "fetid",
        "fetus", "feuds", "fever", "fewer", "fiats", "fiber", "fibre", "fiche", "fidel", "fiefs",
        "field", "fiend", "fiery", "fifes", "fifth", "fifty", "fight", "filch", "filed", "filer",
        "files", "filet", "fills", "filly", "films", "filth", "final", "finch", "finds", "fined",
        "finer", "finis", "finks", "finny", "fired", "fires", "first", "fishy", "fists", "fitly",
        "fiver", "fives", "fixed", "fixer", "fixes", "fjord", "flack", "flags", "flail", "flair",
        "flake", "flaky", "flame", "flank", "flaps", "flare", "flash", "flask", "flats", "flaws",
        "flays", "fleas", "flees", "fleet", "flesh", "flick", "flied", "flier", "flies", "fling",
        "flint", "flips", "flirt", "float", "flock", "floes", "flogs", "flood", "floor", "flops",
        "flora", "floss", "flour", "flout", "flown", "flows", "flubs", "flues", "fluff", "fluid",
        "fluke", "flung", "flunk", "flush", "flute", "flyby", "flyer", "foals", "foams", "foamy",
        "focal", "focus", "foist", "folds", "folks", "folly", "fonts", "foods", "fools", "footy",
        "foray", "force", "fords", "forge", "forgo", "forks", "forms", "forte", "forth", "forty",
        "forum", "fouls", "found", "fount", "fours", "fovea", "foxes", "foyer", "frail", "frame",
        "franc", "frank", "fraps", "frats", "fraud", "frays", "freak", "freed", "freer", "frees",
        "fremd", "fresh", "frets", "friar", "fried", "frier", "fries", "frill", "frisk", "frock",
        "frogs", "frond", "front", "frosh", "frost", "froth", "frown", "froze", "fruit", "frump",
        "fryer", "fudge", "fuels", "fugue", "fully", "fumes", "fumed", "funds", "fungi", "funks",
        "funky", "funny", "furor", "furry", "furze", "fused", "fuses", "fussy", "fusty", "futon",
        "fuzes", "fuzzy", "gabby", "gable", "gaily", "gains", "gaits", "galas", "gales", "galls",
        "gamay", "gamed", "gamer", "games", "gamma", "gamut", "gangs", "gaols", "gaped", "gapes",
        "garbs", "gases", "gassy", "gated", "gates", "gator", "gaudy", "gauge", "gavel", "gawks",
        "gawps", "gazed", "gazes", "gears", "gecko", "geeks", "geese", "gelds", "genes", "genie",
        "genre", "genus", "geode", "germs", "ghost", "ghoul", "giant", "gifts", "gilds", "gills",
        "gimme", "gimps", "gimpy", "gipsy", "girds", "girls", "girly", "girth", "gismo", "given",
        "giver", "gives", "glace", "glade", "gland", "glare", "glass", "glaze", "gleam", "glean",
        "glees", "glens", "glide", "glint", "glitz", "gloat", "globe", "globs", "gloom", "glory",
        "gloss", "glove", "glued", "glues", "gluey", "gluts", "gnarl", "gnash", "gnats", "gnaws",
        "gnome", "goads", "goals", "goats", "gobos", "godly", "goers", "going", "golds", "golem",
        "goner", "gongs", "gonna", "gooey", "goofs", "goofy", "goons", "goose", "gored", "gores",
        "gorge", "gorse", "gotta", "gouge", "gourd", "gouts", "gouty", "gowns", "grabs", "grace",
        "grade", "graft", "grail", "grain", "grams", "grand", "grant", "grape", "graph", "grasp",
        "grass", "grate", "grave", "gravy", "grays", "graze", "great", "grebe", "greed", "greek",
        "green", "greet", "greys", "grids", "grief", "grill", "grime", "grimy", "grind", "grins",
        "gripe", "grips", "grist", "grits", "groan", "groin", "groom", "grope", "gross", "group",
        "grout", "grove", "growl", "grown", "grows", "grubs", "gruel", "gruff", "grump", "grunt",
        "guano", "guard", "guava", "guess", "guest", "guide", "guild", "guile", "guilt", "guise",
        "gulch", "gulfs", "gulls", "gully", "gumbo", "gummy", "gunny", "guppy", "gurus", "gushy",
        "gusto", "gusts", "gusty", "gutsy", "gutty", "guyed", "gypsy", "gyros", "habit", "hadst",
        "hafts", "haiku", "hails", "hairs", "hairy", "hakes", "haled", "haler", "hales", "halos",
        "halts", "halve", "hammy", "hands", "handy", "hangs", "hanks", "hanky", "hants", "happy",
        "hardy", "hared", "harem", "hares", "harks", "harms", "harps", "harpy", "harry", "harsh",
        "harts", "hasps", "haste", "hasty", "hatch", "hated", "hater", "hates", "hauls", "haunt",
        "haven", "haves", "havoc", "hawks", "hayed", "hazel", "hazes", "heads", "heady", "heals",
        "heaps", "heard", "hears", "heart", "heath", "heats", "heave", "heavy", "hedge", "heeds",
        "heels", "heeze", "hefts", "hefty", "heirs", "heist", "helix", "hello", "helms", "helps",
        "helve", "hemps", "hence", "henna", "herbs", "herds", "heres", "heron", "heros", "hertz",
        "hests", "hewed", "hexed", "hexes", "hicks", "hides", "hiked", "hiker", "hikes", "hills",
        "hilly", "hilts", "hilum", "hindu", "hinge", "hints", "hired", "hirer", "hires", "hitch",
        "hived", "hives", "hoagy", "hoard", "hoary", "hobby", "hobos", "hocks", "hoers", "hoggs",
        "hoist", "hokes", "hokey", "holds", "holed", "holes", "holly", "holts", "homes", "homey",
        "honed", "hones", "honey", "honor", "hooch", "hoods", "hoody", "hooks", "hooky", "hoops",
        "hoots", "hoped", "hopes", "horas", "horde", "horse", "horsy", "hosed", "hoses", "hosts",
        "hotel", "hotly", "hound", "hours", "house", "hovel", "hover", "howdy", "howls", "hubby",
        "huffs", "huffy", "huger", "hulas", "hulks", "hullo", "hulls", "human", "humid", "humor",
        "humph", "humps", "humpy", "humus", "hunch", "hunks", "hunt", "hurls", "hurry", "hurst",
        "hurts", "husks", "husky", "hussy", "hutch", "hydra", "hydro", "hyena", "hymen", "hymns",
        "hyoid", "hyper", "hypos", "iambi", "iambs", "icily", "icing", "icons", "ideal", "ideas",
        "idiom", "idiot", "idled", "idler", "idles", "idols", "idyll", "igloo", "igneo", "ileum",
        "ilium", "image", "imams", "imbue", "impel", "imply", "inbox", "incur", "index", "indie",
        "inept", "infer", "infix", "infra", "ingot", "inked", "inlay", "inlet", "inned", "inner",
        "input", "inset", "inter", "intro", "ionic", "iotas", "irate", "irked", "irons", "irony",
        "isles", "issue", "itlls", "ivory", "jabot", "jacks", "jades", "jails", "jakes", "jambs",
        "japan", "japed", "japes", "jargon", "jatos", "jauks", "jaunt", "javas", "jawed", "jazzy",
        "jeans", "jeeps", "jeers", "jefes", "jello", "jells", "jelly", "jenny", "jerks", "jerky",
        "jests", "jetty", "jewel", "jibed", "jiber", "jibes", "jiffs", "jiffy", "jihad", "jilts",
        "jimmy", "jingo", "jivey", "jived", "jives", "jocks", "joins", "joint", "joist", "joked",
        "joker", "jokes", "jokey", "jolly", "jolts", "joule", "joust", "jowls", "jowly", "joyed",
        "judge", "juice", "juicy", "julep", "jumbo", "jumps", "jumpy", "junco", "junks", "junky",
        "junta", "juror", "justs", "jutes", "kabob", "kales", "karma", "kazoo", "kebab", "kebob",
        "keels", "keens", "keeps", "keggy", "kelly", "kelps", "kelpy", "kendo", "kenos", "kepis",
        "kerbs", "ketch", "keyed", "khaki", "kicks", "kicky", "kiddo", "kiddy", "kills", "kilns",
        "kilos", "kilts", "kinds", "kings", "kinks", "kinky", "kiosk", "kited", "kites", "kitsch",
        "kitty", "kiwis", "klieg", "klutz", "knack", "knave", "knead", "kneed", "kneel", "knees",
        "knell", "knelt", "knife", "knits", "knobs", "knock", "knoll", "knops", "knoss", "knots",
        "knows", "knurl", "koala", "kooks", "kooky", "koran", "kraal", "kraut", "krill", "krone",
        "kudzu", "label", "labor", "laced", "lacer", "laces", "lacks", "laden", "lades", "ladle",
        "lager", "laity", "laird", "lairs", "laity", "lakes", "lamas", "lambs", "lamed", "lamer",
        "lames", "lamps", "lance", "lands", "lanes", "lanky", "lapel", "lapse", "larch", "large",
        "largo", "larks", "larva", "laser", "lasso", "lasts", "latch", "lated", "later", "latex",
        "latte", "laugh", "laved", "laves", "lawns", "laxer", "laxes", "layed", "layer", "lazes",
        "leach", "leads", "leafs", "leafy", "leaks", "leaky", "leans", "leaps", "leapt", "learn",
        "lease", "leash", "least", "leave", "ledge", "leech", "leeks", "leers", "leery", "lefts",
        "lefty", "legal", "leggy", "legit", "lemon", "lemur", "lends", "lento", "leper", "letup",
        "levee", "level", "lever", "liars", "libel", "licks", "liege", "liens", "lifer", "lifts",
        "light", "liked", "liken", "likes", "lilac", "lilys", "limbo", "limbs", "limed", "limen",
        "limes", "limit", "limns", "limos", "limps", "lined", "liner", "lines", "lingo", "lings",
        "links", "lints", "linty", "lions", "lipid", "lippy", "lisps", "lists", "lithe", "lived",
        "liven", "liver", "lives", "livid", "llama", "loads", "loafs", "loams", "loamy", "loans",
        "loath", "lobed", "lobes", "local", "lochs", "locks", "locus", "lodes", "lodge", "lofts",
        "lofty", "logan", "loges", "loggy", "logos", "loins", "lolls", "loner", "longs", "loons",
        "loony", "loops", "loopy", "loose", "loots", "lopes", "lords", "lores", "loris", "lorry",
        "loser", "loses", "lossy", "lotto", "lotus", "lough", "loupe", "louse", "lousy", "louts",
        "loved", "lover", "loves", "lowed", "lower", "lowly", "loyal", "luaus", "lucid", "lucks",
        "lucky", "lucre", "luffs", "lulls", "lumen", "lumps", "lumpy", "lunar", "lunch", "lunge",
        "lungs", "lupus", "lurch", "lured", "lurer", "lures", "lurid", "lurks", "lusts", "lusts",
        "lusty", "lutes", "luxes", "lying", "lymph", "lynch", "lyres", "lyric", "macaw", "macho",
        "macro", "madam", "madly", "mafia", "magic", "magma", "magna", "maids", "mails", "maims",
        "mains", "maize", "major", "maker", "makes", "males", "malls", "malts", "mamas", "mamba",
        "mamma", "mange", "mango", "mangy", "mania", "manic", "manly", "manna", "manor", "manse",
        "manus", "maori", "maple", "maqui", "march", "mares", "maria", "marks", "marls", "marry",
        "marsh", "marts", "maser", "masks", "mason", "masse", "massy", "masts", "match", "mated",
        "mater", "mates", "matey", "maths", "matte", "matts", "matty", "mauls", "mauve", "maven",
        "mavin", "maxed", "maxes", "maxim", "mayas", "maybe", "mayor", "mayst", "mazed", "mazes",
        "meals", "mealy", "means", "meant", "meany", "meats", "meaty", "medal", "media", "medic",
        "medly", "meets", "meiny", "melon", "melts", "memos", "mends", "menus", "meows", "mercy",
        "merer", "merge", "merit", "mesas", "mesne", "meson", "messy", "metal", "meted", "meter",
        "metes", "metre", "metro", "mewed", "mewls", "mezzo", "micks", "micro", "middy", "midge",
        "midst", "miffs", "might", "miked", "mikes", "milch", "miler", "miles", "milky", "mills",
        "milts", "mince", "mined", "miner", "mines", "minim", "minis", "minks", "minos", "minor",
        "minos", "mints", "minty", "minus", "mired", "mires", "mirth", "misdo", "miser", "missy",
        "misty", "mites", "mitts", "mixed", "mixer", "mixes", "mizen", "moans", "moats", "mocha",
        "mocks", "modal", "model", "modem", "modes", "modus", "mogul", "moils", "moist", "molar",
        "molds", "moldy", "moles", "molls", "molly", "molts", "momma", "mommy", "money", "monks",
        "month", "mooch", "moods", "moody", "mooed", "moola", "moons", "moony", "moors", "moose",
        "moots", "moped", "moper", "mopes", "mopey", "moral", "moray", "morel", "mores", "morns",
        "morse", "morts", "mosey", "mosks", "moses", "mossy", "mosts", "motel", "motes", "motet",
        "motif", "motor", "motto", "mould", "moult", "mound", "mount", "mourn", "mouse", "mousy",
        "mouth", "moved", "mover", "moves", "movie", "mowed", "mower", "moxie", "muddy", "muffs",
        "mufti", "muggy", "mulch", "mulct", "muled", "mules", "mulls", "mumms", "mummy", "mumps",
        "munch", "muons", "mural", "muras", "mured", "mures", "murks", "murky", "mused", "muser",
        "muses", "mushy", "music", "musks", "musky", "mussy", "musts", "musty", "muted", "mutes",
        "mutts", "muzzy", "mynas", "myrrh", "myths", "nabob", "nacho", "nacre", "nadir", "naiad",
        "naifs", "nails", "naive", "naked", "named", "names", "nanny", "napes", "nappy", "narcs",
        "nards", "nares", "narks", "nasal", "nasty", "natal", "natch", "nates", "natty", "naval",
        "navel", "naves", "navvy", "nears", "neath", "neats", "necks", "needs", "needy", "neons",
        "nerds", "nerdy", "nerve", "nervy", "nests", "nests", "netty", "neuks", "neume", "neuro",
        "neuter", "never", "nevus", "newel", "newer", "newly", "newsy", "newts", "nexus", "nicer",
        "niche", "nicks", "niece", "nifty", "niger", "nighs", "night", "nimbi", "nines", "ninja",
        "ninth", "nippy", "niter", "nitre", "nitty", "nitid", "niton", "nixed", "nixes", "nixie",
        "noble", "nobly", "nocks", "nodal", "noddy", "nodes", "nodus", "noels", "nohow", "noirs",
        "noise", "noisy", "nomad", "nonce", "nooks", "nooky", "noons", "noose", "nopal", "noses",
        "nosey", "notch", "noted", "notes", "nouns", "novae", "novas", "novel", "noway", "nuder",
        "nudes", "nudge", "nudie", "nuked", "nukes", "nulls", "numbs", "numbs", "nurse", "nutty",
        "nylon", "nymph", "oaken", "oakum", "oared", "oases", "oasis", "oaten", "oaths", "oaten",
        "obese", "obeys", "obits", "objet", "oboes", "occur", "ocean", "ocher", "ochre", "octal",
        "octet", "odder", "oddly", "odder", "odeon", "odium", "odors", "odour", "offal", "offed",
        "offer", "often", "ogled", "ogler", "ogles", "ogres", "oiled", "oinks", "okays", "okras",
        "olden", "older", "oldie", "oleic", "oleos", "olios", "olive", "omega", "omens", "omits",
        "onces", "onion", "onset", "oozed", "oozes", "opals", "opens", "opera", "opine", "opium",
        "opted", "optic", "orate", "orbit", "order", "organ", "orlon", "osier", "osmic", "osmol",
        "otter", "ought", "ounce", "ousts", "outdo", "outer", "outgo", "outre", "ouzel", "ouzos",
        "ovals", "ovary", "ovate", "ovens", "overs", "overt", "ovine", "ovoid", "ovule", "owing",
        "owlet", "owned", "owner", "oxbow", "oxide", "ozone", "pacas", "paced", "pacer", "paces",
        "packs", "pacts", "paddy", "padre", "paean", "pagan", "paged", "pager", "pages", "pagod",
        "pails", "pains", "paint", "pairs", "paisa", "paled", "paler", "pales", "palls", "pally",
        "palms", "palmy", "palsy", "pampa", "panda", "pandy", "panel", "panes", "pangs", "panic",
        "pansy", "panto", "pants", "papal", "papas", "paper", "papal", "pappy", "paras", "parch",
        "pardi", "pardy", "pared", "parer", "pares", "pareu", "paris", "parka", "parks", "parol",
        "parry", "parse", "parts", "party", "pasha", "passe", "pasta", "paste", "pasty", "patch",
        "pated", "paten", "pater", "pates", "paths", "patio", "patsy", "patty", "pause", "paved",
        "paves", "pawed", "pawls", "pawns", "paxes", "payed", "payee", "payer", "peace", "peach",
        "peaks", "peals", "peans", "pearl", "pears", "pease", "peats", "peaty", "peavy", "pecan",
        "pecks", "pedal", "peeks", "peels", "peens", "peeps", "peers", "peery", "peeve", "peeks",
        "peels", "peens", "peeps", "peers", "peery", "peeve", "peggy", "peins", "pekan", "pekes",
        "pekin", "pekes", "pekoe", "peles", "pelfs", "pelts", "penal", "pence", "penes", "pengo",
        "penis", "penna", "penne", "penny", "peons", "peony", "pepla", "peppy", "perch", "peril",
        "peris", "perks", "perky", "perms", "perry", "pesky", "pesos", "pests", "pesto", "pesty",
        "petal", "petit", "petty", "pewee", "pewit", "phage", "pharm", "phase", "phial", "phlox",
        "phone", "phony", "photo", "phyla", "physa", "picas", "picks", "picky", "picot", "piece",
        "piers", "pieta", "piety", "piggy", "pigmy", "pikas", "piked", "piker", "pikes", "piles",
        "pilus", "pimas", "pimps", "pinch", "pined", "pines", "piney", "pings", "pinko", "pinks",
        "pinky", "pinon", "pinto", "pints", "pinup", "pions", "pious", "pipal", "piped", "piper",
        "pipes", "pipet", "pipit", "pique", "pitch", "piths", "pithy", "piton", "pivot", "pixel",
        "pixes", "pixie", "pizza", "place", "plaid", "plain", "plait", "plane", "plank", "plans",
        "plant", "plash", "plasm", "plate", "plats", "platy", "playa", "plays", "plaza", "plead",
        "pleas", "pleat", "plebe", "plebs", "plena", "plods", "plonk", "plops", "plots", "plows",
        "ploys", "pluck", "plumb", "plume", "plump", "plums", "plumy", "plunk", "plush", "plyer",
        "poach", "pocks", "pocky", "podgy", "podia", "poems", "poesy", "poets", "pogey", "poked",
        "poker", "pokes", "pokey", "polar", "poled", "poler", "poles", "polio", "polis", "polka",
        "polls", "polyp", "pomps", "ponds", "pones", "pongs", "pooch", "poofs", "poofy", "poohs",
        "pools", "poops", "poori", "popes", "poppa", "poppy", "porch", "pored", "pores", "porgy",
        "porks", "porky", "porno", "porns", "porta", "ports", "posed", "poser", "poses", "posit",
        "posse", "posts", "potsy", "potto", "potty", "pouch", "poufs", "poult", "pound", "pours",
        "pouts", "power", "poxed", "poxes", "prams", "prang", "prank", "praos", "prate", "prats",
        "prawn", "prays", "preen", "preps", "press", "preys", "price", "prick", "pricy", "pride",
        "pried", "prier", "pries", "prigs", "prime", "primp", "print", "prion", "prior", "prise",
        "prism", "privy", "prize", "proas", "probe", "prods", "proem", "profs", "progs", "prole",
        "promo", "proms", "prone", "prong", "proof", "props", "prose", "prosy", "proud", "prove",
        "prowl", "prows", "proxy", "prude", "prune", "psalm", "pshaw", "psych", "pubes", "pubic",
        "pubis", "publy", "pucks", "pudgy", "puffs", "puffy", "puked", "pukes", "pukka", "puler",
        "pules", "pulls", "pulpy", "pulse", "pumas", "pumps", "punch", "punky", "punny", "punts",
        "punty", "pupae", "pupal", "pupas", "pupil", "puppy", "puree", "purer", "purge", "puris",
        "purls", "purrs", "purse", "pushy", "pussy", "putts", "putty", "pygmy", "pylon", "pyres",
        "pyrex", "pyxis", "quack", "quads", "quaff", "quail", "quake", "qualm", "quark", "quart",
        "quash", "quasi", "quays", "queen", "queer", "quell", "query", "quest", "queue", "quick",
        "quids", "quiet", "quill", "quilt", "quint", "quips", "quirk", "quirt", "quite", "quits",
        "quods", "quoin", "quoit", "quond", "quota", "quote", "quoth", "rabat", "rabbi", "rabic",
        "rabid", "raced", "racer", "races", "racks", "radar", "radii", "radio", "radix", "radon",
        "raffs", "rafts", "ragas", "raged", "rages", "raggy", "raids", "rails", "rains", "rainy",
        "raise", "rajah", "rajas", "raked", "rakee", "raker", "rakes", "rally", "ramie", "ramps",
        "ranch", "rands", "randy", "ranee", "range", "rangy", "ranis", "ranks", "rants", "raped",
        "raper", "rapes", "rapid", "rarer", "rased", "raser", "rases", "rasps", "raspy", "rated",
        "ratel", "rater", "rates", "rathe", "ratio", "ratty", "raved", "ravel", "raven", "raves",
        "rawer", "rawly", "raxed", "raxes", "rayed", "rayon", "razed", "razee", "razer", "razes",
        "razor", "reach", "react", "readd", "reads", "ready", "realm", "reals", "reams", "reaps",
        "rearm", "rears", "reave", "rebel", "rebid", "rebus", "recap", "recta", "recto", "recur",
        "redid", "redly", "redon", "redos", "redox", "redry", "reeds", "reedy", "reefs", "reefy",
        "reeks", "reeky", "reels", "reeve", "refer", "refit", "refry", "regal", "reges", "regia",
        "rehab", "reign", "reins", "reive", "relay", "relet", "relic", "relit", "reman", "remap",
        "remet", "remit", "remix", "renal", "rends", "renew", "renig", "renin", "renos", "rente",
        "rents", "reoil", "repay", "repel", "reply", "repos", "repot", "repps", "reran", "rerun",
        "resat", "resaw", "resay", "resee", "reset", "resew", "resin", "resow", "rests", "retch",
        "retem", "retia", "retie", "retro", "retry", "reuse", "revel", "revet", "revue", "rewan",
        "rewax", "rewed", "rewet", "rewon", "rexes", "rheas", "rheum", "rhino", "rhomb", "rhyme",
        "rhyth", "rials", "riant", "riata", "ribby", "ribes", "riced", "ricer", "rices", "ricin",
        "ricks", "rider", "rides", "ridge", "ridgy", "riels", "rifer", "riffs", "rifle", "rifts",
        "right", "rigid", "rigor", "riled", "riles", "riley", "rille", "rills", "rimes", "rimed",
        "rinds", "rings", "rinks", "rinse", "riots", "riped", "ripen", "riper", "ripes", "risen",
        "riser", "rises", "risky", "risus", "rites", "ritzy", "rival", "rived", "riven", "river",
        "rives", "rivet", "riyal", "roach", "roads", "roams", "roans", "roars", "roast", "robed",
        "robes", "robin", "roble", "robot", "rocks", "rocky", "rodeo", "roger", "rogue", "roils",
        "roily", "roles", "rolls", "roman", "romeo", "romps", "rondo", "roods", "roofs", "rooks",
        "rooky", "rooms", "roomy", "roose", "roost", "roots", "rooty", "roped", "roper", "ropes",
        "ropey", "roses", "rosin", "rotes", "rotls", "rotor", "rotos", "rotte", "rotti", "rotty",
        "roues", "rouge", "rough", "round", "rouse", "roust", "route", "routs", "roved", "rover",
        "roves", "rowan", "rowdy", "rowed", "rowel", "rower", "rowth", "royal", "rubes", "ruble",
        "rubys", "ruche", "rucks", "rudds", "ruddy", "ruder", "ruers", "ruffe", "ruffs", "rugby",
        "ruing", "ruins", "ruled", "ruler", "rules", "rumba", "rumen", "rummy", "rumor", "rumps",
        "runes", "rungs", "runic", "runny", "runts", "runty", "rupee", "rural", "ruses", "rushy",
        "rusks", "rusts", "rusty", "ruths", "rutin", "rutty", "saber", "sable", "sabra", "sabre",
        "sacks", "sadly", "safer", "safes", "sagas", "sager", "sages", "saggy", "saids", "sails",
        "saint", "sakes", "salad", "sales", "sally", "salon", "salts", "salty", "salve", "salvo",
        "samba", "samov", "sands", "sandy", "saned", "saner", "sanes", "sanga", "sangs", "sapid",
        "saran", "saree", "sarge", "saris", "sarks", "sassy", "sated", "sates", "satin", "satis",
        "satyr", "sauce", "sauch", "saucy", "sauna", "saute", "saved", "saver", "saves", "savoy",
        "savor", "savvy", "sawed", "sawer", "saxes", "sayed", "sayer", "sayid", "says", "scabs",
        "scads", "scags", "scald", "scale", "scall", "scalp", "scaly", "scamp", "scams", "scans",
        "scant", "scape", "scare", "scarf", "scarp", "scars", "scary", "scats", "scatt", "scaup",
        "scaur", "scena", "scend", "scene", "scent", "schmo", "schwa", "scion", "scoff", "scold",
        "scone", "scoop", "scoot", "scope", "scops", "score", "scorn", "scots", "scour", "scout",
        "scowl", "scows", "scrag", "scram", "scrap", "scree", "screw", "scrim", "scrip", "scrod",
        "scrub", "scrum", "scuba", "scuds", "scuff", "sculk", "scull", "scums", "scups", "scurf",
        "scuta", "scute", "scuts", "seals", "seams", "seamy", "sears", "seats", "sebum", "sects",
        "sedan", "seder", "sedge", "sedgy", "sedum", "seeds", "seedy", "seeks", "seels", "seems",
        "seeps", "seepy", "seers", "segue", "seine", "seise", "seism", "seize", "sells", "semen",
        "semes", "semis", "sends", "sengi", "senna", "senor", "sensa", "sense", "sente", "senti",
        "sepal", "sepia", "sepic", "sepoy", "septa", "septs", "serac", "serai", "seral", "sered",
        "serer", "seres", "serfs", "serge", "serif", "serin", "serum", "serve", "servo", "setae",
        "setal", "seton", "setup", "seven", "sever", "sewed", "sewer", "sexed", "sexes", "sexto",
        "sexts", "shack", "shade", "shads", "shady", "shaft", "shags", "shahs", "shake", "shako",
        "shaky", "shale", "shall", "shalt", "shaly", "shame", "shams", "shank", "shape", "shard",
        "share", "shark", "sharn", "sharp", "shaul", "shave", "shawl", "shawm", "shawn", "shaws",
        "shays", "sheaf", "sheal", "shear", "sheas", "sheds", "sheen", "sheep", "sheer", "sheet",
        "sheik", "shelf", "shell", "shend", "shent", "sheol", "sherd", "shewn", "shews", "shied",
        "shiel", "shier", "shies", "shift", "shill", "shily", "shine", "shins", "shiny", "ships",
        "shire", "shirk", "shirr", "shirt", "shist", "shiva", "shive", "shivs", "shlep", "shoal",
        "shoat", "shock", "shoed", "shoer", "shoes", "shoji", "shone", "shook", "shool", "shoon",
        "shoos", "shoot", "shops", "shore", "shorl", "shorn", "short", "shote", "shots", "shott",
        "shout", "shove", "shown", "shows", "showy", "shoyu", "shred", "shrew", "shris", "shrub",
        "shrug", "shtik", "shuck", "shuls", "shuns", "shunt", "shush", "shute", "shuts", "shyer",
        "shyly", "sials", "sibbs", "sibyl", "sices", "sicko", "sicks", "sided", "sides", "sidle",
        "siege", "sieur", "sieve", "sifts", "sighs", "sight", "sigil", "sigma", "signs", "siker",
        "sikes", "silds", "silex", "silks", "silky", "sills", "silly", "silos", "silts", "silva",
        "simar", "simas", "simps", "since", "sines", "sinew", "singe", "sings", "sinhs", "sinks",
        "sinus", "siped", "sipes", "sired", "siree", "siren", "sires", "sirra", "sirup", "sisal",
        "sises", "sissy", "sitar", "sited", "sites", "situs", "siver", "sixes", "sixmo", "sixte",
        "sixth", "sixty", "sizar", "sized", "sizer", "sizes", "skags", "skald", "skate", "skats",
        "skean", "skeed", "skeen", "skees", "skeet", "skegs", "skein", "skelp", "skene", "skeps",
        "skews", "skids", "skied", "skier", "skies", "skiey", "skiff", "skill", "skimo", "skimp",
        "skims", "skink", "skins", "skint", "skips", "skirl", "skirr", "skirt", "skite", "skits",
        "skive", "skoal", "skuas", "skulk", "skull", "skunk", "skyed", "skyey", "slabs", "slack",
        "slags", "slain", "slake", "slams", "slang", "slank", "slant", "slaps", "slash", "slate",
        "slats", "slaty", "slave", "slaws", "slays", "sleds", "sleek", "sleep", "sleet", "slept",
        "slews", "slice", "slick", "slide", "slier", "slily", "slime", "slims", "slimy", "sling",
        "slink", "slips", "slipt", "slits", "slobs", "sloes", "slogs", "sloid", "slojd", "sloop",
        "slope", "slops", "slosh", "sloth", "slots", "slued", "slues", "sluff", "slugs", "slump",
        "slums", "slung", "slunk", "slurb", "slurp", "slurs", "slush", "sluts", "slyer", "slyly",
        "slype", "smack", "small", "smalt", "smarm", "smart", "smash", "smaze", "smear", "smell",
        "smelt", "smerk", "smews", "smile", "smirk", "smite", "smith", "smock", "smogs", "smoke",
        "smoky", "smolt", "smote", "smuts", "snack", "snafu", "snags", "snail", "snake", "snaky",
        "snaps", "snare", "snarf", "snark", "snarl", "snash", "snath", "snaws", "sneak", "sneap",
        "sneck", "sneds", "sneer", "snell", "snibs", "snick", "snide", "sniff", "snipe", "snips",
        "snits", "snobs", "snood", "snook", "snool", "snoop", "snoot", "snore", "snort", "snots",
        "snout", "snows", "snowy", "snubs", "snuck", "snuff", "snugly", "soaks", "soaps", "soapy",
        "soars", "soave", "sobas", "sober", "socas", "socko", "socks", "socle", "sodas", "soddy",
        "sodic", "sofar", "sofas", "softa", "softs", "softy", "soggy", "soils", "sojas", "sokes",
        "sokol", "solos", "solum", "solus", "soman", "somas", "sonar", "sonde", "sones", "songs",
        "sonic", "sonly", "sonny", "sonsy", "sooth", "soots", "sooty", "sophs", "sophy", "sopor",
        "soppy", "soras", "sorbs", "sords", "sorel", "sorer", "sores", "sorgo", "sorns", "sorry",
        "sorts", "sorus", "sotol", "sough", "souks", "souls", "sound", "soups", "soupy", "sours",
        "souse", "south", "sowar", "sowed", "sower", "soyas", "soyuz", "sozin", "space", "spacy",
        "spade", "spado", "spaed", "spaes", "spahi", "spail", "spait", "spake", "spale", "spall",
        "spang", "spank", "spanx", "spare", "spark", "spars", "spasm", "spate", "spats", "spawn",
        "spays", "speak", "spear", "speck", "specs", "speed", "speel", "speer", "speil", "speir",
        "spell", "spelt", "spend", "spent", "sperm", "spews", "spica", "spice", "spick", "spics",
        "spicy", "spied", "spiel", "spier", "spies", "spiff", "spike", "spiky", "spile", "spill",
        "spilt", "spine", "spins", "spiny", "spire", "spirt", "spiry", "spite", "spits", "spitz",
        "splat", "splay", "split", "spode", "spoil", "spoke", "spoof", "spook", "spool", "spoon",
        "spoor", "spore", "sport", "spots", "spout", "sprag", "sprat", "spray", "spree", "sprig",
        "sprit", "sprit", "sprue", "spuds", "spued", "spues", "spume", "spumy", "spunk", "spurn",
        "spurs", "spurt", "sputa", "squab", "squad", "squat", "squaw", "squeg", "squel", "squib",
        "squid", "stabs", "stack", "stade", "staff", "stage", "stags", "stagy", "staig", "stain",
        "stair", "stake", "stale", "stalk", "stall", "stamp", "stand", "stane", "stang", "stank",
        "staph", "stare", "stark", "stars", "start", "stash", "state", "stats", "stave", "stays",
        "stead", "steak", "steal", "steam", "steed", "steel", "steep", "steer", "stein", "stela",
        "stele", "stems", "steno", "steps", "stere", "stern", "stets", "stews", "stich", "stick",
        "stied", "sties", "stiff", "stile", "still", "stilt", "stime", "stimy", "sting", "stink",
        "stint", "stipe", "stirk", "stirp", "stirs", "stoae", "stoai", "stoas", "stoat", "stock",
        "stogy", "stoic", "stoke", "stole", "stoma", "stomp", "stone", "stony", "stood", "stook",
        "stool", "stoop", "stope", "stops", "stopt", "store", "stork", "storm", "story", "stoss",
        "stoup", "stour", "stout", "stove", "stows", "strap", "straw", "stray", "strep", "strew",
        "stria", "strip", "strop", "strow", "stroy", "strum", "strut", "stubs", "stuck", "studs",
        "study", "stuff", "stull", "stump", "stums", "stung", "stunk", "stuns", "stunt", "stupa",
        "sturt", "styed", "styes", "style", "styli", "stymy", "suave", "subas", "suber", "sucks",
        "sucre", "sudds", "sudor", "sudsy", "suede", "suers", "suets", "suety", "sugar", "sughs",
        "suing", "suint", "suite", "suits", "sulci", "sulfa", "sulfo", "sulks", "sulky", "sully",
        "sulus", "sumac", "summa", "sumos", "sumps", "sunna", "sunns", "sunny", "sunup", "super",
        "supes", "supra", "surah", "sural", "suras", "surds", "surer", "surfs", "surfy", "surge",
        "surgy", "surly", "surra", "sushi", "susli", "sutta", "sutta", "suture", "swabs", "swage",
        "swags", "swail", "swain", "swale", "swami", "swamp", "swamy", "swang", "swank", "swans",
        "swaps", "sward", "sware", "swarf", "swarm", "swart", "swash", "swath", "swats", "sways",
        "swear", "sweat", "swede", "sweep", "sweer", "sweet", "swell", "swept", "swift", "swigs",
        "swill", "swims", "swine", "swing", "swink", "swipe", "swirl", "swish", "swiss", "swith",
        "swive", "swobs", "swoon", "swoop", "swops", "sword", "swore", "sworn", "swots", "swoun",
        "swung", "sycee", "syces", "sykes", "sylis", "sylph", "sylva", "synch", "syncs", "synds",
        "synod", "syphs", "syren", "syrup", "tabby", "taber", "tabes", "tabid", "tabla", "table",
        "taboo", "tabor", "tabun", "tabus", "taces", "tacet", "tache", "tachs", "tacit", "tacks",
        "tacky", "tacos", "tacts", "taels", "taffy", "tafia", "taggy", "tahrs", "taiga", "tails",
        "tains", "taint", "tajes", "takas", "taken", "taker", "takes", "takin", "talar", "talas",
        "talcs", "taler", "tales", "talks", "talky", "tally", "talon", "taluk", "talus", "tamal",
        "tamed", "tamer", "tames", "tamis", "tamps", "tango", "tangs", "tangy", "tanka", "tanks",
        "tansy", "tanto", "tapas", "taped", "taper", "tapes", "tapir", "tapis", "tardo", "tardy",
        "tared", "tares", "targe", "tarns", "taroc", "tarok", "taros", "tarot", "tarps", "tarre",
        "tarry", "tarsi", "tarts", "tasks", "tasse", "taste", "tasty", "tatar", "tater", "tates",
        "tatty", "taunt", "taupe", "tauts", "tawed", "tawer", "tawie", "tawny", "tawse", "taxed",
        "taxer", "taxes", "taxis", "taxon", "taxus", "tazza", "tazze", "teach", "teaks", "teals",
        "teams", "tears", "teary", "tease", "teats", "techs", "techy", "teiid", "teind", "telae",
        "telco", "teles", "telex", "telia", "telic", "tells", "telly", "teloi", "telos", "temps",
        "tempt", "tench", "tends", "tenet", "tenia", "tenon", "tenor", "tense", "tenth", "tents",
        "tenty", "tepal", "tepas", "tepee", "tepid", "terai", "terce", "terga", "terms", "terne",
        "terns", "terra", "terry", "terse", "tesla", "testa", "tests", "testy", "tetra", "texts",
        "thack", "thane", "thank", "tharm", "thaws", "thebe", "theca", "theft", "thegn", "thein",
        "their", "theme", "thens", "there", "therm", "these", "theta", "thews", "thewy", "thick",
        "thief", "thigh", "thill", "thine", "thing", "think", "thins", "thiol", "third", "thirl",
        "thole", "thong", "thorn", "thoro", "thorp", "those", "thous", "thraw", "three", "threw",
        "thrip", "throb", "throe", "throw", "thrum", "thuds", "thugs", "thuja", "thumb", "thump",
        "thurl", "thuya", "thyme", "thymi", "thymy", "tiara", "tibia", "tical", "ticks", "tidal",
        "tided", "tides", "tiers", "tiffs", "tiger", "tight", "tigon", "tikes", "tikis", "tilak",
        "tilde", "tiled", "tiler", "tiles", "tills", "tilth", "tilts", "timed", "timer", "times",
        "timid", "tinct", "tinea", "tined", "tines", "tings", "tinny", "tints", "tipis", "tippy",
        "tipsy", "tired", "tires", "tirls", "tiros", "titan", "titer", "tithe", "titis", "title",
        "titre", "titty", "tizzy", "toads", "toady", "toast", "togue", "toile", "toils", "toits",
        "tokay", "toked", "token", "toker", "tokes", "tolan", "tolas", "toles", "tolus", "tolyl",
        "tombs", "tomes", "tommy", "tonal", "tondi", "tondo", "toned", "toner", "tones", "toney",
        "tonga", "tongs", "tonic", "tonne", "tonus", "tools", "toons", "tooth", "toots", "topaz",
        "toped", "topee", "toper", "topes", "tophi", "tophs", "topic", "topis", "topoi", "topos",
        "toque", "torah", "toras", "torch", "torcs", "tores", "toric", "torii", "toros", "torot",
        "torrs", "torse", "torsi", "torsk", "torso", "torta", "torte", "torts", "torus", "total",
        "toted", "totem", "toter", "totes", "touch", "tough", "tours", "touse", "touts", "towed",
        "towel", "tower", "towie", "towns", "towny", "toxic", "toxin", "toyed", "toyer", "toyon",
        "toyos", "trace", "track", "tract", "trade", "tragi", "traik", "trail", "train", "trait",
        "tramp", "trams", "trank", "traps", "trapt", "trash", "trass", "trave", "trawl", "trays",
        "tread", "treat", "treed", "treen", "trees", "treks", "trend", "tress", "trets", "treys",
        "triac", "triad", "trial", "tribe", "trice", "trick", "tried", "trier", "tries", "trigo",
        "trigs", "trike", "trill", "trims", "trine", "triol", "trios", "tripe", "trips", "trite",
        "trium", "trivs", "troak", "trock", "trode", "trois", "troke", "troll", "tromp", "trona",
        "trone", "troop", "trooz", "trope", "troth", "trots", "trout", "trove", "trows", "troys",
        "truce", "truck", "trued", "truer", "trues", "trull", "truly", "trump", "trunk", "truss",
        "trust", "truth", "tryma", "tryst", "tsade", "tsadi", "tsars", "tsked", "tsuba", "tsuts",
        "tubae", "tubal", "tubas", "tubby", "tubed", "tuber", "tubes", "tucks", "tufas", "tuffs",
        "tufts", "tufty", "tules", "tulip", "tulle", "tumid", "tummy", "tumor", "tumps", "tunas",
        "tuned", "tuner", "tunes", "tungs", "tunic", "tunny", "tupek", "turbo", "turds", "turfs",
        "turfy", "turks", "turns", "turps", "tushy", "tusks", "tutee", "tutor", "tutti", "tutty",
        "tutus", "tuxes", "tuyer", "twaes", "twain", "twang", "twats", "tweak", "tweed", "tween",
        "tweet", "twerp", "twice", "twier", "twigs", "twill", "twine", "twins", "twiny", "twirl",
        "twirp", "twist", "twits", "twixt", "twoes", "twyer", "tyees", "tyers", "tying", "tykes",
        "tyler", "typal", "typed", "types", "typey", "typic", "typos", "typps", "tyred", "tyres",
        "tyros", "tythe", "tzars", "udder", "ugets", "ughes", "uglis", "uhlan", "uhuru", "ukase",
        "ulama", "ulans", "ulcer", "ulema", "ulnad", "ulnae", "ulnar", "ulnas", "ulpan", "ultra",
        "ulvas", "umami", "umbel", "umber", "umbos", "umbra", "umiak", "umiaq", "umpty", "unais",
        "unapt", "unaus", "unban", "unbar", "unbid", "unbox", "uncap", "uncia", "uncle", "uncos",
        "uncus", "uncut", "undee", "under", "undid", "undue", "unfed", "unfit", "unfix", "ungot",
        "unhat", "unhip", "unify", "union", "unite", "units", "unity", "unlay", "unled", "unlet",
        "unlit", "unman", "unmet", "unmew", "unmix", "unpeg", "unpen", "unpin", "unrig", "unrip",
        "unsay", "unset", "unsew", "unsex", "untie", "until", "unwed", "unwet", "unwit", "unwon",
        "unzip", "upbow", "upbye", "updos", "updry", "upend", "uplit", "upped", "upper", "upset",
        "uraei", "urare", "urari", "urase", "urate", "urban", "urbia", "urbs", "ureal", "ureas",
        "uredo", "ureic", "urged", "urger", "urges", "urial", "urine", "urped", "ursae", "ursid",
        "usage", "users", "usher", "using", "usnea", "usque", "usual", "usurp", "usury", "uteri",
        "utile", "utter", "uveal", "uveas", "uvula", "uzbek", "vacua", "vaded", "vades", "vague",
        "vagus", "vails", "vairs", "vakil", "vales", "valet", "valid", "valor", "valse", "value",
        "valve", "vamps", "vanda", "vaned", "vanes", "vangs", "vapid", "vapor", "vapour", "vara",
        "varas", "varia", "varix", "varna", "varus", "varve", "vasal", "vases", "vasts", "vasty",
        "vatic", "vault", "vaunt", "veals", "vealy", "vedic", "veena", "veeps", "veers", "veery",
        "vegan", "veils", "veins", "veiny", "velar", "velds", "veldt", "velum", "venae", "venal",
        "vends", "venin", "venom", "vents", "venue", "verbs", "verge", "versa", "verse", "verso",
        "verst", "verts", "verve", "vesta", "vests", "vetch", "vexed", "vexer", "vexes", "vexil",
        "vials", "viand", "vibes", "vicar", "viced", "vices", "vichy", "video", "viers", "views",
        "viewy", "vigas", "vigil", "vigor", "viler", "villa", "villi", "vills", "vimen", "vinal",
        "vinas", "vinca", "vined", "vines", "vinic", "vinos", "vinum", "vinyl", "viola", "viols",
        "viper", "viral", "vireo", "vires", "virga", "virid", "virls", "virtu", "virus", "visas",
        "vised", "vises", "visit", "visor", "vista", "visto", "vitae", "vital", "vitta", "vivas",
        "vivat", "vivid", "vixen", "vizir", "vizor", "vocal", "voces", "vodka", "vodun", "vogie",
        "vogue", "voice", "voids", "voila", "voile", "volar", "voled", "voles", "volta", "volte",
        "volti", "volts", "volva", "vomer", "vomit", "voted", "voter", "votes", "vouch", "vowed",
        "vowel", "vower", "vroom", "vrouw", "vrows", "vuggs", "vuggy", "vughs", "vulgo", "vulva",
        "vying", "wacke", "wacko", "wacks", "wacky", "waddy", "waded", "wader", "wades", "wadis",
        "wafer", "waffs", "wafts", "waged", "wager", "wages", "wagon", "wahoo", "waifs", "wails",
        "wains", "wairs", "waist", "waits", "waive", "waked", "waken", "waker", "wakes", "waled",
        "waler", "wales", "walks", "walla", "walls", "wally", "waltz", "wames", "wamus", "wands",
        "waned", "wanes", "waney", "wanly", "wants", "wards", "wared", "wares", "warks", "warms",
        "warns", "warps", "warts", "warty", "washy", "wasps", "waspy", "waste", "wasts", "watap",
        "watch", "water", "watts", "waugh", "wauks", "wauls", "waved", "waver", "waves", "wavey",
        "wawls", "waxed", "waxen", "waxer", "waxes", "weald", "weals", "weans", "wears", "weary",
        "weave", "webby", "wedel", "wedge", "wedgy", "weeds", "weedy", "weeks", "weens", "weeny",
        "weeps", "weepy", "weest", "weets", "wefts", "weigh", "weird", "weirs", "wekas", "welch",
        "welds", "wells", "welsh", "welts", "wench", "wends", "wenny", "wests", "wetly", "whack",
        "whale", "whamo", "whams", "whang", "whaps", "wharf", "whats", "wheal", "wheat", "wheel",
        "wheen", "wheep", "whelk", "whelm", "whelp", "where", "whets", "whews", "wheys", "which",
        "whids", "whiff", "whigs", "while", "whims", "whine", "whins", "whiny", "whips", "whipt",
        "whirl", "whirr", "whirs", "whish", "whisk", "whist", "white", "whits", "whity", "whizz",
        "whole", "whomp", "whoof", "whoop", "whops", "whore", "whorl", "whort", "whose", "whoso",
        "whump", "wicks", "widdy", "widen", "wider", "wides", "widow", "width", "wield", "wifed",
        "wifes", "wifey", "wifty", "wigan", "wiggy", "wiggy", "wight", "wikis", "wilds", "wiled",
        "wiles", "wills", "willy", "wilts", "wimps", "wimpy", "wince", "winch", "winds", "windy",
        "wined", "wines", "winey", "wings", "wingy", "winks", "winos", "winze", "wiped", "wiper",
        "wipes", "wired", "wirer", "wires", "wirra", "wised", "wiser", "wises", "wishy", "wisps",
        "wispy", "wists", "witan", "witch", "wited", "wites", "withe", "withy", "witty", "wived",
        "wiver", "wives", "wizen", "wizes", "woads", "woald", "woful", "woken", "wolds", "wolfs",
        "woman", "wombs", "women", "wonky", "wonts", "woods", "woody", "wooed", "wooer", "woofs",
        "wools", "wooly", "woops", "woosh", "woozy", "words", "wordy", "works", "world", "worms",
        "wormy", "worry", "worse", "worst", "worth", "would", "wound", "woven", "wowed", "wrack",
        "wraith", "wrang", "wrap", "wraps", "wrapt", "wrath", "wreak", "wreck", "wrest", "wrick",
        "wried", "wrier", "wries", "wring", "wrist", "write", "writs", "wrong", "wrote", "wroth",
        "wrung", "wryer", "wryly", "wurst", "wussy", "wyled", "wyles", "wynds", "wynns", "wyted",
        "wytes", "xebec", "xenia", "xenic", "xenon", "xeric", "xerox", "xerus", "xylan", "xylem",
        "xylol", "xylyl", "xysti", "xysts", "yacht", "yacks", "yaffs", "yager", "yagis", "yahoo",
        "yaird", "yamen", "yamun", "yangs", "yanks", "yapok", "yapon", "yapps", "yappy", "yards",
        "yarer", "yarns", "yauld", "yaups", "yawed", "yawls", "yawns", "yawps", "yeahs", "years",
        "yeast", "yecch", "yechs", "yechy", "yeggs", "yelks", "yells", "yelps", "yenta", "yente",
        "yerba", "yerks", "yeses", "yetis", "yetts", "yeuks", "yeuky", "yield", "yikes", "yills",
        "yince", "yipes", "yirds", "yirrs", "yirth", "yobbo", "yocks", "yodel", "yodhs", "yodle",
        "yogas", "yogee", "yoghs", "yogic", "yogin", "yogis", "yoked", "yokel", "yokes", "yolks",
        "yolky", "yomim", "yonic", "yonis", "yonks", "yores", "young", "yourn", "yours", "youse",
        "youth", "yowed", "yowes", "yowie", "yowls", "yuans", "yucas", "yucca", "yucch", "yucks",
        "yucky", "yugas", "yulan", "yules", "yummy", "yupon", "yurta", "yurts", "zaire", "zamia",
        "zanza", "zappy", "zarfs", "zaxes", "zayin", "zazen", "zeals", "zebec", "zebra", "zebus",
        "zedo", "zeds", "zeins", "zerks", "zeros", "zests", "zesty", "zetas", "zibet", "zilch",
        "zills", "zincs", "zincy", "zineb", "zings", "zingy", "zinky", "zippy", "ziram", "zitis",
        "zizit", "zlote", "zloty", "zoeae", "zoeal", "zoeas", "zombi", "zonal", "zoned", "zoner",
        "zones", "zonks", "zooid", "zooks", "zooms", "zoons", "zooty", "zoril", "zoris", "zowie",
        "zupas", "zygal", "zymes"};

        ofstream outFile("words.txt");

    if (outFile.is_open()) {
        // Write 5-letter words to the file
        for (const string& word : words) {
            if (word.size() == 5) {
                outFile << word << endl;
            }
        }
        outFile.close();
    } else {
        cerr << "Unable to open the file for writing." << endl;
    }

    printBlankLine();
    printBlankLine();
    printBlankLine();
    printBlankLine();
    printBlankLine();
    printWelcome();
    printBlankLine();
    printBlankLine();
    system("pause");
    system("cls");
    selectGame();
    cout << ANSI_COLOR_RESET;
    printBlankLine();
    printBlankLine();
    int n;
    cout<<"Select Game : "<<endl;
    cout<<"Enter 1 to play Wordle, 2 to play SpellBee and 3 to play Anagrams : "<<endl;
    cin>>n;
    if(n==1)
    {
    system("cls");
    printWordle();
    printBlankLine();
    printBlankLine();
    system("pause");
    system("cls");
    Wordle gameWord;
	int counter = 0;
	gameWord.WelcomeMessage();
	gameWord.ReadWords();
	gameWord.pickWord();
	for (int i = 1; i < numTries + 1; i++)
	{
		gameWord.initializeArray();
		cout << ANSI_COLOR_LIGHT_BLUE;
		cout << "Guess " << i;
		cout << ANSI_COLOR_RESET;
		gameWord.getInput();
		if (gameWord.checkAnswer())
        {
			cout << "You win!"<<endl;
			cout<<"The correct word is : ";
			gameWord.PrintWord();
			cout<<endl;
			return 0;
		}
		else
			gameWord.PrintCheck();
	}
	cout << ANSI_COLOR_LIGHT_MAGENTA;
	cout << "Sorry, out of tries. Better luck next time!" << endl;
	cout << ANSI_COLOR_RESET;
	cout<<"The correct word is : ";
	gameWord.PrintWord();
	cout<<endl;
	system("pause");
    system("cls");
    //Thank You
	cout<<"\n\n";
    cout<<"\n\x1B[31;96m                              88888888888 888    888        d8888 888b    888 888    d8P   .d8888b.       8888888888 .d88888b.  8888888b.\x1B[0m  ";
    cout<<"\n\x1B[31;96m                                  888     888    888       d88888 8888b   888 888   d8P   d88P  Y88b      888       d88P\" \"Y88b 888   Y88b\x1B[0m ";
    cout<<"\n\x1B[31;96m                                  888     888    888      d88P888 88888b  888 888  d8P    Y88b.           888       888     888 888    888\x1B[0m ";
    cout<<"\n\x1B[31;96m                                  888     8888888888     d88P 888 888Y88b 888 888d88K      \"Y888b.        8888888   888     888 888   d88P\x1B[0m ";
    cout<<"\n\x1B[31;96m                                  888     888    888    d88P  888 888 Y88b888 8888888b        \"Y88b.      888       888     888 8888888P\"\x1B[0m  ";
    cout<<"\n\x1B[31;96m                                  888     888    888   d88P   888 888  Y88888 888  Y88b         \"888      888       888     888 888 T88b\x1B[0m   ";
    cout<<"\n\x1B[31;96m                                  888     888    888  d8888888888 888   Y8888 888   Y88b  Y88b  d88P      888       Y88b. .d88P 888  T88b\x1B[0m  ";
    cout<<"\n\x1B[31;96m                                  888     888    888 d88P     888 888    Y888 888    Y88b  \"Y8888P\"       888        \"Y88888P\"  888   T88b\x1B[0m ";
    cout<<"\n\n\n\n";
    cout<<"\n\x1B[31;96m                                              8888888b.  888             d8888 Y88b   d88P 8888888 888b    888  .d8888b.  888\x1B[0m              ";
    cout<<"\n\x1B[31;96m                                              888   Y88b 888            d88888  Y88b d88P    888   8888b   888 d88P  Y88b 888\x1B[0m              ";
    cout<<"\n\x1B[31;96m                                              888    888 888           d88P888   Y88o88P     888   88888b  888 888    888 888\x1B[0m              ";
    cout<<"\n\x1B[31;96m                                              888   d88P 888          d88P 888    Y888P      888   888Y88b 888 888        888\x1B[0m              ";
    cout<<"\n\x1B[31;96m                                              8888888P\"  888         d88P  888     888       888   888 Y88b888 888  88888 888\x1B[0m              ";
    cout<<"\n\x1B[31;96m                                              888        888        d88P   888     888       888   888  Y88888 888    888 Y8P\x1B[0m              ";
    cout<<"\n\x1B[31;96m                                              888        888       d8888888888     888       888   888   Y8888 Y88b  d88P  \"\x1B[0m               ";
    cout<<"\n\x1B[31;96m                                              888        88888888 d88P     888     888     8888888 888    Y888  \"Y8888P88 888\x1B[0m              ";
    cout<<"\n\n\n\n";
    }
    else if(n==2)
    {
    system("cls");
    printSpellBee();
    printBlankLine();
    printBlankLine();
    system("pause");
    system("cls");
    string letters;
    char center;
    vector<string> words;
    vector<int> points;
    vector<string> dictionary;

    // Load dictionary from file
    ifstream infile("words.txt");
    if (!infile.is_open())
    {
        cerr << "Error: Unable to open dictionary file." << endl;
        return 1;
    }

    string word;
    while (infile >> word)
    {
        dictionary.push_back(word);
    }
    infile.close();

    displayInstructions();

    cout << "Enter the 7 letters followed by the center letter: ";
    cin >> letters >> center;

    // Setting time limit
    constexpr int timeLimitSec = 60; // 1 minute
    cout << "You have 1 minute to make as many 5 letter words as you can." << endl;
    auto start = chrono::steady_clock::now();
    int totalWords = 0;
    while (chrono::steady_clock::now() - start < chrono::seconds(timeLimitSec))
    {
        string word;
        cout << ANSI_COLOR_LIGHT_CYAN;
        cout << "Enter a word (or 'q' to quit): ";
        cout << ANSI_COLOR_RESET;
        cin >> word;
        if (word == "q")
            break;

        if (isValidWord(word, letters, center) && isUnique(word, words))
        {
            if (isWordInDictionary(word, dictionary))
            {
                cout << "Valid word!" << endl;
                int wordPoints = calculatePoints(word);
                words.push_back(word);
                points.push_back(wordPoints);
                totalWords++;
            }
            else
            {
                cout << "Valid word, but not present in the dictionary." << endl;
            }
        }
        else if (!isValidWord(word, letters, center))
        {
            cout << ANSI_COLOR_LIGHT_RED;
            cout << "Invalid word. Make sure it's at least 4 letters long and contains the center letter." << endl;
            cout << ANSI_COLOR_RESET;
        }
        else
        {
            cout << "You've already entered that word. Try a different one." << endl;
        }

        // Display remaining time after each word entry
        int secondsElapsed = chrono::duration_cast<chrono::seconds>(chrono::steady_clock::now() - start).count();
        int secondsLeft = timeLimitSec - secondsElapsed;
        if (secondsLeft > 0)
        {
            displayRemainingTime(secondsLeft);
        }
    }

    // Display results
    displayWords(words, points);

    // Calculate total points
    int totalPoints = 0;
    for (int point : points)
    {
        totalPoints += point;
    }
    cout << ANSI_COLOR_LIGHT_CYAN;
    cout << "Total points: " << totalPoints << endl;
    cout << ANSI_COLOR_RESET;

    // Display final message based on the number of words entered
    displayFinalMessage(totalWords);
    system("pause");
    system("cls");
	//Thank You
	cout<<"\n\n";
    cout<<"\n\x1B[31;96m                              88888888888 888    888        d8888 888b    888 888    d8P   .d8888b.       8888888888 .d88888b.  8888888b.\x1B[0m  ";
    cout<<"\n\x1B[31;96m                                  888     888    888       d88888 8888b   888 888   d8P   d88P  Y88b      888       d88P\" \"Y88b 888   Y88b\x1B[0m ";
    cout<<"\n\x1B[31;96m                                  888     888    888      d88P888 88888b  888 888  d8P    Y88b.           888       888     888 888    888\x1B[0m ";
    cout<<"\n\x1B[31;96m                                  888     8888888888     d88P 888 888Y88b 888 888d88K      \"Y888b.        8888888   888     888 888   d88P\x1B[0m ";
    cout<<"\n\x1B[31;96m                                  888     888    888    d88P  888 888 Y88b888 8888888b        \"Y88b.      888       888     888 8888888P\"\x1B[0m  ";
    cout<<"\n\x1B[31;96m                                  888     888    888   d88P   888 888  Y88888 888  Y88b         \"888      888       888     888 888 T88b\x1B[0m   ";
    cout<<"\n\x1B[31;96m                                  888     888    888  d8888888888 888   Y8888 888   Y88b  Y88b  d88P      888       Y88b. .d88P 888  T88b\x1B[0m  ";
    cout<<"\n\x1B[31;96m                                  888     888    888 d88P     888 888    Y888 888    Y88b  \"Y8888P\"       888        \"Y88888P\"  888   T88b\x1B[0m ";
    cout<<"\n\n\n\n";
    cout<<"\n\x1B[31;96m                                              8888888b.  888             d8888 Y88b   d88P 8888888 888b    888  .d8888b.  888\x1B[0m              ";
    cout<<"\n\x1B[31;96m                                              888   Y88b 888            d88888  Y88b d88P    888   8888b   888 d88P  Y88b 888\x1B[0m              ";
    cout<<"\n\x1B[31;96m                                              888    888 888           d88P888   Y88o88P     888   88888b  888 888    888 888\x1B[0m              ";
    cout<<"\n\x1B[31;96m                                              888   d88P 888          d88P 888    Y888P      888   888Y88b 888 888        888\x1B[0m              ";
    cout<<"\n\x1B[31;96m                                              8888888P\"  888         d88P  888     888       888   888 Y88b888 888  88888 888\x1B[0m              ";
    cout<<"\n\x1B[31;96m                                              888        888        d88P   888     888       888   888  Y88888 888    888 Y8P\x1B[0m              ";
    cout<<"\n\x1B[31;96m                                              888        888       d8888888888     888       888   888   Y8888 Y88b  d88P  \"\x1B[0m               ";
    cout<<"\n\x1B[31;96m                                              888        88888888 d88P     888     888     8888888 888    Y888  \"Y8888P88 888\x1B[0m              ";
    cout<<"\n\n\n\n";
    }
    else if(n==3)
    {
        system("cls");
        printAnagram();
        printBlankLine();
        printBlankLine();
        system("pause");
        system("cls");

        srand(time(0)); // Seed the random number generator

    vector<string> words;

    ifstream inputFile("words.txt"); // Open the file containing words
    string word;

    // Read words from file
    while (getline(inputFile, word))
    {
        words.push_back(word);
    }

    inputFile.close(); // Close the file

    cout << ANSI_COLOR_MAGENTA;
    cout << "Welcome to the Anagrams Game!\n";
    cout << ANSI_COLOR_RESET;

    int level;
    cout << "Choose the difficulty level : "<<endl;
    cout << "1. Easy (40 seconds)"<<endl;
    cout<<"2. Medium (30 seconds)"<<endl;
    cout<<"3. Hard (20 seconds)"<<endl;
    cout << "Enter your choice (1-3) : ";
    cin >> level;

    int timeLimit;

    // Set time limit based on difficulty level
    switch (level)
    {
        case 1:
            timeLimit = 40; // Time limit for easy level: 20 seconds
            break;
        case 2:
            timeLimit = 30; // Time limit for medium level: 30 seconds
            break;
        case 3:
            timeLimit = 20; // Time limit for hard level: 40 seconds
            break;
        default:
            cout << "Invalid choice. Exiting.\n";
            return 1;
    }

    int randomIndex = rand() % words.size(); // Generate a random index for the word
    string targetWord = words[randomIndex]; // Select a random word from the array
    string scrambledWord = scrambleWord(targetWord); // Scramble the letters of the word

    cout << ANSI_COLOR_LIGHT_BLUE;
    cout << "Unscramble the letters to form a valid word.\n";
    cout << ANSI_COLOR_RESET;

    cout << ANSI_COLOR_LIGHT_YELLOW;
    cout << "Scrambled word: " << scrambledWord << endl;
    cout << ANSI_COLOR_RESET;

    string guess;
    cout << "Enter your guess: ";

    auto startTime = high_resolution_clock::now(); // Start timing

    cin >> guess;

    auto stopTime = high_resolution_clock::now(); // Stop timing
    auto duration = duration_cast<seconds>(stopTime - startTime).count(); // Calculate duration in seconds

    // Convert both the guess and targetWord to lowercase for case-insensitive comparison
    transform(guess.begin(), guess.end(), guess.begin(), ::tolower);
    transform(targetWord.begin(), targetWord.end(), targetWord.begin(), ::tolower);

    if (guess == targetWord && duration <= timeLimit)
    {
        cout << ANSI_COLOR_LIGHT_GREEN;
        cout << "Congratulations! You unscrambled the word correctly within the time limit.\n";
        cout << ANSI_COLOR_RESET;
    }
    else if (guess == targetWord)
    {
        cout << ANSI_COLOR_LIGHT_RED;
        cout << "Sorry, time's up!" << endl;
        cout << ANSI_COLOR_RESET;
        cout << "The correct word is: " << targetWord << endl;
    }
    else
    {
        cout << ANSI_COLOR_LIGHT_RED;
        cout << "Sorry, that's incorrect." << endl;
        cout << ANSI_COLOR_RESET;
        cout << "The correct word is: " << targetWord << endl;
    }
    printBlankLine();
    printBlankLine();
    system("pause");
    system("cls");
	//Thank You
	cout<<"\n\n";
    cout<<"\n\x1B[31;96m                              88888888888 888    888        d8888 888b    888 888    d8P   .d8888b.       8888888888 .d88888b.  8888888b.\x1B[0m  ";
    cout<<"\n\x1B[31;96m                                  888     888    888       d88888 8888b   888 888   d8P   d88P  Y88b      888       d88P\" \"Y88b 888   Y88b\x1B[0m ";
    cout<<"\n\x1B[31;96m                                  888     888    888      d88P888 88888b  888 888  d8P    Y88b.           888       888     888 888    888\x1B[0m ";
    cout<<"\n\x1B[31;96m                                  888     8888888888     d88P 888 888Y88b 888 888d88K      \"Y888b.        8888888   888     888 888   d88P\x1B[0m ";
    cout<<"\n\x1B[31;96m                                  888     888    888    d88P  888 888 Y88b888 8888888b        \"Y88b.      888       888     888 8888888P\"\x1B[0m  ";
    cout<<"\n\x1B[31;96m                                  888     888    888   d88P   888 888  Y88888 888  Y88b         \"888      888       888     888 888 T88b\x1B[0m   ";
    cout<<"\n\x1B[31;96m                                  888     888    888  d8888888888 888   Y8888 888   Y88b  Y88b  d88P      888       Y88b. .d88P 888  T88b\x1B[0m  ";
    cout<<"\n\x1B[31;96m                                  888     888    888 d88P     888 888    Y888 888    Y88b  \"Y8888P\"       888        \"Y88888P\"  888   T88b\x1B[0m ";
    cout<<"\n\n\n\n";
    cout<<"\n\x1B[31;96m                                              8888888b.  888             d8888 Y88b   d88P 8888888 888b    888  .d8888b.  888\x1B[0m              ";
    cout<<"\n\x1B[31;96m                                              888   Y88b 888            d88888  Y88b d88P    888   8888b   888 d88P  Y88b 888\x1B[0m              ";
    cout<<"\n\x1B[31;96m                                              888    888 888           d88P888   Y88o88P     888   88888b  888 888    888 888\x1B[0m              ";
    cout<<"\n\x1B[31;96m                                              888   d88P 888          d88P 888    Y888P      888   888Y88b 888 888        888\x1B[0m              ";
    cout<<"\n\x1B[31;96m                                              8888888P\"  888         d88P  888     888       888   888 Y88b888 888  88888 888\x1B[0m              ";
    cout<<"\n\x1B[31;96m                                              888        888        d88P   888     888       888   888  Y88888 888    888 Y8P\x1B[0m              ";
    cout<<"\n\x1B[31;96m                                              888        888       d8888888888     888       888   888   Y8888 Y88b  d88P  \"\x1B[0m               ";
    cout<<"\n\x1B[31;96m                                              888        88888888 d88P     888     888     8888888 888    Y888  \"Y8888P88 888\x1B[0m              ";
    cout<<"\n\n\n\n";
    }
    return 0;
}
