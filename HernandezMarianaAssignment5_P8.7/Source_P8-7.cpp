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
	for (int i = 0; i < static_cast<int>(upperWord.size()); i++) {
		unsigned char uc = static_cast<unsigned char>(upperWord[i]);
		upperWord[i] = static_cast<char>(toupper(uc));
	}
	return upperWord;
}

// Remove duplicates, keep order
string duplicateOrder(const string& no_double_word) {
	string no_doubles;
	for (int i = 0; i < static_cast<int>(no_double_word.size()); i++) { // loop to see each character from keyword
		char c = no_double_word[i];
		bool found = false;

		// check if we already added this character
		for (int j = 0; j < static_cast<int>(no_doubles.size()); j++) {
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
	return no_doubles; // returns the word without duplicate letters. BALLON to BALON
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
		for (int j = 0; j < static_cast<int>(uniqueKey.size()); j++) {  // <-- add static_cast<int>
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
				outputFile.put(letter);                              // fallback (shouldnt happen)
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

int main(int argc, char* argv[]) {
	if (argc < 5) {                                                // Need at least 5 args: prog, -e/-d (encrpyt or decrypt), -k (keyword), input, output
		cerr << "Usage: crypt (-e|-d) -k<KEY> <input> <output>\n"; // Print usage if not enough args
		return 1;                                                  // Exit with error
	}

	bool doEncrypt = (string(argv[1]) == "-e");                    // Check if first flag is -e
	bool doDecrypt = (string(argv[1]) == "-d");                    // Check if first flag is -d

	if (!doEncrypt && !doDecrypt) {                                // If neither encrypt nor decrypt chosen
		cerr << "Error: must specify -e or -d\n";                  // Show error
		return 1;                                                  // Exit
	}

	string key;                                                    // To hold the cipher keyword
	string kArg = argv[2];                                         // The second argument should be -k... 
	if (kArg.rfind("-k", 0) == 0) {                                // Does it start with "-k"?
		if (kArg.size() > 2) {                                     // Case: -kFEATHER (keyword attached)
			key = kArg.substr(2);                                  // Extract everything after -k
		}
		else if (argc > 3) {                                     // Case: -k FEATHER (keyword separate)
			key = argv[3];                                         // Grab the next argument as keyword
			for (int i = 3; i < argc - 1; i++) argv[i] = argv[i + 1]; // Shift arguments left so input/output stay at end
			argc--;                                                // Reduce count since we consumed one arg
		}
	}

	if (key.empty()) {                                             // If no keyword was found
		cerr << "Error: missing keyword (-k)\n";                   // Show error
		return 1;                                                  // Exit
	}

	if (argc < 5) {                                                // After adjusting args, still need input/output
		cerr << "Error: missing input/output file\n";              // Show error
		return 1;                                                  // Exit
	}

	string inPath = argv[3];                                      // Input file name
	string outPath = argv[4];                                      // Output file name
	string cipher = buildCipherAlphabet(key);                     // Build cipher alphabet using keyword

	if (doEncrypt) {                                               // If encrypt mode
		encryptFile(inPath, outPath, cipher);                      // Encrypt input to output
		cout << "Encrypted " << inPath << " -> " << outPath << "\n"; // Confirm to user
	}
	else {                                                       // Else decrypt mode
		decryptFile(inPath, outPath, cipher);                      // Decrypt input to output
		cout << "Decrypted " << inPath << " -> " << outPath << "\n"; // Confirm to user
	}
	return 0;                                                      // Program success
}