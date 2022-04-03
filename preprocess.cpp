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
#include<math.h>
#include<chrono>
#include<array>
#include "pythonpp.h"


using namespace std;

int main(int argc, char * argv[]){

    if(argc < 5){
        cerr << "Usage: " << argv[0] << " <trainFile.csv> <vocabulary.txt> <groupLabels.txt> <trainSplitRatio>" << endl;
        return 0;
    }

    cout << "Reading " << argv[1] << " ...." << endl;
    vector<vector<int>> data_initial;
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    data_initial = read_csv_int((string) argv[1]);
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    std::cout << "Time to read file = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;

    cout << "Preprocessing data ...." << endl;
    vector<vector<int>> shuffledData = shuffleDataFrame(data_initial);
    pair<vector<vector<int>>, vector<vector<int>>> train_test = train_test_split(shuffledData, atof(argv[4]));

    vector<vector<int>> data = train_test.first;
    vector<vector<int>> test_data = train_test.second;
    data = seperateTargets(data, 0).first; 
    write_csv(test_data, "customTest.csv");

    vector<string> vocab;
    vocab = read_lines(argv[2]);

    vector<string> label_vocab;
    label_vocab = read_lines(argv[3]);

    //Delta Matrix declaration
    vector<vector<int>> deltaMatrix;

    //X matrix declaration
    vector<vector<int>> dataMatrix;

    // Constants
    const int number_of_classes = label_vocab.size();
    const int number_of_unique_words = vocab.size(); 

    // File to store Delta matrix
    ofstream deltaMatrixFile;
    deltaMatrixFile.open("deltaMatrix.mtx");
    for(int i=0; i<number_of_classes; i++){
        vector<int> temp(data.size(), 0);
        deltaMatrix.push_back(temp);
    }

    //File to store Data matrix
    ofstream dataMatrixFile;
    dataMatrixFile.open("dataMatrix.mtx");

    // File to store vector containing total word counts per class
    ofstream rawCountFile;
    rawCountFile.open("rawCount.vec");
    vector<int> rawCount(number_of_classes, 0);

    // File to store matrix of word to count frequencies
    ofstream wordToClassCountFile;
    wordToClassCountFile.open("wordToClassCount.mtx");
    vector<vector<int>> wordToClassCount;

    for(int i=0; i<number_of_classes; i++){
        vector<int> temp(number_of_unique_words, 0);
        wordToClassCount.push_back(temp);
    }

    // File to store vector containing total representation for each class
    ofstream classRepresentationFile;
    classRepresentationFile.open("classRepresentation.vec");
    vector<int> classRepresentation(number_of_classes, 0);

    int _class;

    // Gather the data 
    for (int i = 0; i < data.size(); i++) {
        _class = data.at(i).at((int) data.at(i).size() - 1);
        classRepresentation.at(_class - 1) += 1;
        for (int j = 0; j < data.at(i).size() - 1; j++) {
            wordToClassCount.at(_class - 1).at(j) += data.at(i).at(j);
            rawCount.at(_class - 1) += data.at(i).at(j);
        }
        deltaMatrix.at(_class - 1).at(i) = 1;
    }

    // Write To File
    writeIntVectorToFile(rawCount, rawCountFile);
    writeIntVectorToFile(classRepresentation, classRepresentationFile);
    writeIntMatrixToFile(wordToClassCount, wordToClassCountFile);
    writeIntMatrixToFile(deltaMatrix, deltaMatrixFile);
    writeIntMatrixToFile(data, dataMatrixFile);

    // Close Files
    rawCountFile.close();
    wordToClassCountFile.close();
    classRepresentationFile.close();
    deltaMatrixFile.close();
    dataMatrixFile.close();

    // //Write log probability matrix to a file
    // vector<vector<double>> logProbabilityMatrix;

    // for(int i=0; i<wordToClassCount.size(); i++){
    //     vector<double> temp(wordToClassCount.at(i).size(), 0);
    //     logProbabilityMatrix.push_back(temp); 
    // }   

    // for(int i=0; i<wordToClassCount.size(); i++){
    //     vector<double> temp;
    //     for(int j=0; j<wordToClassCount.at(i).size(); j++){
    //         double prob = (double) (wordToClassCount.at(i).at(j) + 1)/ (double) rawCount.at(i);
    //         double logProb = log2(prob);
    //         temp.push_back(logProb);
    //     }
    //     logProbabilityMatrix.push_back(temp);
    // }

    // ofstream logProbMatrixFile;
    // logProbMatrixFile.open("logProbMatrix.mtx");
    // writeDoubleMatrixToFile(logProbabilityMatrix, logProbMatrixFile);
    // logProbMatrixFile.close();

    return 0;
}