#define _CRT_SECURE_NO_WARNINGS
#include <iostream> 
#include<fstream>
#include<vector> 
#include <algorithm>
using namespace std;


//IMPORT THE LETTERS
void readLetters(char* array, int n) {
	ifstream read("alphabet.txt");

	for (int index = 0; index < n; index++) {
		read >> array[index];
	}
}

//RANDONIZE A WORD
int random(int i) {
	return rand() % i;
}

string randomWord(int level) {
	string fileName;
	string word;

	switch (level) {
	case 1:fileName = "beginner.txt"; break;
	case 2:fileName = "intermediate.txt"; break;
	case 3:fileName = "expert.txt"; break;
	default:fileName = "beginner.txt"; break;
	}

	ifstream file(fileName);
	srand(time(0));
	vector<string> words(istream_iterator<string>{file}, istream_iterator<string>{});

	random_shuffle(words.begin(), words.end());
	word = words[random(1)];

	return word;
}

//CHECK IF THE LETTER EXIST IN LETTER_USED > RETURN FALSE IF DON T EXIST
bool checkLetter(char letter, char* letters_used, int n) {
	for (int index = 0; index < n; index++) {
		if (letters_used[index] == letter) {
			return true;//exita deja
		}
	}
	return false;
}

//ADD THE LETTER FROM CONSOLE IN LETTERS_USED
void pushLetter(char letter,char* letters_used,int n) {
	for (int index = 0; index < n; index++) {
		if (letters_used[index] == '0') {//free position
			letters_used[index] = letter;
			break;
		}
	}
}

//DELETE THE LETTER FROM ALPHABET
 void deleteLetterFromAlphabet(int* n,char letter,char* array) {
	for (int j = 0; j < *n; j++) {
		if (letter == array[j]) {
			for (int k = j; k < *n; k++) {
				array[k] = array[k + 1];
			}
			(*n)--;
			break;
		}
	}
}

//THE FUNCTION TO READ A SINGLE LETTER
 void readLetter(int attemps, int* mistakes, char* letters, char* letters_used, int* sizeLetters, char* wordForGuessing_c, char* final) {

	char letterFromConsole;
	bool ok1 = false; //if the letter from console is in word
	bool ok2 = false; //if the letter from console is already in letters_used vector


	//READ LETTER FROM CONSOLE
	cout << "You inserted the letter ";
	cin >> letterFromConsole;
	letterFromConsole = toupper(letterFromConsole);

	for (int v = 0; v < *sizeLetters; v++) {
		if (letters_used[v] == letterFromConsole) {
			cout << "YOU ALREADY INSERTED THIS LETTER!!! " << endl;

			//INCREASE THE NUMBER OF MISTAKES
			(*mistakes)++;

			ok2 = true;
			break;
		}
	}

	if (!ok2) {
		for (int i = 0; i < strlen(wordForGuessing_c); i++) {

			//IF THE LETTER FROM CONSOLE... IS IN WORD FOR GUESSING
			if (letterFromConsole == wordForGuessing_c[i]) {
				final[i] = letterFromConsole;
				ok1 = true;

				if ((checkLetter(letterFromConsole, letters_used, *sizeLetters))==false) {
					pushLetter(letterFromConsole, letters_used, *sizeLetters);
					deleteLetterFromAlphabet(sizeLetters, letterFromConsole, letters);
				}
			}
		}

		if (ok1 == false) {

			if ((checkLetter(letterFromConsole, letters_used, *sizeLetters)) == false) {
				pushLetter(letterFromConsole, letters_used, *sizeLetters);
				deleteLetterFromAlphabet(sizeLetters, letterFromConsole, letters);
			}

			//INCREASE THE NUMBER OF MISTAKES
			(*mistakes)++;
		}
	}

	//INCREASE THE NUMBER OF MISTAKES

	cout << "You have " << attemps - (*mistakes) << " more attempts." << endl;

	cout << "Letters left: ";
	for (int index1 = 0; index1 < *sizeLetters; index1++) {
		cout << letters[index1] << " ";
	}
	cout << endl;

	cout << "Letters used: ";
	for (int index2 = 0; index2 < *sizeLetters; index2++) {
		if (letters_used[index2] != '0') {
			cout << letters_used[index2] << " ";
		}
	}
	cout << endl << endl;
}


//FUNCTIONS WITH MESSAGES
void winnerMessage(char* word) {
	cout << "\t\t:)" << endl;
	cout << "You guessed the word << " << word << " >> Congratulations!" << endl << endl;
	cout << "\t\t(:" << endl;
}

void loserMessage(char* word) {
	cout << "You have consumed your 4 attempts :(" << endl;
	cout << "The game is over! The word was " << word << endl;
}

