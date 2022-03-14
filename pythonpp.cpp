#include "pythonpp.h"
#include <iostream>
#include <string>
#include <fstream>
#include <algorithm>
#include <random>
#include <vector>
#include <utility>
#include <stdexcept>
#include <sstream>
#include <time.h>
#include <stdlib.h>
#include <set>
#include <math.h>
#include <unordered_map>
#include "chisqr.h"
#include "gamma.h"

using namespace std;

vector<vector<string>> read_csv(string filename){ //From https://www.gormanalysis.com/blog/reading-and-writing-csv-files-with-cpp/
    vector<vector<string>> result;
    ifstream myFile(filename); // Create an input filestream
    if(!myFile.is_open()) throw runtime_error("Could not open file"); // Make sure the file is open
    string line, attribute; // Helper vars
    if(myFile.good())
    {
        while(getline(myFile, line)){ // Extract the first line in the file
            vector<string> row;
            stringstream ss(line); // Create a stringstream from line
            while(getline(ss, attribute, ',')){ // Extract each column name
                row.push_back(attribute); // Push attributes to row
            }
            result.push_back(row); //Push rows to the result vector
        }
    }
    myFile.close(); // Close file
    return result;
}

//Read a csv file and interpret all values as integers. Much more memory efficient than reading in strings.
vector<vector<int>> read_csv_int(string filename){
    vector<vector<int>> result;
    ifstream myFile(filename); // Create an input filestream
    if(!myFile.is_open()) throw runtime_error("Could not open file"); // Make sure the file is open
    string line, attribute; // Helper vars
    if(myFile.good())
    {
        while(getline(myFile, line)){ // Extract the first line in the file
            vector<int> row;
            stringstream ss(line); // Create a stringstream from line
            while(getline(ss, attribute, ',')){ // Extract each column name
                row.push_back(stoi(attribute)); // Push attributes to row
            }
            result.push_back(row); //Push rows to the result vector
        }
    }
    myFile.close(); // Close file
    return result;
}

//Read a csv file and interpret all values as integers. Much more memory efficient than reading in strings.
vector<vector<double>> read_csv_double(string filename){
    vector<vector<double>> result;
    ifstream myFile(filename); // Create an input filestream
    if(!myFile.is_open()) throw runtime_error("Could not open file"); // Make sure the file is open
    string line, attribute; // Helper vars
    if(myFile.good())
    {
        while(getline(myFile, line)){ // Extract the first line in the file
            vector<double> row;
            stringstream ss(line); // Create a stringstream from line
            while(getline(ss, attribute, ',')){ // Extract each column name
                row.push_back(stod(attribute)); // Push attributes to row
            }
            result.push_back(row); //Push rows to the result vector
        }
    }
    myFile.close(); // Close file
    return result;
}

//read lines from any file
vector<string> read_lines(string filename){
    vector<string> result;
    ifstream myFile(filename); // Create an input filestream
    if(!myFile.is_open()) throw runtime_error("Could not open file"); // Make sure the file is open
    string line, attribute; // Helper vars
    if(myFile.good())
    {
        while(getline(myFile, line)){ // Extract the first line in the file
            result.push_back(line); //Push rows to the result vector
        }
    }
    myFile.close(); // Close file
    return result;
}

vector<int> read_vec_int(string filename){
    vector<int> result;
    result = read_csv_int(filename).at(0);
    return result;
}

vector<double> read_vec_double(string filename){
    vector<double> result;
    result = read_csv_double(filename).at(0);
    return result;
}   

//Write an integer vector to a file
void writeIntVectorToFile(vector<int> arr, ofstream& file) {
    for (int i = 0; i < arr.size(); i++) {
        if (i < arr.size() - 1) {
            file << arr[i] << ",";
        } else {
            file << arr[i];
        }
        
    }
}

//Write an integer matrix to a file
void writeIntMatrixToFile(vector<vector<int>> arr, ofstream& file) {
    for(vector<int> item : arr){
        writeIntVectorToFile(item, file);
        file << endl;
    }
}

//Write an Double vector to a file
void writeDoubleVectorToFile(vector<double> arr, ofstream& file) {
    for (int i = 0; i < arr.size(); i++) {
        if (i < arr.size() - 1) {
            file << arr[i] << ",";
        } else {
            file << arr[i];
        }
        
    }
}

