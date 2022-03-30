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
        int numItr; // number of iterations 
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

        MatrixXd createTestX(vector<vector<int>> data){
            MatrixXd result((int) data.size(), (int) data.at(0).size() + 1);

            for(int i=0; i< data.size(); i++){
                result(i, 0) = 1;
                for(int j=1; j<data.at(i).size(); j++){
                    result(i, j) = data.at(i).at(j);
                }
            }

            return result;
        }

        void Exp(MatrixXd& matrix){
            for(int i=0; i<k; i++){
                for(int j=0; j<m; j++){
                    matrix(i, j) = exp(matrix(i, j));
                }
            }
        }

    public:
        // Hyperparams still missing
        logisticRegression(string trainFile, string vocab_file, string labels_file, double lr, double pt, int ni){
            // Hyperparams
            learningRate = lr; //Learning rate
            penaltyTerm = pt; //Penalty term
            numItr = ni;

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
            cout << "Applying exp to probmatrix" << endl;
            Exp(probMatrix);
            cout << "Done applying exp to probmatrix" << endl;
        }

        void train() {
            int currItr = 0;
            while (currItr < numItr) {
                W = W + learningRate * (((delta - probMatrix) * X) - (penaltyTerm * W));
                currItr = currItr + 1;
            }
        }

        int predict(MatrixXd features) {
           MatrixXd results = W * features.transpose();   // k x 1 

            int maxIndex = 0;
            double maxValue = -std::numeric_limits<double>::infinity();

            for (int i = 0; i < k; i++) {
                if (maxValue < results(i, 0)) {
                    maxIndex = i;
                    maxValue = results(i, 0);
                }
            }
            return maxIndex + 1;
        }

        void testModel(string file, bool produceSubmissionFile) {
            chrono::steady_clock::time_point begin;
            chrono::steady_clock::time_point end;
            chrono::steady_clock::time_point begin1;
            chrono::steady_clock::time_point end1;
            begin = chrono::steady_clock::now();
            vector<vector<int>> data = read_csv_int(file);
            end = chrono::steady_clock::now();
            std::cout << "Time to read file = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;
            begin = chrono::steady_clock::now();
            if (produceSubmissionFile) {
                data = seperateTargets(data, 0).first;
                MatrixXd testMatrix = createTestX(data);  // convert to eigen matrix

                // Crete submission file
                ofstream submission;
                submission.open("submission.csv");
                submission << "id,class" << endl;
                begin1 = chrono::steady_clock::now();
                for (int i = 0; i < data.size(); i++) {
                    submission << 12001 + i << "," << predict(testMatrix.row(i)) << endl;
                }
                end1 = chrono::steady_clock::now();
                std::cout << "Time to predict = " << std::chrono::duration_cast<std::chrono::milliseconds>(end1 - begin1).count() << "[ms]" << std::endl;
                submission.close();
            } else {
                data = seperateTargets(data, 0).first;
                vector<int> Y = seperateTargets(data, data.at(0).size() - 1).second;
                data = seperateTargets(data, data.at(0).size() - 1).first;

                MatrixXd testMatrix = createTestX(data);  // convert to eigen matrix

                // Crete file to rec
                ofstream record;
                record.open("last_run_info.txt");

                double correct = 0.0;
                double total = 0.0;
                int prediction;

                for (int i = 0; i < Y.size(); i++) {
                    prediction = predict(testMatrix.row(i));
                    if (prediction == Y.at(i)) {
                        correct = correct + 1.0;
                        cout << prediction << " :)" << endl;
                    } else {
                        cout << prediction << " X" << endl;
                    }
                    total = total + 1.0;
                }

                record << "Total: " << total << endl << "Correct: " << correct << endl << "Accuracy: " << (correct/total) * 100 << "%" << endl;
                record.close();
            }
            end = chrono::steady_clock::now();
            std::cout << "Total time to predict classes = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;     
        }
};

int runLR(int argc, char** argv){
    logisticRegression lr(argv[2], argv[3], argv[4], stod(argv[5]), stod(argv[6]), stoi(argv[7]));
    cout << "Train start" << endl;
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    lr.train();
    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    std::cout << "Time to train model = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;
    
    // lr.testModel("../testing.csv", true);

    lr.testModel("customTest.csv", false);
    
    return 0;
}