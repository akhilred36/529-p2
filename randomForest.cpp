#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <utility> // pair
#include <stdexcept> // runtime_error
#include "randomForest.h"
#include "pythonpp.h"

Forest::Forest(vector<vector<string>> dataset, int target, int numBags, int minFeatureSize, string pruneMethod, string splitCriterion, double confidence){
    datasetIndices = bagFeaturesIndices(dataset, target, numBags, minFeatureSize);
    setSplitCriterion(splitCriterion);
    setPruneMethod(pruneMethod);
    datasets = bagFeatures(dataset, datasetIndices);
    for(int i=0; i<numBags; i++){
        trees.push_back(new Tree(datasets.at(i)));
        trees.at(i)->setSplitCriterion(splitCriterion);
        if(pruneMethod.compare("chiSquared") == 0){
            trees.at(i)->toggleChiSquared();
            trees.at(i)->setConfidence(confidence);
        }
    }
}

Forest::Forest(vector<vector<string>> dataset, int target, int numBags, int minFeatureSize){
    datasetIndices = bagFeaturesIndices(dataset, target, numBags, minFeatureSize);
    datasets = bagFeatures(dataset, datasetIndices);
    for(int i=0; i<numBags; i++){
        trees.push_back(new Tree(datasets.at(i)));
        trees.at(i)->setSplitCriterion("gini");
        trees.at(i)->toggleChiSquared();
        trees.at(i)->setConfidence(0.95);
    }
}

void Forest::train(){
    if((int) trees.size() == 0){
        cout << "Error. No trees initialized." << endl;
        return;
    }
    else{
        for(int i=0; i<trees.size(); i++){
            trees.at(i)->train();
        }
        return;
    }
}

string Forest::predict(vector<string> input){
    string result;
    vector<pair<string, int>> votes;
    for(int i=0; i<trees.size(); i++){
        vector<string> filteredFeatures;
        for(int j=0; j<datasetIndices.at(i).size() - 1; j++){ //Exclude target index
            filteredFeatures.push_back((string) input.at(datasetIndices.at(i).at(j)));
        }
        string prediction = trees.at(i)->predict(filteredFeatures);
        bool found = false;
        for(int j=0; j<votes.size(); j++){
            if(votes.at(j).first.compare(prediction) == 0){
                found = true;
                votes.at(j).second = votes.at(j).second + 1;
            }
        }
        if(!found){
            pair<string, int> tmp;
            tmp.first = prediction;
            tmp.second = 1;
            votes.push_back(tmp);
        }
    }
    int maxVotes = 0;
    for(int i=0; i<votes.size(); i++){
        if(votes.at(i).second > maxVotes){
            result = votes.at(i).first;
            maxVotes = votes.at(i).second;
        }
    }
    if(maxVotes == 0){
        cout << "Error. Max vote is 0";
        return "0";
    }
    return result;
}

void Forest::setSplitCriterion(string c){
    criterion = c;
}

void Forest::setPruneMethod(string m){
    pruneMethod = m;
}

Forest::~Forest(){
    for(int i=0; i<trees.size(); i++){
        delete trees.at(i);
    }
}

int main(int argc, char* argv[]){

    // if(argc < 6){
    //     cerr << "Usage: " << argv[0] << " <splitCriterion> <numBags> <minAttrs> <pruneMethod> <confidence>" << endl;
    //     return 0;
    // }
    // cout << "--------------------------------------------------------------------------------" << endl;
    // cout << "Using " << argv[1] << ", " << argv[2] << " bags, " << argv[3] << " min attrs, " << argv[4] << ", confidence " << argv[5] << endl;
    // //For known dataset testing : 
    // double splitPercent = 0.8;
    // vector<vector<string>> data = read_csv("train_refined.csv");

    // pair<vector<vector<string>>, vector<vector<string>>> train_test = train_test_split(shuffleDataFrame(seperateHeader(data).second),splitPercent);

    // vector<vector<string>> train = train_test.first;
    // vector<vector<string>> test = train_test.second;
    // int target = (int) train.at(0).size() - 1;
    // //(vector<vector<string>> dataset, int target, int numBags, int minFeatureSize, string pruneMethod, string splitCriterion, double confidence)
    // Forest rf = Forest(train, target, (int) atoi(argv[2]), (int) atoi(argv[3]), argv[4], argv[1], (double) atof(argv[5]));
    // rf.train();

    // pair<vector<vector<string>>, vector<string>> feature_label = seperateTargets(test, target);
    // vector<vector<string>> X = feature_label.first;
    // vector<string> Y = feature_label.second;
    // int total = 0;
    // int correct = 0;
    // string prediction;
    // for (int i = 0; i < (int) X.size(); i++) {
    //     prediction = rf.predict(X.at(i));
    //     if (prediction.compare(Y.at(i)) == 0) {
    //         correct = correct + 1;
    //     }

    //     total = total + 1;
    // }  

    // cout << "Acc.\n" << "------------------------------\n" << "Correct: " << correct << endl << "Total: " << total << endl << "Percent: " << (double) (100 * ((double) correct / (double) total)) << "%" << endl << endl;

    //For unknown prediction generation:
    if(argc < 8){
        cerr << "Usage: " << argv[0] << " <splitCriterion> <numBags> <minAttrs> <toggleChiSquared> <confidence> <trainDataset(csv)> <testDataset(csv)>" << endl;
        return 0;
    }
    cout << "--------------------------------------------------------------------------------" << endl;
    cout << "Using " << argv[1] << ", " << argv[2] << " bags, " << argv[3] << " min attrs, " << argv[4] << ", confidence " << argv[5] << endl;

    vector<vector<string>> data = read_csv(argv[6]);
    vector<vector<string>> train = shuffleDataFrame(seperateHeader(data).second);
    int target = (int) train.at(0).size() - 1;
    Forest rf = Forest(train, target, (int) atoi(argv[2]), (int) atoi(argv[3]), argv[4], argv[1], (double) atof(argv[5]));
    rf.train();

    vector<vector<string>> X = seperateTargets(seperateHeader(read_csv(argv[7])).second, 0).first;

    ofstream myfile;
    myfile.open ("predictions.csv");
    myfile << "id,class" << endl;
    for (int i = 0; i < (int) X.size(); i++) {
        string prediction = rf.predict(X.at(i)); 
        myfile << i+2001 << "," << prediction << endl;
    } 
    myfile.close();
    return 0;
}