//Write an double matrix to a file
void writeDoubleMatrixToFile(vector<vector<double>> arr, ofstream& file) {
    for(vector<double> item : arr){
        writeDoubleVectorToFile(item, file);
        file << endl;
    }
}

//Write a 2d vector to a csv file
void write_csv(vector<vector<int>> input, string filename){
    ofstream file1;
    file1.open("filename");
    for(int i=0; i<input.size(); i++){
        for(int j=0; j<input.at(i).size(); j++){
            if(j == input.at(i).size() - 1){
                file1 << input.at(i).at(j) << endl;
            }
            else{
                file1 << input.at(i).at(j) << ",";
            }
        }
    }
}

//Write a 2d vector to a csv file
void write_csv(vector<vector<double>> input, string filename){
    ofstream file1;
    file1.open("filename");
    for(int i=0; i<input.size(); i++){
        for(int j=0; j<input.at(i).size(); j++){
            if(j == input.at(i).size() - 1){
                file1 << input.at(i).at(j) << endl;
            }
            else{
                file1 << input.at(i).at(j) << ",";
            }
        }
    }
}

//Write a 2d vector to a csv file
void write_csv(vector<vector<string>> input, string filename){
    ofstream file1;
    file1.open("filename");
    for(int i=0; i<input.size(); i++){
        for(int j=0; j<input.at(i).size(); j++){
            if(j == input.at(i).size() - 1){
                file1 << input.at(i).at(j) << endl;
            }
            else{
                file1 << input.at(i).at(j) << ",";
            }
        }
    }
}

//Return dictionary that maps the input vector of strings to indices based on their order
unordered_map<string, int> make_dict(vector<string> vocab){
    unordered_map<string, int> result;
    for(int i=0; i<vocab.size(); i++){
        result[vocab[i]] = i;
    }
    return result;
}

//Return dictionary that maps the input vector of strings to indices based on their order with an added offset
unordered_map<string, int> make_dict(vector<string> vocab, int offset){
    unordered_map<string, int> result;
    for(int i=0; i<vocab.size(); i++){
        result[vocab[i]] = i+offset;
    }
    return result;
}

//Print 2 dimensional dataframe
void printDataFrame(vector<vector<string>> data){
    for(int i=0; i<data.size(); i++){
        for(int j=0; j<data.at(i).size(); j++){
            cout << data.at(i).at(j) << " ";
        }
    cout << "\n";
    }
    return; 
}

//Print 1 dimensional row/column
void printColumns(vector<string> data){
    for(int i=0; i<data.size(); i++){
        cout << data.at(i) << " ";
    }
    cout << "\n";
}

//Separate attributes from target and return as a pair
pair<vector<vector<string>>, vector<string>> seperateTargets(vector<vector<string>> data, int targetIndex){
    pair<vector<vector<string>>, vector<string>> result;
    vector<string> targets;
    for(int i=0; i<data.size(); i++){
        vector<string> row;
        for(int j=0; j<data.at(i).size(); j++){
            if(j == targetIndex){
                targets.push_back(data.at(i).at(j));
            }
            else{
                row.push_back(data.at(i).at(j));
            }
        }
        result.first.push_back(row);
    }
    result.second = targets;
    return result;
}

//Separate attributes from target and return as a pair
pair<vector<vector<int>>, vector<int>> seperateTargets(vector<vector<int>> data, int targetIndex){
    pair<vector<vector<int>>, vector<int>> result;
    vector<int> targets;
    for(int i=0; i<data.size(); i++){
        vector<int> row;
        for(int j=0; j<data.at(i).size(); j++){
            if(j == targetIndex){
                targets.push_back(data.at(i).at(j));
            }
            else{
                row.push_back(data.at(i).at(j));
            }
        }
        result.first.push_back(row);
    }
    result.second = targets;
    return result;
}

//Separate column headers and return as a pair
pair<vector<string>, vector<vector<string>>> seperateHeader(vector<vector<string>> data){
    pair<vector<string>, vector<vector<string>>> result;
    for(int i=0; i<data.size(); i++){
        vector<string> row;
        for(int j=0; j<data.at(i).size(); j++){
            row.push_back(data.at(i).at(j));
        }
        if(i == 0){
            result.first = row;
        }
        else{
            result.second.push_back(row);
        }
    }
    return result;
}

