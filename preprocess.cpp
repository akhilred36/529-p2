#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <random>
#include <vector>
#include <utility> // pair
#include <stdexcept> // runtime_error
#include <sstream> // stringstream
#include <time.h>
#include<chrono>
#include<array>
#include "pythonpp.h"


using namespace std;

void writeIntArrayToFile(array<int> arr, ofstream file) {
    for (int i = 0; i < arr.size(); i++) {
        if (i < arr.size() - 1) {
            file << arr[i] << ",";
        } else {
            file << arr[i];
        }
        
    }
}

void writeIntMatrixToFile(array<int> arr, ofstream file) {
    for (int i = 0; i < arr.size(); i++) {
        if (i < arr.size() - 1) {
            file << arr[i] << ",";
        } else {
            file << arr[i];
        }
        
    }
}

int main(){
    vector<vector<int>> data;
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    data = read_csv_int("../training.csv");
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    std::cout << "Time to read file = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;

    vector<string> vocab;
    vocab = read_lines("../vocabulary.txt");

    vector<string> label_vocab;
    label_vocab = read_lines("../newsgrouplabels.txt");

    // Constants
    const int NUMBER_OF_CLASSES = (int) label_vocab.size();
    const int NUMBER_OF_UNIQUE_WORDS = (int) vocab.size(); 


    // File to store vector containing total word counts per class
    ofstream rawCountFile;
    rawCountFile.open("rawCount.vec");
    array<int, NUMBER_OF_CLASSES> rawCount;
    rawCount.fill(0);

    // File to store matrix of word to count frequencies
    ofstream wordToClassCount;
    wordToClassCount.open("wordToClassCount.mtx");
    array<array<int, NUMBER_OF_UNIQUE_WORDS>, NUMBER_OF_CLASSES> wordToClassCount;
    
    for (int i = 0; i < NUMBER_OF_CLASSES; i++) {
        wordToClassCount[i].fill(0);
    }

    // File to store vector containing total representation for each class
    ofstream classRepresentationFile;
    classRepresentationFile.open("classRepresentation.vec");
    array<int, NUMBER_OF_CLASSES> classRepresentation;
    classRepresentation.fill(0);


    // Gathher the data 
    for (int i = 0; i < data.size(); i++) {
        int _class = data.at(i).at((int) data.at(i).size() - 1);
        classRepresentation[_class - 1] += 1;
        for (int j = 0; j < data.at(i).size(); i++) {
            wordToClassCount[_class - 1][j] += data.at(i).at(j);
            rawCount[_class - 1] += data.at(i).at(j);
        }
    }

    // Write To File
    writeIntArrayToFile(rawCount, rawCountFile);
    writeIntArrayToFile(classRepresentation, classRepresentationFile);


    // Close Files
    rawCountFile.close();
    wordToClassCount.close();
    classRepresentationFile.close();

    return 0;
}