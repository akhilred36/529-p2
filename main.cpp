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
//#include <stdlib.h>
#include "tree.h"
#include "pythonpp.h"


using namespace std;

class DecisionTreeClassifier{

};


int main(){
    //Initialize vector of string vectors for the dataframe
    vector<vector<string>> data; 
    data = read_csv("car_evaluation.csv");
    pair<vector<string>, vector<vector<string>>> seperatedData = seperateHeader(data);
    vector<string> columns = seperatedData.first;
    vector<vector<string>> dataPoints = seperatedData.second;
    pair<vector<vector<string>>, vector<string>> attr_targets = seperateTargets(seperatedData.second, 6); 
    vector<vector<string>> shuffled = shuffleDataFrame(seperatedData.second);
    //printDataFrame(shuffled);
    pair<vector<vector<string>>, vector<vector<string>>> data_split = train_test_split(shuffled, 0.8);
    cout << "Length of Train data: ";
    int trainSize = data_split.first.size();
    cout << trainSize << endl;
    cout << "Length of Test data: ";
    int testSize = data_split.second.size();
    cout << testSize << endl;
    vector<string> unique_attr = getUniqueAttributes(data_split.first, 1);
    vector<vector<vector<string>>> sub_datasets = attribute_based_split(data_split.first, 1, unique_attr);
    for(int i=0; i<sub_datasets.size(); i++){
        cout << "Dataset " << i << " Length: " << (int) sub_datasets.at(i).size() << endl;
    }
    // double gainGini = getGain(data_split.first, "misclassificationError", 4, 6);
    // vector<vector<vector<string>>> attribute_split_datasets = attribute_based_filter(data_split.first, 0);
    // p("Length of subsets: ");
    // pln((int) attribute_split_datasets.size());
    int index = getMaxGainIndex(data_split.first, "entropy", 6);
    cout << "Index: " << index << endl;
    vector<pair<string, vector<vector<string>>>> all_attr_splits = attribute_based_split_labelled_all(data_split.first, 0);
    cout << all_attr_splits.at(3).first << endl;
    // bool chiTest = chiSquaredTest(data_split.first, 0, 0.95, 6);
    // string result;
    // if(chiTest){
    //     result = "pass";
    // }
    // else result = "fail";
    // cout << "Chi Squared Test: " << result << endl;
    return 0;
}