//Separate column headers and return as a pair
pair<vector<int>, vector<vector<int>>> seperateHeader(vector<vector<int>> data){
    pair<vector<int>, vector<vector<int>>> result;
    for(int i=0; i<data.size(); i++){
        vector<int> row;
        for(int j=0; j<data.at(i).size(); j++){
            row.push_back(data.at(i).at(j));
        }
        if(i == 0){
            result.first = row;
        }
        else{
            result.second.push_back(row);
        }
    }
    return result;
}

//Shuffle dataframe
vector<vector<string>> shuffleDataFrame(vector<vector<string>> data){
    auto rng = default_random_engine {};
    shuffle(data.begin(), data.end(), rng);
    return data;
}

//Split dataframe into train and test based on trainRatio(between 0 and 1)
pair<vector<vector<string>>, vector<vector<string>>> train_test_split(vector<vector<string>> data, float trainRatio){
    pair<vector<vector<string>>, vector<vector<string>>> result;
    int lastTrainIdx = (int) (trainRatio * (float) data.size());
    for(int i=0; i<data.size(); i++){
        if(i < lastTrainIdx){
            result.first.push_back(data.at(i));
        }
        else{
            result.second.push_back(data.at(i));
        }
    }
    return result;
}

// get unique values of all attribute choices 
vector<string> getUniqueAttributes(vector<vector<string>> data, int attribute){
    vector<string> result;
    set<string> attr_set;
    for(int i=0; i<data.size(); i++){
        attr_set.insert(data.at(i).at(attribute));
    }
    result.assign(attr_set.begin(), attr_set.end());
    return result;
}

// cut out attribute column, and return k subsets based on k choices for said attribute
vector<vector<vector<string>>> attribute_based_split(vector<vector<string>> data, int attribute, vector<string> values){
    vector<vector<vector<string>>> result;
    for(int i=0; i<values.size(); i++){
        result.push_back(vector<vector<string>> {});
    }
    for(int i=0; i<data.size(); i++){
        for(int j=0; j<values.size(); j++){
            if(data.at(i).at(attribute).compare(values.at(j)) == 0){
                result.at(j).push_back(data.at(i));
            }
        }
    }
    return result;
}

//Returns singular pair of subdataset and attribute label based on the value passed in
pair<string, vector<vector<string>>> attribute_based_split_labelled(vector<vector<string>> data, int attribute, string value){
    pair<string, vector<vector<string>>> result;
    result.first = value;
    result.second = vector<vector<string>>{};
    for(int i=0; i<data.size(); i++){
        if(data.at(i).at(attribute).compare(value) == 0){
            result.second.push_back(data.at(i));
        }
    }
    return result;
}

//Returns all pairs of subdatasets based on all possible values of the attribute passed in.
vector<pair<string, vector<vector<string>>>> attribute_based_split_labelled_all(vector<vector<string>> data, int attribute){
    vector<pair<string, vector<vector<string>>>> result;
    vector<string> values = getUniqueAttributes(data, attribute);
    vector<vector<vector<string>>> all_subdatasets = attribute_based_split(data, attribute, values);
    for(int i=0; i<values.size(); i++){
        result.push_back(pair<string, vector<vector<string>>>{});
        result.at(i).first = values.at(i);
    }
    for(int i=0; i<all_subdatasets.size(); i++){
        for(int j=0; j<values.size(); j++){
            if(all_subdatasets.at(i).at(0).at(attribute).compare(values.at(j)) == 0){
                result.at(j).second = all_subdatasets.at(i);
            }
        }
    }
    return result;
}

// return sub-datasets, each containing homogeneous values for the chosen attribute
vector<vector<vector<string>>> attribute_based_filter(vector<vector<string>> data, int attribute){
    vector<vector<vector<string>>> result;
    vector<string> uniqueValues = getUniqueAttributes(data, attribute);
    for(int i=0; i<uniqueValues.size(); i++){
        result.push_back(vector<vector<string>> {});
    }
    for(int i=0; i<data.size(); i++){
        for(int j=0; j<uniqueValues.size(); j++){
            if((data.at(i).at(attribute)).compare(uniqueValues[j]) == 0){
                result.at(j).push_back(data.at(i));
            }
        }
    }
    return result;
}

