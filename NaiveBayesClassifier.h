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
#include <math.h>  
#include <unordered_map>
#include "pythonpp.h"


using namespace std;


class NaiveBayes {

    public:
        // Probabilities for each class in the training set
        unordered_map<int, double> classProbabilities;

        // Matrix of word counts in a class
        vector<vector<int>> countMatrix;

        // Matrix of word log probabilities in a class
        vector<vector<double>> probMatrix;

        // Alpha
        double alpha;

        // Beta smoothing factor
        double beta;

        // Number of documents
        int numberOfDcuments;

        // List of words
        vector<string> vocab;

        // List of classes
        vector<string> label_vocab;

        // Vector containing total word counts per class
        vector<int> rawCount;

        // Vector containing total representation for each class
        vector<int> classRepresentation;

        NaiveBayes(string file, double b) {
            countMatrix = read_csv_int(file);

            // Load labels and vocab from files
            vocab = read_lines("vocabulary.txt");
            label_vocab = read_lines("newsgrouplabels.txt");

            if (beta > 0) {
                beta = b;
                alpha = 1 + b;
            } else {
                beta = 1.0/vocab.size();
                alpha = 1 + b;
            }
            

            // Preprocess Probability matrix
            for(int i=0; i<countMatrix.size(); i++){
                vector<double> temp((int) countMatrix.at(i).size(), 0.0);
                probMatrix.push_back(temp);
            }

            // Load preprocessed data into model
            rawCount = read_vec_int("rawCount.vec");
            classRepresentation = read_vec_int("classRepresentation.vec");

            numberOfDcuments = 0;   // Sum of class representations

            // Calculate number of documents
            for (int i : classRepresentation) {
                numberOfDcuments = numberOfDcuments + 1;
            }

            fillClassProbabilities();
            fillProbabilityMatrix();
        }

        void testModel(string file, bool produceSubmissionFile) {
            cout << "nj" << endl;
            vector<vector<int>> data = seperateHeader(read_csv_int(file)).second;
            cout << "i9" << endl;
            if (produceSubmissionFile) {
                data = seperateTargets(data, 0).first;
                data = seperateTargets(data, data.at(0).size()).first;

                cout << "JIHK" << endl;

                // Crete submission file
                ofstream submission;
                submission.open("submission.csv");
                submission << "id,class" << endl;
                for (int i = 0; i < 1/*data.size()*/; i++) {
                    submission << 12001 + i << "," << predict(data.at(i)) << endl;
                }
                submission.close();
            } else {
                data = seperateTargets(data, 0).first;
                vector<int> Y = seperateTargets(data, data.at(0).size()).second;
                data = seperateTargets(data, data.at(0).size()).first;

                // Crete file to rec
                ofstream record;
                record.open("last_run_info.txt");

                double correct = 0.0;
                double total = 0.0;

                for (int i = 0; i < Y.size(); i++) {
                    if (predict(data.at(i)) == Y.at(i)) {
                        correct = correct + 1.0;
                    }
                    total = total + 1.0;
                }

                record << "Total: " << total << endl << "Correct: " << correct << endl << "Accuracy: " << (correct/total) * 100 << "%" << endl;
                record.close();
            }   
            
        }
        

    private:
        void fillClassProbabilities() {
            for (int i = 0; i < classRepresentation.size(); i++){
                classProbabilities[i] = log2(((double) classRepresentation.at(i) / (double) numberOfDcuments));
            }
        }

        void fillProbabilityMatrix() {
            // I assume i refers to the class index here
            for (int i = 0; i < countMatrix.size(); i++) {
                for (int j = 0; j < countMatrix.at(i).size(); j++) {
                    probMatrix.at(i).at(j) = (countMatrix.at(i).at(j) + (alpha - 1)) / ((double) rawCount.at(i) + ((alpha - 1) * vocab.size()));
                    probMatrix.at(i).at(j) = log2(probMatrix.at(i).at(j));
                }
            }
        }

        int predict(vector<int> features) {
            int maxIndex = 0;
            double maxVal = classProbabilities[0];

            for (int i = 0; i < features.size(); i++) {
                if (countMatrix.at(0).at(i) > 0) {
                    maxVal = maxVal + (countMatrix.at(0).at(i)  * probMatrix.at(0).at(i));
                }
            }
            
            double currVal;
            for (int i = 1; i < classRepresentation.size(); i++) {
                currVal = classProbabilities[i];

                for (int j = 0; j < features.size(); j++) {
                    if (countMatrix.at(i).at(j) > 0) {
                        maxVal = maxVal + (countMatrix.at(i).at(j)  * probMatrix.at(i).at(j));
                    }
                }

                if (currVal > maxVal) {
                    maxVal = currVal;
                    maxIndex = i;
                }
            }
            cout << "JJJJ" << endl;
            return maxIndex;
        }

        
};