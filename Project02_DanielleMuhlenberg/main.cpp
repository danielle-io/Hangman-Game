/******************************************************************
Danielle Muhlenberg 08/02/2017
Programming Project 2
The function of this program is to act as a hangman game
Input = The user inputs letters as guesses
Output = The board is updated, guesses and guess amount is
totaled, and the user finds out if they won or lost while they
are at 8 guesses, or have correctly guessed the word. They then
have the option to play again or exit the game.
******************************************************************/
#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <vector>
#include <ctime>
using namespace std;
void hangman_menu();
void initialize_board(string game_board[], int size);
void print_board(string board[], int size);
string file_read(vector<string> &words_used);
string output_initial_hangman(string word);
int take_guess(string word, string initial_hangman, string game_board[], int size);
string spaces(string guess, int length);
string lower(string guess, int length);
int full_word_guessed(string word, string words_guessed, string guess, int already, int length);
int digit_punct_check(string guess, int test, int length);
int single_letter_guessed(string guess, int already, string chars_guessed);
vector<int> guessing_placement(string word, string guess, int length, vector<int> locations);
void update_board(string board[], int size, int guess);
string hangman_update(string word, string initial_hangman);
void guesses_left(string guess, int guess_amount, string letters_guessed);
int repeat_prompt();

/******************************************************************
(1) HANGMAN MENU FUNCTION
Purpose = displays hangman board and takes guess + validates it
through the call of functions
Input = none
Output = none
******************************************************************/
void hangman_menu() {

	// Declare variables
	vector<string> words_used;
	int repeat_program = 0;

	// Run a do while as long as repeat program = 1 (meaning play again)
	do {

		// Declare variables
		string word;
		string guess = "";
		string all_guesses = "";
		int size = 16;
		
		// Declare the empty board
		string game_board[16];

		// Call INITIALIZE BOARD FUNCTION to output the board
		initialize_board(game_board, size);

		// Call the PRINT BOARD FUNCTION
		print_board(game_board, size);

		// Call FILE READ FUNCTION
		word = file_read(words_used);

		// Update vector
		words_used.push_back(word);

		// Call OUTPUT INITIAL HANGMAN FUNCTION to output a _ for each letter in the word
		string initial_hangman = output_initial_hangman(word);

		// Call TAKE GUESS FUNCTION to take in the user's guess
		repeat_program = take_guess(word, initial_hangman, game_board, size);
	} while (repeat_program == 1);
}
	
/******************************************************************
(2) INITIALIZE BOARD FUNCTION
Purpose = set the board up
Input = string board[], int size
Output = none
******************************************************************/
void initialize_board(string board[], int size) {
	board[0] = "\n\t\tHangman Game\n";
	board[1] = "\t___________";
	board[2] = "\t|\t  |";
	board[3] = "\t|";
	board[4] = "\t|";
	board[5] = "\t|";
	board[6] = "\t|";
	board[7] = "\t|";
	board[8] = "\t|";
	board[9] = "\t|";
	board[10] = "\t|";
	board[11] = "\t|";
	board[12] = "\t|";
	board[13] = "\t|";
	board[14] = "\t|";
	board[15] = "  ______|_________\n";
}
	
/******************************************************************
(3) PRINT BOARD FUNCTION
Purpose = displays hangman board
Input = string board[], int size
Output = none
******************************************************************/
// Print the hanging amount
void print_board(string board[], int size) {
	for (int i = 0; i < size; i++) {
		cout << board[i] << endl;
	}
}

/******************************************************************
(4) FILE READ FUNCTION
Purpose = read text from file
Input = vector<string> words_used
Output = string word
******************************************************************/
string file_read(vector<string> &words_used) {
	// Declare variables
	string word;
	bool unused = false;

	// Run a do while as long as unused = false, meaning there are unused words left
	do {

		// Read input from file into vector and get a random line
		vector<string> words;
		ifstream file("words.txt");
		string one_line;
		while (getline(file, one_line)) words.push_back(one_line);

		// Set word as the random line
		word = words[rand() % words.size()];

		// Add to vector to not repeat words
		if (find(words_used.begin(), words_used.end(), word) == words_used.end()) {
			unused = true;
		}

		// When all words have been used, output a message and end the game
		if (words_used.size() == 20) {
			cout << "\nAll words have been used. Thanks for playing!\n" << endl;
			words_used.clear();
			unused = true;
			system("pause");
			repeat_prompt();
		}
	} while (unused == false);

	// Return the chosen word from the document
	return word;
}