// Get the misclassification error for a dataset, given the attribute's column id, and target's column id
double getMisclassificationError(vector<vector<string>> data, int target){
    vector<string> unq_targets = getUniqueAttributes(data, target);
    vector<double> counts;
    vector<double> probabilities;
    int total = data.size();
    for(int i=0; i<unq_targets.size(); i++){
        counts.push_back(0);
    }
    for(int i=0; i<data.size(); i++){
        for(int j=0; j<unq_targets.size(); j++){
            if((data.at(i).at(target)).compare(unq_targets[j]) == 0){
                counts[j] += 1;
            }
        }
    }
    for(int i=0; i<counts.size(); i++){
        probabilities.push_back(counts[i]/total);
    }
    double max = *max_element(probabilities.begin(), probabilities.end());
    return 1-max;
}

// Get the entropy measure for a dataset, given the attribute's column id, and target's column id
double getEntropy(vector<vector<string>> data, int target){
    vector<string> unq_targets = getUniqueAttributes(data, target);
    vector<double> counts;
    vector<double> probabilities;
    int total = data.size();
    for(int i=0; i<unq_targets.size(); i++){
        counts.push_back(0);
    }
    for(int i=0; i<data.size(); i++){
        for(int j=0; j<unq_targets.size(); j++){
            if((data.at(i).at(target)).compare(unq_targets[j]) == 0){
                counts[j] += 1;
            }
        }
    }
    for(int i=0; i<counts.size(); i++){
        probabilities.push_back(counts[i]/total);
    }
    double result = 0;
    for(int i=0; i<probabilities.size(); i++){
        result += (-1)*(log2(probabilities[i])*probabilities[i]);
    }
    return result;
}

// Get the Gini index for a dataset, given the attribute's column id, and target's column id
double getGini(vector<vector<string>> data, int target){
    vector<string> unq_targets = getUniqueAttributes(data, target);
    vector<double> counts;
    vector<double> probabilities;
    int total = data.size();
    for(int i=0; i<unq_targets.size(); i++){
        counts.push_back(0);
    }
    for(int i=0; i<data.size(); i++){
        for(int j=0; j<unq_targets.size(); j++){
            if((data.at(i).at(target)).compare(unq_targets[j]) == 0){
                counts[j] += 1;
            }
        }
    }
    for(int i=0; i<counts.size(); i++){
        probabilities.push_back(counts[i]/total);
    }
    double sum = 0;
    for(int i=0; i<probabilities.size(); i++){
        sum += (probabilities[i] * probabilities[i]);
    }
    double result = 1-sum;
    return result;
}

// Get the information gain for a dataset, given the attribute's column id, target's column id, and split criterion (gini or entropy)
double getGain(vector<vector<string>> data, string criterion, int attribute, int target){
    double result;
    int data_length = data.size();
    vector<string> classes = getUniqueAttributes(data, target);
    vector<string> attributes = getUniqueAttributes(data, attribute);
    int attributes_length = attributes.size();
    int length_classes = classes.size();
    if(criterion.compare("entropy") == 0){
        double impurity_parent = getEntropy(data, target);
        vector<vector<vector<string>>> subDatasets = attribute_based_filter(data, attribute);
        double sum = 0;
        for(int i=0; i<subDatasets.size(); i++){
            double entropy = getEntropy(subDatasets.at(i), target);
            double calc = (((double) (subDatasets.at(i)).size())/(double) data.size())*entropy;
            sum += calc;
        }
        result = impurity_parent - sum;
        return result;
    }
    else if(criterion.compare("gini") == 0){
        double impurity_parent = getGini(data, target);
        vector<vector<vector<string>>> subDatasets = attribute_based_filter(data, attribute);
        double sum = 0;
        for(int i=0; i<subDatasets.size(); i++){
            double gini = getGini(subDatasets.at(i), target);
            sum += (((double)(subDatasets.at(i)).size())/(double) data.size()) * gini;
        }
        result = impurity_parent - sum;
        return result;
    }
    else if(criterion.compare("misclassificationError") == 0){
        double impurity_parent = getMisclassificationError(data, target);
        vector<vector<vector<string>>> subDatasets = attribute_based_filter(data, attribute);
        double sum = 0;
        for(int i=0; i<subDatasets.size(); i++){
            double me = getMisclassificationError(subDatasets.at(i), target);
            sum += ((double) ((subDatasets.at(i)).size())/(double) data.size()) * me;
        }
        result = impurity_parent - sum;
        return result;
    }
    else{
        cout << "Invalid split criterion. Returning 0" << endl;
        return 0;
    }
    return result;
}

