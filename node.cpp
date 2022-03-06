#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <utility>  
#include <stdexcept>  
#include <unordered_map>
#include "node.h"

using namespace std;

// Constructor intended for root node
Node::Node(string label, vector<vector<string>> data) {
    id_num = id_count;
    dataset.label = label;
    dataset.data = data;
    target = data.at(0).size() - 1;
    attribute = getMaxGainIndex(data, criterion, target);
    gain = getGain(data, criterion, attribute, target); 
    isLeaf = false;
    parent = NULL;
    level = 0;
    uniqueAttributes = getUniqueAttributes(data, target);


    id_count = id_count + 1;    // Increment static variable

}

// Alternate constructor for non-root nodes
Node::Node(LabeledDataset data) {
    id_num = id_count;
    dataset = data;
    target = ((int) data.data.at(0).size()) - 1;
    isLeaf = false;
    attribute = getMaxGainIndex(data.data, criterion, target);
    gain = getGain(data.data, criterion, attribute, target);
    uniqueAttributes = getUniqueAttributes(data.data, target);

    id_count++;    // Increment static variable
    

    // If there are no more dvisions to make to the dataset, node is a leaf
    if (dataSetPurityTest() >= datasetPurity) {  
        isLeaf = true;
    }

}



Node::~Node() {
    // delete your children
    for (int i = 0; i < children.size(); i++) {
        delete children.at(i);
    }

}

// Returns the purity of the dataset. i.e. the percentage that the majority class takes up in the dataset
double Node::dataSetPurityTest() {
    unordered_map<string, int> labelCount;

    for (int i = 0; i < uniqueAttributes.size(); i++) {
        labelCount[uniqueAttributes.at(i)] = 0;
    }

    for (int i = 0; i < dataset.data.size(); i++) {
        labelCount[dataset.data.at(i).at(target)] = labelCount[dataset.data.at(i).at(target)] + 1;
    }

    double total = (double) dataset.data.size();
    double maxPercentage = 0.0;

    double candidatePercentage;
    for (int i = 0; i < uniqueAttributes.size(); i++) {
        candidatePercentage = labelCount[uniqueAttributes.at(i)] / total;
        // print("Candidate %: ");
        // println((float) candidatePercentage);
        if (candidatePercentage > maxPercentage) {
            maxPercentage = candidatePercentage;
            majorityLabel = uniqueAttributes.at(i);
        }
    }

    return maxPercentage;
}

// Splits current nodes dataset and uses the sub datasets to create child nodes
void Node::initializeChildren() {
    if (isLeaf) {
        return;
    }

    if (level >= max_depth) {
        return;
    }

    // Are we using chi squared test
    if (chiSquared) {
        if (!chiSquaredTest(dataset.data, attribute, confidence, target)) {  
            isLeaf = true;
            return;
        }
    } 

    
    vector<LabeledDataset> datasets = splitDataset(dataset);
    int n = (int) datasets.size();

    for (int i = 0; i < n; i++) {
        Node * child = new Node(datasets.at(i));
        child->parent = this;
        child->setLevel();

        
        children.push_back(child);
    }

    

    if (children.size() < 2) {
        isLeaf = true;
    } else {
        int mostDiverseChildSize = (int) children.at(0)->uniqueAttributes.size();
        mostDiverseChild = children.at(0);

        for (int i = 0; i < n; i++) {
            if (children.at(i)->uniqueAttributes.size() > mostDiverseChildSize) {
                mostDiverseChildSize = (int) children.at(i)->uniqueAttributes.size();
                mostDiverseChild = children.at(i);
            }

        }

    }
}

vector<LabeledDataset> Node::splitDataset(LabeledDataset data) {
    vector<pair<string, vector<vector<string>>>> temp = attribute_based_split_labelled_all(data.data, attribute);

    vector<LabeledDataset> result;

    for (int i = 0; i < temp.size(); i = i + 1) {
        LabeledDataset subset;
        subset.label = temp[i].first;
        subset.data = temp[i].second;
        result.push_back(subset);
    }
     
    return result; 
}

int Node::getId() {
    return id_num;
}

string Node::getLabel() {
    return dataset.label;
}

void Node::setLevel() {
    level = parent->level + 1;
}

int Node::getLevel() {
    return level;
}


// Set default static variables
int Node::id_count = 0;
string Node::criterion = "misclassificationError";
double Node::datasetPurity = 0.9;
double Node::confidence = 1.0;
bool Node::chiSquared = false;
int Node::max_depth = 99999999;
