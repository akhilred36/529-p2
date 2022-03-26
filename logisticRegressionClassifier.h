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
#include <Eigen/Core>
#include <unsupported/Eigen/MatrixFunctions>

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
        MatrixXd XT; //Transpose 
        MatrixXd Y; //True Classification matrix
        MatrixXd W; //Weight matrix
        MatrixXd probMatrix; //Probability Matrix

        // // Matrix of word counts in a class
        vector<vector<int>> countMatrix;

        // Vector containing total representation for each class
        vector<int> classRepresentation;

        void createXY(vector<vector<int>> data){
            cout << "start createXY" << endl;
            X.resize(m, n + 1);
            Y.resize(m, 1);
            cout << "Finish declaring X Y" << endl;
            for(int i=0; i< data.size(); i++){
                X(i, 0) = 1;
                Y(i, 0) = data.at(i).at(data.at(i).size() - 1);
                for(int j=1; j<data.at(i).size() - 1; j++){
                    X(i, j) = data.at(i).at(j);
                }
            }
            cout << "Done" << endl;
            return;
        }

        double Exp(double x){
            return std::exp(x);
        }

    public:
        logisticRegression(string trainFile, string vocab_file, string labels_file){
            // Load labels and vocab from files
            n = (int) (read_lines(vocab_file)).size();
            k = (int) (read_lines(labels_file)).size();

            classRepresentation = read_vec_int("classRepresentation.vec");

            m = 0;   // Sum of class representations

            // Calculate number of documents
            for (int i : classRepresentation) {
                m = m + i;
            }

            //Load in the delta matrix
            delta = dfToMatrixInt(read_csv_int("deltaMatrix.mtx"));

            cout << "Reading in " << trainFile << endl;
            vector<vector<int>> * data = read_csv_int_p(trainFile);
            cout << "Read in" << endl;
            createXY(* data);
            cout << "finished createXY" << endl;
            delete data; //Great memory management. Please give extra credit.
            cout << "Done deleting" << endl;
            //Initialize weight matrix
            W.resize(k, n+1);
            double f;
            double w;
            for(int i=0; i<k; i++){
                for(int j=0; j<n+1; j++){
                    f = (double) rand() / RAND_MAX;
                    w = 0 + f * (1 - 0);
                    W(i, j) = w;
                }
            }

            //Transpose X and multiply with W
            XT = X.transpose();
            probMatrix = W*XT;

            //Normalize probMatrix
            int numRows = probMatrix.rows();
            int numCols = probMatrix.cols();
            for(int i=0; i<numCols; i++){
                probMatrix(numRows - 1, i) = 1; //Fill last row with all 1s
            }
            VectorXd columnSums = probMatrix.colwise().sum();
            for(int i=0; i<numCols; i++){
                probMatrix.col(i) /= columnSums(i); //Normalize
            }
            cout << "Original prob matrix: " << endl << probMatrix << endl;
            probMatrix.exp();
            cout << "New prob matrix: " << endl << probMatrix << endl;
        }
};

int runLR(int argc, char** argv){
    logisticRegression lr(argv[2], argv[3], argv[4]);
    return 0;
}