//Returns child's index with maximum information gain
int getMaxGainIndex(vector<vector<string>> data, string criterion, int target){
    vector<double> gains;
    for(int i=0; i<data.at(0).size(); i++){
        if(i != target){
            gains.push_back(getGain(data, criterion, i, target));
        }
    }
    int maxElementIndex = max_element(gains.begin(),gains.end()) - gains.begin();
    return maxElementIndex;
}

//Return number of instances for each value in an attribute/target
vector<pair<string, int>> getValueInstances(vector<vector<string>> data, int attribute){
    vector<pair<string, int>> result;
    vector<string> unqValues = getUniqueAttributes(data, attribute);
    for(int i=0; i<unqValues.size(); i++){
        pair<string, int> temp;
        temp.first = unqValues.at(i);
        temp.second = 0;
        result.push_back(temp);
    }
    for(int i=0; i<data.size(); i++){
        for(int j=0; j<result.size(); j++){
            if((data.at(i).at(attribute)).compare(result.at(j).first) == 0){
                result.at(j).second = result.at(j).second + 1;
            }
        }
    }
    return result;
}

//Returns p value from chi squared lookup table
double chiSquaredLookup(double degreeFreedom, double alpha){
    double lookupValue = chisqr(degreeFreedom, alpha);
    return lookupValue;
}

// //Computes X^2 value for the chosen split attribute
// double chiSquaredValue(vector<vector<string>> parentData, int attribute, int target){
//     vector<string> classes = getUniqueAttributes(parentData, target);
//     vector<string> unqValues = getUniqueAttributes(parentData, attribute);
//     int numClasses = classes.size();
//     int numValues = unqValues.size();
//     vector<pair<string, int>> classCountParent;
//     //Count class instances in the parent node
//     for(int i=0; i<classes.size(); i++){
//         pair<string, int> temp;
//         temp.first = classes.at(i);
//         temp.second = 0;
//         classCountParent.push_back(temp);
//     }
//     for(int i=0; i<parentData.size(); i++){
//         for(int j=0; j<classCountParent.size(); j++){
//             if(parentData.at(i).at(target).compare(classCountParent.at(j).first) == 0){
//                 classCountParent.at(j).second = classCountParent.at(j).second + 1;
//             }
//         }
//     }
//     //Store real counts and expected counts for each child node
//     vector<pair<string, vector<vector<string>>>> splitData_all = attribute_based_split_labelled_all(parentData, attribute);
//     vector<pair<pair<string, string>, double>> expectedCounts;
//     vector<pair<pair<string, string>, double>> realCounts;
//     for(int i=0; i<splitData_all.size(); i++){
//         string child_attr = splitData_all.at(i).first;
//         vector<pair<string, vector<vector<string>>>> subSplitData_all = attribute_based_split_labelled_all(splitData_all.at(i).second, target);
//         for(int j=0; j<subSplitData_all.size(); j++){
//             pair<string, string> tempStringPair;
//             tempStringPair.first = child_attr;
//             tempStringPair.second = subSplitData_all.at(j).first;
//             pair<pair<string, string>, double> countPair;
//             countPair.first = tempStringPair;
//             countPair.second = (double) 0;
//             expectedCounts.push_back(countPair);
//             realCounts.push_back(countPair);
//         }
//     }
//     //Store real counts
//     for(int i=0; i<splitData_all.size(); i++){
//         string attr = splitData_all.at(i).first;
//         int count = (int) splitData_all.at(i).second.size();
//         for(int j=0; j<splitData_all.at(i).second.size(); j++){
//             vector<pair<string, vector<vector<string>>>> subSplitData_all = attribute_based_split_labelled_all(splitData_all.at(i).second, target);
//             for(int k=0; k<subSplitData_all.size(); k++){
//                 string class_ = subSplitData_all.at(k).first;
//                 for(int m=0; m<realCounts.size(); m++){
//                     if((realCounts.at(m).first.first).compare(attr) == 0){
//                         if((realCounts.at(m).first.second).compare(class_) == 0){
//                             realCounts.at(m).second = realCounts.at(m).second + 1;
//                         }
//                     }
//                 }
//             }
//         }
//     }
    
