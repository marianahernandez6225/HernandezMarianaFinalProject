/* ---------------------------------------------------------------------------
** I attest that this following code represents my own work and is subject to
** the plagiarism policy found in the course syllabus.
**
** Class: 	CSC 242
** Assignment: 	Assignment 5: Comprehensive Program Challenge
** File: 	P8_2.cpp
** Description: Write a program that checks the spelling of all words in a file against a dictionary file.
**
** Author: 	Mariana Hernandez, Damien Roseberry, and Karlos Grajeda
** Date: 	2025-09-23
** -------------------------------------------------------------------------*/

#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <cctype> // to be able to use tolower built in function
#include <unordered_set>
#include <set>

using namespace std;

string checkUpper(const string& word) {
	string checkedWord;

	for (size_t i = 0; i < word.size(); i++) {
		unsigned char c = word[i];            // get each character
		if (isalpha(c)) {                     // only keep letters
			checkedWord.push_back(tolower(c));// convert to lowercase
		}
	}

	return checkedWord;
}

//Mariana Hernandez load dictionary file into memory
vector<string> loadDictionary(const string& path) {

	ifstream file_reader(path);		// Open dictionary file
	vector<string> words;		    // vector string named words
	string word;				    // place holder for each word

	while (file_reader >> word) {   // loop that keeps reading words until the file ends.
		words.push_back(word);	    // store each word into vector
	}

	file_reader.close();	        // close file

	return words;

}

// Damien Roseberry load words from document to be checked
vector<string> loadDocumentWords(const string& path) {

	ifstream file_reader(path);     // Open input file (the document we are checking)
	vector<string> documentWords;   // Vector to hold all words from the document
	string word;                    // Placeholder for each word

	while (file_reader >> word) {
		documentWords.push_back(word); // Store each word into vector
	}

	file_reader.close();            // Close file

	return documentWords;
}

// Karlos Grajeda compare document words with dictionary
void checkSpelling(const vector<string>& dict, const vector<string>& doc) {
	unordered_set<string> dictSet(dict.begin(), dict.end()); // coverts dictionary vector into a set  

	for (size_t i = 0; i < dict.size(); i++) {
		string cleanWord = checkUpper(dict[i]);
		if (!cleanWord.empty()) {
			dictSet.insert(cleanWord);
		}
	}
	set<string> misspellings; // store unique misspelled words

	for (size_t i = 0; i < doc.size(); i++) {
		string cleanWord = checkUpper(doc[i]);  // clean document word
		if (!cleanWord.empty() && dictSet.find(cleanWord) == dictSet.end()) {
			misspellings.insert(doc[i]);        // store original for display
		}
	}
	//for (const string& w : doc) {
	//	if (!w.empty() && dictSet.find(w) == dictSet.end()) { //checks if word is missing from dictionary
	//		misspellings.insert(w);
	//	}
	//}

	cout << "\nMisspelled words:\n"; //outputs misspelled words
	if (misspellings.empty()) {
		cout << "None found.\n"; //if no mispellings prints none found
	}
	else {
		for (const auto& word : misspellings) { //shows misspelled words 
			cout << word << endl;
		}
	}
}
// This main is the actual main function for Step 1 and Step 2 that will be turned in. Just need to uncomment it and add step 3.
/*int main() {
	// Step 1: Load dictionary words
	vector<string> dictionary = loadDictionary("dictionary.txt");
	cout << "Loaded " << dictionary.size() << " words from dictionary." << endl;

	// Step 2: Load document words
	vector<string> document = loadDocumentWords("input.txt");
	cout << "Loaded " << document.size() << " words from document." << endl;

	// Step 3: Placeholder for Person C
	cout << "Now pass dictionary and document to the comparison function (Person C)." << endl;

	return 0;
} */




// This is just a test function to demonstrate loading dictionary and document words is working.
// It shows the number of words loaded and the first few words from each list.
// This function can be removed when the final main function is implemented.

int main() {
	vector<string> dictionary = loadDictionary("dictionary.txt");
	cout << "Dictionary words loaded: " << dictionary.size() << "\n";

	// show first up to 5 dictionary words
	cout << "First 5 dictionary words: ";
	for (size_t i = 0; i < dictionary.size() && i < 5; ++i) {
		cout << dictionary[i] << (i + 1 < 5 && i + 1 < dictionary.size() ? ", " : "\n");
	}

	vector<string> document = loadDocumentWords("document.txt");
	cout << "Document words loaded: " << document.size() << "\n";

	// show first up to 5 document words
	cout << "First 5 document words: ";
	for (size_t i = 0; i < document.size() && i < 5; ++i) {
		cout << document[i] << (i + 1 < 5 && i + 1 < document.size() ? ", " : "\n");
	}
	// Compare and list misspellings
	checkSpelling(dictionary, document);
	//cout << "A+B test complete. Hand off to Karlos next.\n";
	return 0;
}