//=================================================================================================================================//
//                                        ADSA Assigment 3 - Hash Table with linear probing                                        //            
//                                                      Written by A1716063                                                        //
//                                                             2020                                                                //
//=================================================================================================================================//

#include <iostream>
#include <string>
#include <sstream>
#include <ctype.h>

#define TABLE_SIZE 26           //Size of the hash table

using namespace std;

//Struct used to store all the data for one element of the hashtable, contains a value and the status of the element
struct hashTableElement {
    string value;                                   // Stores the data of the element
    int status;                                     // 0=neverused, 1=occupied, 2=tombstone
};


//Function used to calculate the hash value of a given word
int hashFunction(string key) {
    return (tolower(key.back()) - 97);              // Aligns ascii number of the last letter in the key to begin at a=0
}


//Search the hash table for a given word (key), either returns the index of the word if found, otherwise returns -1
int searchHashValue(hashTableElement * hashTable, string key) {
    int hash = hashFunction(key);                   // Calculate the hash for the given key
    
    while (hashTable[hash].status != 0) {           // Start at the expected index value and search each consecutive index after it that has been used
        if (hashTable[hash].value == key) {
            return hash;                            // Returns index of found word
        } else {
            hash = (hash + 1) % TABLE_SIZE;         // Increment hash and loop back to zero if at the end of the array
        }
    }
    return -1;                                      // Hash table does not contain the key                  
}


//Insert a new value into its place in the hash table, if a collision occurs perform linear probing until a space is found
void insertHashValue(hashTableElement * hashTable, string key) {
    int hash = hashFunction(key);
    
    if (searchHashValue(hashTable, key) == -1) {    // First check if hash table already contains the key
        while(hashTable[hash].status == 1) {        // Locates the first available index suitable for the key
            hash = (hash + 1) % TABLE_SIZE;
        }
        hashTable[hash] = {key, 1};                 // Inserts the key at index given by hash and sets its status to occupied
    }
}


//Used to delete a hash value from the hash table
void deleteHashValue(hashTableElement * hashTable, string key) {
    int hash = searchHashValue(hashTable, key);     // Sets hash to index of the key in the hash table
    
    if (hash >= 0) {                                
        hashTable[hash].status = 2;                  // If the key is in the hash table, sets its status to tombstone
    }
}


//Main function
int main() {

    string input, output, command, key;
    char action;
    stringstream inputstream;
    getline(cin, input);                            // Get a line of input from the user
    inputstream << input;

    hashTableElement hashTable[TABLE_SIZE];         // Initialisese an array of hashTableElements
    for (int i = 0; i < TABLE_SIZE; i++) {
        hashTable[i] = {"", 0};                     // Initialises each element as an empty string and "never used" status
    }

    while (!inputstream.eof()) {
        inputstream >> command;                     // Places modification move into a seperate string
        action = command[0];                        // Seperates the action from the word
        key = command.substr(1);

        if (action == 'A') {                        // Insert word into hash table
            insertHashValue(hashTable, key);
        } 
        else if (action == 'D') {                   // Delete word from hash table
            deleteHashValue(hashTable, key);
        }
    }

    for (int i = 0; i < TABLE_SIZE; i++) {          // Add all the contents of the hash table into a single string
        if (hashTable[i].status == 1) {
            output += hashTable[i].value + " ";
        }
    }
    cout << output.substr(0, output.size()-1) << endl;   // Print the output
}