//     //Calculate expected counts
//     vector<pair<string, int>> parentClassCounts = getValueInstances(parentData, target);
//     int parentTotal = 0;
//     for(int i=0; i<parentClassCounts.size(); i++){
//         parentTotal += parentClassCounts.at(i).second;
//     }
//     vector<pair<string, double>> classProportions; //store proportions for targets in parent node
//     for(int i=0; i<parentClassCounts.size(); i++){
//         double prop;
//         prop = ((double) parentClassCounts.at(i).second)/((double) parentTotal);
//         pair<string, double> temp;
//         temp.first = parentClassCounts.at(i).first;
//         temp.second = prop;
//         classProportions.push_back(temp); 
//     }
    
//     for(int i=0; i<splitData_all.size(); i++){
//         string attr = splitData_all.at(i).first;
//         int count = splitData_all.at(i).second.size();
//         for(int j=0; j<classes.size(); j++){
//             string class_ = classes.at(j);
//             for(int k=0; k<expectedCounts.size(); k++){
//                 if(((expectedCounts.at(k).first.first).compare(attr) == 0) && ((expectedCounts.at(k).first.second).compare(class_) == 0)){
//                     double proportion = 0;
//                     for(int m=0; m<classProportions.size(); m++){
//                         if(classProportions.at(m).first.compare(class_) == 0){
//                             proportion = classProportions.at(m).second;
//                         }
//                     }
//                     double value = (double) proportion * (double) count;
//                     expectedCounts.at(k).second = value;
//                 }
//             }
//         }
//     }
//     double chiSqValue = 0;
//     for(int i=0; i<realCounts.size(); i++){
//         double diff = realCounts.at(i).second - expectedCounts.at(i).second;
//         double ret = (diff*diff)/expectedCounts.at(i).second;
//         chiSqValue += ret;
//     }
//     return chiSqValue;
// }

//Computes X^2 value for the chosen split attribute - using unordered_maps
double chiSquaredValue(vector<vector<string>> parentData, int attribute, int target){
    vector<string> classes = getUniqueAttributes(parentData, target);
    vector<string> unqValues = getUniqueAttributes(parentData, attribute);
    int numClasses = classes.size();
    int numValues = unqValues.size();
    //vector<pair<string, int>> classCountParent;
    unordered_map<string, int> classCountParent;
    //Count class instances in the parent node
    for(int i=0; i<classes.size(); i++){
        classCountParent[classes.at(i)] = 0;
    }
    for(int i=0; i<parentData.size(); i++){
        classCountParent[parentData.at(i).at(target)] = classCountParent[parentData.at(i).at(target)] + 1;
    }
    //Store real counts and expected counts for each child node
    vector<pair<string, vector<vector<string>>>> splitData_all = attribute_based_split_labelled_all(parentData, attribute);
    unordered_map<string, unordered_map<string, double>> expectedCounts; //in the format outer string = attr, inner string = target
    unordered_map<string, unordered_map<string, double>> realCounts; //in the format outer string = attr, inner string = target
    unordered_map<string, int> childrenCounts;
    //Store real counts
    for(int i=0; i<parentData.size(); i++){
        if(realCounts.count(parentData.at(i).at(attribute))){
            if(realCounts[parentData.at(i).at(attribute)].count(parentData.at(i).at(target))){
                realCounts[parentData.at(i).at(attribute)][parentData.at(i).at(target)] = 
                realCounts[parentData.at(i).at(attribute)][parentData.at(i).at(target)] + 1; 
            }
            else{
                realCounts[parentData.at(i).at(attribute)][parentData.at(i).at(target)] = 1;
            }
        }
        else{
            unordered_map<string, double> temp;
            temp[parentData.at(i).at(target)] = 1;
            realCounts[parentData.at(i).at(attribute)] = temp;
        }
        if(childrenCounts.count(parentData.at(i).at(attribute))){
            childrenCounts[parentData.at(i).at(attribute)] = childrenCounts[parentData.at(i).at(attribute)] + 1;//Get total children counts
        }
        else{
            childrenCounts[parentData.at(i).at(attribute)] = 1;//Get total children counts
        }
    }
    
    int parentDataSize = (int) parentData.size();
    for(int i=0; i<classes.size(); i++){
        for(int j=0; j<unqValues.size(); j++){
            expectedCounts[unqValues.at(j)][classes.at(i)] = 
            ((double) childrenCounts[unqValues.at(j)] * ((double) classCountParent[classes.at(i)]/ (double) parentDataSize)); 
        } 
    }

    double chiSqValue = 0;
    for(int i=0; i<classes.size(); i++){
        for(int j=0; j<unqValues.size(); j++){
            double diff = ((double) realCounts[unqValues.at(j)][classes.at(i)]) - ((double) expectedCounts[unqValues.at(j)][classes.at(i)]);
            double ret = (diff * diff)/expectedCounts[unqValues.at(j)][classes.at(i)];
            chiSqValue = chiSqValue + ret;
        }
    }
    return chiSqValue;
}