void you_guessed_by_now(char* array, int size) {
	cout << "You guessed by now : ";
	for (int p = 0; p < size; p++) {
		cout << array[p];
	}
	cout << "\n__________________________________________________________________________________________________\n" ;
	cout << endl<<endl<<endl;
}


//THE GAME
void game(int level,char* letters, char* letters_used,int sizeLetters, char* wordForGuessing_c, int size, char* final) {

	char* a = nullptr; 	//i ve used a for previous version of the word 
	char* b = nullptr;  //i ve uded b for new version of the word

	int mistakes = 0;	//the mistakes made
	int attemps;		//the number of attemps

	switch (level) {
	case 1:attemps = 4; break;
	case 2:attemps = 5; break;
	case 3:attemps = 7; break;
	default:attemps = 4; break;
	}

	//while you shill have your 4 attemps, the game is available
	while (mistakes != attemps) {

		readLetter(attemps,&mistakes, letters, letters_used ,&sizeLetters, wordForGuessing_c, final);
		b = final;

		if (a == nullptr) {//for the first iteration
			a = new char[size];
			a = b;
		}
		else {//when something changes (a letter)
			for (int i = 0; i < size; i++) {
				if (a[i] != b[i] && b[i] != '#') {
					a[i] = b[i];//replace
				}
			}
		}

		//message
		you_guessed_by_now(a, size);

		if (mistakes == attemps) {
			loserMessage(wordForGuessing_c);
		}
		if (mistakes < attemps + 1) {
			if (strcmp(final, wordForGuessing_c) == 0) {
				winnerMessage(wordForGuessing_c);
				break;
			}
		}
	}
}

/////////////////////////////////////////////
void main() {

	//MAIN VARIABLES
	int nLetters=26;	//number of letters
	char* letters= new char[nLetters];	//array of char of letters
	char* letters_used= new char[nLetters];	//stl:vector with char elements > when we store the used letters
	memset(letters_used, '0', nLetters);

	string wordG_s; //word for guessing (string)
	char* wordG_c;  //word for guessing (char*)
	int size_c=0;		//number of letters in the word for guessing (char*)

	//AUXILIARY VARIABLES
	char hashtag = '#';
	char buffer[50];
	char answer;
	int level;

	//welcome section
	cout << "____________________________________________________________________________________________________\n";
	cout << "_________________________________________W E L C O M E __ T O ______________________________________\n";
	cout << "_____________________________________________H A N G M A N__________________________________________\n";
	cout << "________________________________________________G A M E ____________________________________________\n";

	cout << "\n____________________________________________________________________________________________________\n";
	cout << "DO YOU WANNA PLAY ?(Y/N)" << endl;
	cin >> answer;
	answer = toupper(answer);

	while (answer == 'Y') {

		cout << "\n_________________________________________________________________________________________________\n";
		cout << "|CHOOSE A LEVEL" << endl;
		cout << "| 1\t->\tbeginner" << endl;
		cout << "| 2\t->\tintermediate" << endl;
		cout << "| 3\t->\texpert" << endl;
		cout << "|____________________________________________________________________________________________________\n";

		cin >> level;

		//PREPARE THE ALPHABET
		readLetters(letters, nLetters);

		//CHOOSE A WORD
		wordG_s = randomWord(level);

		///
		///
		///
		///
		/// 
		//SHOW THE WORD
		//cout << endl << wordG_s << endl;
		///
		///
		/// 
		/// 
		/// 

		//CONVERSION STRING-CHAR*
		buffer[wordG_s.length() + 1];
		strcpy(buffer, wordG_s.c_str());
		wordG_c = new char[strlen(buffer) + 1];
		strcpy(wordG_c, buffer);

		size_c = strlen(wordG_c);

		//THE FINAL WORD FILLED WITH HASHTAG (#) .. IN SPACES WHERE WE HAVE TO GUESS THE LETTERS
		char* final = new char[wordG_s.length() + 1];
		strcpy(final, wordG_s.c_str());
		memset(final, '#', size_c);

		cout << endl << final << endl;

		//THIS FUNCTION IS THE ENTIRE GAME ..WHEN THE USER TRY TO GUESS THE LETTERS
		//AND IT S TWO WAYS TO FINIS THIS GAME : LOSING OR WINNING :)
		game(level,letters, letters_used,nLetters, wordG_c, size_c, final );

		cout << "\n____________________________________________________________________________________________________\n";
		cout << "DO YOU WANNA PLAY ?(Y/N)" << endl;

		cin >> answer;
		answer = toupper(answer);
	}

	if (answer == 'N') {
		cout << "\n____________________________________________________________________________________________________\n";
		cout << "IT S OKAY! BYEEEE" << endl;
	}
}