/******************************************************************
(5) OUTPUT INITIAL HANGMAN FUNCTION
Purpose = output a _ for each letter in the word
Input = string word
Output = string initial hangman
******************************************************************/
string output_initial_hangman(string word) {

	//Declare variables
	int length;
	string initial_hangman = "";

	// Set length to = word.length()
	length = word.length();

	// Print Word: followed by a _ for each letter in word
	cout << "\nWord: ";
	for (int count = 0; count < length; count++) {
		initial_hangman += "_";
		cout << "_ ";
	}
	return initial_hangman;
}

/******************************************************************
(6) TAKE GUESS FUNCTION
Purpose = take in the users guess & call functions to validate it
Input = string word, string initial_hangman, string game_board[], int siz
Output = int repeat_program
******************************************************************/
int take_guess(string word, string initial_hangman, string game_board[], int size) {

	// Declare variables
	int guess_amount = 0;
	string letters_guessed = "";
	int repeat_program = 0;
	string guess;
	int already = 0;
	int test = 0;
	string chars_guessed = "";
	string words_guessed = "";
	int length;
	int placement = -1;
	vector<int> locations;

	// Outer loop runs while the word wasn't guessed & guesses are under 8
	do {

		// Inner loop runs while already & test != 0, meaning guess isn't a repeat & no invalid input
		do {

			// If var test is above 0, let user know they can't input digits or punct
			if (test != 0) {
				cout << "\nYour guess can only consist of letters. Please try again.";
			}
			if (already != 0) {
				cout << "\nYou already guessed " << guess << ". Please try again.";
			}

			// Set var test (which will determine if there's a # or punctuation) back to 0
			test = 0;

			// Set var already back to 0 (to later determine if guess has already been guessed)
			already = 0;

			// Set the vector back to empty
			locations.clear();

			// Ask user to enter their guess
			cout << "\n\nPlease enter your guess: ";
			getline(cin, guess);

			// Set an int length = to guess.length()
			length = guess.length();

			// Call SPACES FUNCTION to check for white space, re-assign guess to guess sans spaces
			guess = spaces(guess, length);

			// Re-assign length to the new length of guess (it may have changed after spaces)
			length = guess.length();

			// Call the LOWERCASE FUNCTION to convert letters to lowercase letters
			guess = lower(guess, length);

			// Call FULL WORD GUESSED FUNCTION to see if the word was guessed, and set already to 1 if so
			if (length > 1) {
				already = full_word_guessed(word, words_guessed, guess, already, length);
			}

			// Call the DIGIT & PUNCT CHECK FUNCTION and set test to the returned value
			test = digit_punct_check(guess, test, length);

			
			// Call the SINGLE LETTERS GUESSED FUNCTION and set already to the returned value
			if (length == 1) {
				already = single_letter_guessed(guess, already, chars_guessed);
			}
		} while (test != 0 || already != 0);

		// Call GUESSING PLACEMENT FUNCTION and respond if placement is an index higher than -1
		locations = guessing_placement(word, guess, length, locations);
		
		// Set vector_size to = locations.size
		int vector_size = locations.size();

		// If placement came up above -1, we need to replace initial hangman's _ with the guess
		if (!locations.empty()) {

			for (int i = 0; i < vector_size; i++) {
				int x = locations.at(i);
				initial_hangman[x] = word[x];
			}
			placement = 0;
		}
		else {
			placement = -1;
		}
		// If the length of the guess is greater than 1, the word must match it letter for letter
		if (length > 1) {
			if (guess == word) {
				placement = -2;
				initial_hangman = word;
			}
		}

		// Clear the vector
		locations.clear();

		// If the placement is -1, then the guess was wrong and we need to add to guess amount and update the board
		if (placement == -1) {
			guess_amount++;

			// Call the UPDATE BOARD FUNCTION only if the user does not guess a correct letter
			update_board(game_board, size, guess_amount);
		}

		// Call PRINT BOARD FUNCTION to update board based on user's guess
		print_board(game_board, size);

		// Call HANGMAN UPDATE FUNCTION to ouput initial hangman ( _ _ _ ) with updated guessed letters
		initial_hangman = hangman_update(word, initial_hangman);

		// Break guesses into letter and word strings so we can search for duplicate guesses
		if (length == 1) {
			chars_guessed += guess;
		}
		if (length > 1) {
			words_guessed += guess;
		}

		// Create a string of what has been guessed thus far
		letters_guessed += guess + "  ";

		// Call the GUESSES LEFT FUNCTION to let user know how many  they have used and how many are left
		guesses_left(guess, guess_amount, letters_guessed);

		// Check if the user got the word right, and call repeat program function if so
		if (initial_hangman.find('_') == -1) {
			cout << "\nCongratulations! You won the game for guessing " << "\"" << word << "\"" << "!";

			// Call REPEAT PROGRAM FUNCTION
			repeat_program =repeat_prompt();
			return repeat_program;
		}

	} while (guess != word && guess_amount < 8);

	// Respond to user getting the answer wrong
	cout << "\n\nGame over. The correct word was " << "\"" << word << "\"" << "!";

	// Call REPEAT PROGRAM FUNCTION
	repeat_program = repeat_prompt();
	return repeat_program;
}

