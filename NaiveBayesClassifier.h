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
#include <stdlib.h>
#include <unordered_map>
#include "pythonpp.h"


using namespace std;


class NaiveBayes {

    public:
        // Probabilities for each class in the training set
        unordered_map<string, float> classProbabilities;
        NaiveBayes() {

        }
        
    private:
        
};