//Returns true if the chosen attribute based on the dataset passes the chi squared test
bool chiSquaredTest(vector<vector<string>> parentData, int attribute, double confidence, int target){
    double alpha = 1 - confidence;
    double X2 = chiSquaredValue(parentData, attribute, target);
    int dof;
    vector<string> classes = getUniqueAttributes(parentData, target);
    vector<string> attr_values = getUniqueAttributes(parentData, attribute);
    dof = ((int) classes.size() - 1) * ((int) attr_values.size() - 1);
    double lookup = chiSquaredLookup(dof, alpha);
    if(X2 > lookup) return true;
    else return false;
}

/*

//Return vector of vector of attributes that have randomly sampled (with replacement) features. Includes target
vector<vector<int>> bagFeaturesIndices(vector<vector<string>> dataset, int target, int numBags, int minFeatureSize){
    vector<vector<int>> selectedAttributes;
    vector<int> in;
    for(int i=0; i<dataset.at(0).size(); i++){
        if(i != target) in.push_back(i);
    }
    for(int i=0; i<numBags; i++){
        vector<int> out;
        srand(i);
        int diff = ((int) in.size()) - minFeatureSize;
        int num = rand() % diff + minFeatureSize;
        std::sample(in.begin(), in.end(), std::back_inserter(out), num, std::mt19937{std::random_device{}()});
        out.push_back(target);
        selectedAttributes.push_back(out);
    }
    return selectedAttributes;
}



//Return vector of datasets that have randomly sampled (with replacement) features
//Incomplete
vector<vector<vector<string>>> bagFeatures(vector<vector<string>> dataset, vector<vector<int>> baggedIndices){
    vector<vector<vector<string>>> result;
    for(int i=0; i<baggedIndices.size(); i++){
        vector<vector<string>> temp;
        result.push_back(temp);
    }
    for(int i=0; i<dataset.size(); i++){
        for(int j=0; j<baggedIndices.size(); j++){
            vector<string> temp;
            for(int k=0; k<baggedIndices.at(j).size(); k++){
                temp.push_back(dataset.at(i).at(baggedIndices.at(j).at(k)));
            }
            result.at(j).push_back(temp);
        }
    }
    return result;
}
*/
//Print wrappers - polymorphism for various data types
void println(string s){
    cout << s << endl;
}

void print(string s){
    cout << s;
}

void println(int s){
    cout << s << endl;
}

void print(int s){
    cout << s;
}

void println(short s){
    cout << s << endl;
}

void print(short s){
    cout << s;
}

void println(long s){
    cout << s << endl;
}

void print(long s){
    cout << s;
}

void println(float s){
    cout << s << endl;
}

void print(float s){
    cout << s;
}

void println(double s){
    cout << s << endl;
}

void print(double s){
    cout << s;
}

void println(bool s){
    cout << s << endl;
}

void print(bool s){
    cout << s;
}


