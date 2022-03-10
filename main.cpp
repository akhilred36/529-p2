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
//#include <stdlib.h>
#include "pythonpp.h"


using namespace std;


int main(){
    // vector<vector<int>> data;
    // chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    // data = read_csv_int("../training.csv");
    // chrono::steady_clock::time_point end = chrono::steady_clock::now();
    // std::cout << "Time to read file = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;

    // vector<string> vocab;
    // unordered_map<string, int> vocab_dict;
    // vocab = read_lines("../vocabulary.txt");
    // vocab_dict = make_dict(vocab);
    // cout << "The index of " << vocab[932] << " is " << vocab_dict[vocab[932]] << endl;

    // vector<string> label_vocab;
    // unordered_map<string, int> label_vocab_dict;
    // label_vocab = read_lines("../newsgrouplabels.txt");
    // label_vocab_dict = make_dict(label_vocab, 1);
    // cout << "The index of " << label_vocab[15] << " is " << label_vocab_dict[label_vocab[15]] << endl;

    //Read in logProbMatrix
    vector<vector<double>> logProbMatrix;
    logProbMatrix = read_csv_double("logProbMatrix.mtx");
    return 0;
}