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
#include "logisticRegressionClassifier.h"

using namespace std;


int main(int argc, char** argv){
    if(strcmp(argv[1], "nb") == 0){
        return runNB(argc, argv);
    }
    else if(strcmp(argv[1], "lr") == 0){
        return runLR(argc, argv);
    }
    else{
        cerr << "Invalid classfier. Options are 'lr' or 'nb'" << endl;
    }    
}