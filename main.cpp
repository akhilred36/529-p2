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
#include "NaiveBayesClassifier.h"

using namespace std;


int main(int argc, char** argv){
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    double fd = atof(argv[3]); //"wordToClassCount.mtx"
    NaiveBayes test(argv[1], fd);

    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    std::cout << "Time to train model = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

    begin = chrono::steady_clock::now();

    test.testModel(argv[2], true);

    end = chrono::steady_clock::now();
    std::cout << "Total time for reading and predicting = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;
    return 0;
}