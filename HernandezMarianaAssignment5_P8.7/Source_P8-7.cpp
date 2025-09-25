/* ---------------------------------------------------------------------------
** I attest that this following code represents my own work and is subject to
** the plagiarism policy found in the course syllabus.
**
** Class: 	CSC 242
** Assignment: 	Assignment 5: Comprehensive Program Challenge
** File: 	P8_7.cpp
** Description: Write a program that encrypts a file using a monoalphabetic cipher and decrypts it back to the original text.
**
** Author: 	Mariana Hernandez, Damien Roseberry, and Karlos Grajeda
** Date: 	2025-09-23
** -------------------------------------------------------------------------*/

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cctype> // to be able to use toupper built in function

using namespace std;

// Mariana Hernandez function to build cipher alphabet with keyword
// Make a string uppercase
string upperCaseString(const string& keyword) {
	string upperWord = keyword;
	for (size_t i = 0; i < upperWord.size(); i++) {
		unsigned char uc = static_cast<unsigned char>(upperWord[i]);
		upperWord[i] = static_cast<char>(toupper(uc));
	}
	return upperWord;
}

// Remove duplicates, keep order
string duplicateOrder(const string& no_double_word) {
	string no_doubles;
	for (int i = 0; i < no_double_word.size(); i++) {	//loop to see each character from keyword
		char c = no_double_word[i];
		bool found = false;

		// check if we already added this character
		for (int j = 0; j < no_doubles.size(); j++) {
			if (no_doubles[j] == c) {
				found = true;
				break;
			}
		}

		// if not found, add it
		if (!found) {
			no_doubles.push_back(c);
		}
	}
	return no_doubles; // returns the word without duplicate letters. BALLON -> BALON
}

string buildCipherAlphabet(const string& keyword) {
	if (keyword.empty()) return "";

	string uppercase_keyword = upperCaseString(keyword); // calling uppercase function 

	string uniqueKey = duplicateOrder(uppercase_keyword); // remove duplicates and keep in order

	string remaining;
	for (int i = 25; i >= 0; i--) {   // loop backwards through alphabet
		char ch = 'A' + i;            // convert index to letter
		bool found = false;

		// check if ch is already in uniqueKey
		for (int j = 0; j < uniqueKey.size(); j++) {
			if (uniqueKey[j] == ch) {
				found = true;
				break;
			}
		}

		if (!found) {
			remaining.push_back(ch); // if letter not found add it.
		}
	}

	// Combine keyword letters with remaining letters
	string cipherAlphabet = uniqueKey + remaining;

	return cipherAlphabet;
}

// Damien Roseberry encrypt a file using the cipher alphabet
void encryptFile(const string& inputPath, const string& outputPath, const string& cipherAlphabet) {
	string normalAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";            // plain alphabet

	if (cipherAlphabet.size() != 26) {                               // sanity check
		cout << "Error: cipher alphabet must be 26 characters.\n";
		return;
	}

	ifstream inputFile(inputPath, ios::binary);                      // open input
	ofstream outputFile(outputPath, ios::binary);                    // open output
	if (!inputFile || !outputFile) {                                 // check files
		cout << "Error: could not open input or output file.\n";
		return;
	}

	char letter;
	while (inputFile.get(letter)) {                                  // read per char
		unsigned char uc = static_cast<unsigned char>(letter);       // cctype-safe
		if (isalpha(uc)) {                                           // only letters
			bool isLower = islower(uc);                              // remember case
			char upperLetter = static_cast<char>(toupper(uc));       // A..Z
			size_t pos = normalAlphabet.find(upperLetter);           // index in A..Z

			if (pos != string::npos) {                               // guard (should be true)
				char newChar = cipherAlphabet[pos];                  // map via cipher
				if (isLower) newChar = static_cast<char>(tolower(static_cast<unsigned char>(newChar)));
				outputFile.put(newChar);                             // write mapped
			}
			else {
				outputFile.put(letter);                              // fallback (shouldn’t happen)
			}
		}
		else {
			outputFile.put(letter);                                  // copy non-letters
		}
		if (!outputFile) {                                           // write error
			cout << "Error: write failed.\n";
			return;
		}
	}
}

// Karlos Grajeda decrypt a file using the cipher alphabet
void decryptFile(const string& inputPath, const string& outputPath, const string& cipherAlphabet) { //reads encrypted and writes out decrptyed version
	string normalAlphabet = "ABCDEFGHIJKLMNOPQRSTUVWXYZ";

	if (cipherAlphabet.size() != 26) {
		cout << "Error: cipher alphabet must be 26 characters.\n"; //ensures cipher is 26 letters of alphabet if not it fails
		return;
	}

	ifstream inputFile(inputPath, ios::binary); //opens encryped input file 
	ofstream outputFile(outputPath, ios::binary); // opens decrypted output file
	if (!inputFile || !outputFile) {				// ensures files open correctly
		cout << "Error: could not open input or output file.\n";
		return;
	}

	char letter;
	while (inputFile.get(letter)) {							//reads characters
		unsigned char uc = static_cast<unsigned char>(letter);
		if (isalpha(uc)) {									//only decrypts letters
			bool isLower = islower(uc);						//checks if originals are lowercase
			char upperLetter = static_cast<char>(toupper(uc)); //works them in uppercase

			size_t pos = cipherAlphabet.find(upperLetter); // search in cipher alphabet
			if (pos != string::npos) {						//if found
				char newChar = normalAlphabet[pos]; // map back to plain alphabet
				if (isLower) newChar = static_cast<char>(tolower(static_cast<unsigned char>(newChar)));
				outputFile.put(newChar);				//writes decrypt characters
			}
			else {
				outputFile.put(letter);				//writes as is
			}
		}
		else {
			outputFile.put(letter); // copy non-letters as is
		}

		if (!outputFile) {
			cout << "Error: write failed.\n"; // fails out 
			return;
		}
	}
}

int main() {
	string keyword;
	cout << "Enter keyword: ";
	if (!getline(cin, keyword) || keyword.empty()) {
		cerr << "Error: no keyword supplied.\n";
		return 1;
	}

	cout << "You entered: " << keyword << endl;
	
	string cipher = buildCipherAlphabet(keyword);

	cout << "Cipher keyword: " << cipher << endl; 

	encryptFile("plain.txt", "encrypted.txt", cipher);               // run encrypt
	cout << "Encryption complete. See encrypted.txt\n";

	decryptFile("encrypted.txt", "decrypted.txt", cipher); //runs decrypt, puts .txt in folder
	cout << "Decryption complete. See decrypted.txt\n";

	return 0;
}