/******************************************************************
(7) VALIDATE SPACES FUNCTION
Purpose = Check to make sure the word has no spaces before or after
the intended input
Input = string guess, int length
Output = string guess
******************************************************************/
string spaces(string guess, int length) {
	
	// Check if the word starts with any spaces
	int count = 0;
	while (guess[count] == ' ' && count < length) {
		count++; 
		if (guess[count] != ' ' && count < length) {
			guess = guess.substr(count, length - count);
			break;
		}
	}

	// Set int temp to = -1 if there are no more spaces at the end of guess
	int temp = guess.find(' ');

	// Set length to the new length of guess
	length = guess.length();

	// Set count to last letter
	if (temp != -1) {
		count = length - 1;
		while (guess[count] == ' ' && count < length) {
			count--;
			if (count < 0) {
				break;
			}
		}
		if (count < 0) {
			count++;
		}
		guess = guess.substr(0, count + 1);
	}
	return guess;
}

/******************************************************************
(8) LOWERCASE FUNCTION
Purpose = Change guess letters to lowercase letters
Input = string guess, int length
Output = string lower_guess
******************************************************************/
string lower(string guess, int length) {

	// Declare variables
	string lower_guess;

	// Loop through guess and convert letters to lowercase
	for (int count = 0; count < length; count++) {
		lower_guess += tolower(guess[count]);
	}
	return lower_guess;
}

/******************************************************************
(9) FULL WORDS GUESSED FUNCTION
Purpose = if the user enters a full word, check if they already
guessed that word
Input = string word, string words_guessed, string guess, int already,
int length
Output = int
******************************************************************/
int full_word_guessed(string word, string words_guessed, string guess, int already, int length) {

	// Set found_word to evaluate to > -1 if the guess is inside the guessed words string
	int found_word = words_guessed.find(guess);

	// If more than 1 char was entered, and the word has been guessed, var already = 1
	if (length > 1) {
		if (found_word != -1) {
			already = 1;
		}
	}
	return already;
}

/******************************************************************
(10) DIGIT & PUNCTUATION CHECK FUNCTION
Purpose = loop through guess to check for #s or punctuation
Input = string guess, int test, int length
Output = int test
******************************************************************/
int digit_punct_check(string guess, int test, int length) {
	
	// Check if the user entered nothing
	if (length == 0) {
		return ++test;
	}

	// Set temp = to guess.find(' ')
	int temp = guess.find(" ");

	// Account for a space only input
	if (temp != -1) {
		return ++test;
	}

	// Loop through the guess and see if there are numbers
	for (int count = 0; count < length; count++) {
		if (isdigit(guess[count]) || ispunct(guess[count])) {
			test++;
		}
	}
	return test;
}

