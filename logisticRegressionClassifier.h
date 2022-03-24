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
#include <chrono>
#include <stdlib.h>
#include <math.h>  
#include <unordered_map>
#include "pythonpp.h"
#include <Eigen/Dense>

using namespace Eigen;
using namespace std;

class logisticRegression{
    private:
        int m; //Number of examples
        int n; //Number of attributes for each example
        int k; //Number of classes
        double learningRate; //Learning rate
        double penaltyTerm; //Penalty term
        MatrixXd delta; //Equation 29 Mitchell --> binary matrix that tells us which class each example belongs to
        MatrixXd X; //Feature matrix
        MatrixXd Y; //True Classification matrix
        MatrixXd W; //Weight matrix
        MatrixXd probMatrix; //Probability Matrix

        // // Matrix of word counts in a class
        vector<vector<int>> countMatrix;

        // Vector containing total representation for each class
        vector<int> classRepresentation;



    public:
        logisticRegression(string file, string vocab_file, string labels_file){
            countMatrix = read_csv_int(file);

            // Load labels and vocab from files
            n = (int) (read_lines(vocab_file)).size();
            k = (int) (read_lines(labels_file)).size();

            classRepresentation = read_vec_int("classRepresentation.vec");

            m = 0;   // Sum of class representations

            // Calculate number of documents
            for (int i : classRepresentation) {
                m = m + 1;
            }
        }

};

int runLR(int argc, char** argv){
    return 0;
}