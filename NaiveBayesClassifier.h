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

        NaiveBayes(string file, string vocab_file, string labels_file, double b) {
            countMatrix = read_csv_int(file);

            // Load labels and vocab from files
            vocab = read_lines(vocab_file);
            label_vocab = read_lines(labels_file);

            if (b > 0) {
                beta = b;
                alpha = 1 + b;
                cout << "Alpha: " << alpha << endl;
            } else {
                beta = (1.0/(double) vocab.size());
                alpha = 1 + beta;
                cout << "Alpha: " << alpha << endl;
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
                // data = seperateTargets(data, data.at(0).size()).first;

                // Crete submission file
                ofstream submission;
                submission.open("submission.csv");
                submission << "id,class" << endl;
                begin1 = chrono::steady_clock::now();
                for (int i = 0; i < data.size(); i++) {
                    submission << 12001 + i << "," << predict(data.at(i)) << endl;
                }
                end1 = chrono::steady_clock::now();
                std::cout << "Time to predict = " << std::chrono::duration_cast<std::chrono::milliseconds>(end1 - begin1).count() << "[ms]" << std::endl;
                submission.close();
            } else {
                data = seperateTargets(data, 0).first;
                vector<int> Y = seperateTargets(data, data.at(0).size() - 1).second;
                data = seperateTargets(data, data.at(0).size() - 1).first;

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
            end = chrono::steady_clock::now();
            std::cout << "Total time to predict classes = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;     
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
                    if(probMatrix.at(i).at(j) <= 0){
                        cout << "prob is: " << probMatrix.at(i).at(j) << endl;
                    }
                    probMatrix.at(i).at(j) = log2(probMatrix.at(i).at(j));
                }
            }
            cout << "Length of outer vector: " << probMatrix.size() << endl;
            cout << "Length of inner vector: " << probMatrix.at(0).size() << endl;
            ofstream file;
            file.open("probMatrix.mtx");
            writeDoubleMatrixToFile(probMatrix, file);
            file.close();
        }

        int predict(vector<int> features) {
            int maxIndex = 0;
            double maxVal = 0;

            for (int i = 0; i < features.size(); i++) {
                if (countMatrix.at(0).at(i) > 0) {
                    maxVal = maxVal + (countMatrix.at(0).at(i)  * probMatrix.at(0).at(i));
                }
            }
            
            double currVal;
            for (int i = 1; i < classRepresentation.size(); i++) {
                currVal = classProbabilities[i];

                for (int j = 0; j < features.size(); j++) {
                    if (features.at(j) > 0) {
                        currVal = currVal + (((double) features.at(j))  * probMatrix.at(i).at(j));
                    }
                }
                if (currVal > maxVal) {
                    maxVal = currVal;
                    maxIndex = i;
                }
            }
            return maxIndex + 1;
        }

        
};

int runNB(int argc, char** argv) {
    if(argc < 7){
        cerr << "Usage: " << argv[0] << " <countMatrix.mtx> <vocab.txt> <labels.txt> <testFile.csv> <betaValue>" << endl;
        return 0;
    }
    chrono::steady_clock::time_point begin = chrono::steady_clock::now();
    NaiveBayes test(argv[2], argv[3], argv[4], atof(argv[6]));

    chrono::steady_clock::time_point end = chrono::steady_clock::now();
    std::cout << "Time to train model = " << std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count() << "[ms]" << std::endl;

    begin = chrono::steady_clock::now();

    test.testModel(argv[5], true);

    end = chrono::steady_clock::now();
    std::cout << "Total time for reading and predicting = " << std::chrono::duration_cast<std::chrono::seconds>(end - begin).count() << "[s]" << std::endl;
    return 0;
}