/******************************************************************
(11) SINGLE LETTERS GUESSED FUNCTION
Purpose = if user enters a single letter, check if they already
guessed it
Input = string guess, int already, string chars_guessed
Output = int already
******************************************************************/
int single_letter_guessed(string guess, int already, string chars_guessed) {

	// Declare variables
	int chars_here;

	// Check through single chars guessed, if new guess is there, chars_here will be above -1
	chars_here = chars_guessed.find(guess);

	// If the guess isn't found in chars_here, we add one to already
	if (chars_here > -1) {
		already++;
	}
	return already;
}

/******************************************************************
(12) GUESSING PLACEMENT FUNCTION
Purpose = figure out the index of the found char in the word
Input = string word, string guess, int length, vector<int> locations
Output = int placement
******************************************************************/
vector<int> guessing_placement(string word, string guess, int length, vector<int> locations) {

	// Declare variables
	int placement = -1;
	char a = guess[0];

	// Make a variable for word's length
	int word_length = word.length();

	// Loop through the word and see if the guess is there, store index in vector if so
	if (length == 1) {

		for (int i = 0; i < word_length; i++) {
			if (word[i] == a) {
				locations.push_back(i);
			}
		}
	}
	return locations;
}

/******************************************************************
(13) UPDATE BOARD FUNCTION
Purpose = updates the board with the hanging man
Input = string board[], int size, int guess_number
Output = none
******************************************************************/
void update_board(string board[], int size, int guess_number) {

	// Use a switch statement to output the hanging man if guess # changed
	switch (guess_number) {
	case 1:
		board[4] += "\t(  ";
		break;
	case 2:
		board[4] += " )";
		break;
	case 3:
		board[5] += "\t  |";
		break;
	case 4:
		board[6] += "      --";
		break;
	case 5:
		board[6] += "   --";
		break;
	case 6:
		board[7] += "\t  |";
		break;
	case 7:
		board[8] += "        /";
		break;
	case 8:
		board[8] += " \\";
		break;
	default:
		break;
	}
}

/******************************************************************
(14) HANGMAN UPDATE
Purpose = ouput the initial hangman (the _ _ _ for the word) with
updated guessed letters
Input = string word, string initial_hangman
Output = string initial_hangman
******************************************************************/
string hangman_update(string word, string initial_hangman) {

	// Declare variables
	int length;

	// Set length = to word.length()
	length = word.length();

	// Print Word: followed by letters found and _'s for unknown letters
	cout << "\nWord: ";
	for (int count = 0; count < length; count++) {
		cout << initial_hangman[count];
		cout << " ";
	}

	// End two lines after initial hangman output
	cout << endl << endl;
	return initial_hangman;
}

/******************************************************************
(15) GUESSES LEFT FUNCTION
Purpose = output the last guess, what guess # it was, and amount
of guesses left, to the user
Input = string guess, int guess_amount, string letters_guessed
Output = none
******************************************************************/
void guesses_left(string guess, int guess_amount, string letters_guessed) {
	cout << "Guess " << guess << " is guess " << guess_amount << "/" << "8. " 
		<< 8 - guess_amount << " tries left!\n\n";
	cout << "Guesses so far: " << letters_guessed << endl;
}

/******************************************************************
(16) REPEAT PROGRAM FUNCTION
Purpose = keeps the program from closing until user chooses to do so
Input = none
Output = int rp
******************************************************************/
int repeat_prompt() {

	// Declare variables
	string repeat_program = "";
	int repeat_program_status = 0;
	int rp;
	int nrp;

	// Ask user if they would like to repeat the program
	do {
		cout << "\n\nWould you like to play again? (Enter 1 for yes or 2 for no): ";
		getline(cin, repeat_program);

		// Set rp to = 0 if the number 1 is found in repeat_program
		rp = repeat_program.find('1');
		nrp = repeat_program.find('2');
		if (rp == 0) {
			rp = 1;
			return rp;
		}
		else if (nrp == 0) {
			cout << "\nProgram exiting. Goodbye! \n\n";
			system("pause");
		}
	} while (rp != 0 && nrp != 0);
	return rp;
}

// Main
int main() {

	// Keep random numbers new every time
	srand((unsigned int)time(NULL));

	// Call the hangman menu function
	hangman_menu();
	return 0;
}