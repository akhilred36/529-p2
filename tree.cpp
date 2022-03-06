#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <utility> // pair
#include <stdexcept> // runtime_error
#include <unordered_map>
#include "tree.h"



Tree::Tree(vector<vector<string>> data) {

    // Initialize tree by creating root node
    root = new Node("root", data);
    node_count = 1;
    nodes.push_back(root);

}

Tree::~Tree() {
    delete root;
}

// Sets what criteria the tree will use to split nodes
void Tree::setSplitCriterion(string criterion) {
    Node::criterion = criterion;
}

// Sets the threshold at which the tree stops making new nodes split nodes
void Tree::setPurityThreshold(double purity) {
    Node::datasetPurity = purity;
}

// Creates the full tree
void Tree::train() {
    int curr = 0;
    
    // While we still have nodes to process, continue
    while (curr < node_count) {

        // Initialize children of current nodes
        nodes.at(curr)->initializeChildren();
        
        // Update depth as needed
        if (nodes.at(curr)->getLevel() > depth) {
            depth = nodes.at(curr)->getLevel();
        }

        // If current node is a leaf, skip
        if (nodes.at(curr)->isLeaf) {
            curr++;
            continue;
        }
        
        // Add all of the current node's children to node list
        for (int i = 0; i < (int) nodes.at(curr)->children.size(); i++){  
            nodes.push_back(nodes.at(curr)->children.at(i));
            node_count++;
        }

        curr++;
        
    }
    cout << "Node count: " << node_count << endl;

}

// Predict label from list of features
string Tree::predict(vector<string> features) {
    Node* curr = root;

    // While we can still make decisions
    while (!curr->isLeaf) {

        // find next node/decision criteria
        curr = returnCorrectChild(curr, features.at(curr->attribute));
    }

    return curr->majorityLabel;
}

// Returns child that matches the target label from the prediction
Node* Tree::returnCorrectChild(Node* parent, string targetLabel) {

    for (int i = 0; i < parent->children.size(); i++){
        if (parent->children.at(i)->getLabel() == targetLabel) {
            return parent->children.at(i);
        }
    }

    return parent->mostDiverseChild;   // Most likely to have correct answer if target node is missing
}

// Toggles chiSquared as the method for determining if a node should be a leaf
void Tree::toggleChiSquared() {
    Node::chiSquared = true;
};

// Sets confidence score for chisqr test
void Tree::setConfidence(double con) {
    Node::confidence = con;
};

// Serts the maximum depth for the tree. By default, max depth is 99999999
void Tree::setMaxDepth(int md) {
    Node::max_depth = md;
};

// Test tree
void Tree::test(vector<vector<string>> test, int target_column) {
    float correct = 0;
    float total = 0;

    pair<vector<vector<string>>, vector<string>> feature_label = seperateTargets(test, target_column);
    vector<vector<string>> X = feature_label.first;
    vector<string> Y = feature_label.second;


    string prediction;
    for (int i = 0; i < (int) X.size(); i++) {
        prediction = predict(X.at(i));
        if (prediction.compare(Y.at(i)) == 0) {
            correct = correct + 1;
        } else {
            // for (int j = 0; j < X.at(i).size(); j++) {
            //     print(X.at(i).at(j));
            //     print(", ");
            // }
            // print("| ");
            // print((int) prediction.length());
            // print(" - ");
            // print(prediction);
            // println(" |");
            // print(" =/= ");
            // println(Y.at(i));
            // cout << "| " << prediction.length() << " - " << Y.at(i).length() << " |" << endl;
            // cout << "| " << prediction << " - " << Y.at(i) << " |" << endl;
        }

        total = total + 1;
    }  

    cout << "Acc.\n" << "------------------------------\n" << "Correct: " << correct << endl << "Total: " << total << endl << "Percent: " << (correct / total) * 100 << "%" << endl << endl;

}

// Generate predictions for unseen feature set. Outputs into file
void Tree::testUnseen(vector<vector<string>> X) {

    ofstream myfile;
    myfile.open ("submission.csv");
    for (int i = 0; i < (int) X.size(); i++) {
        myfile << predict(X.at(i)) << "," << endl;
    } 
    myfile.close();

}

// Biulds list of paths from every leaf node back to the root
void Tree::buildPaths(vector<vector<Node *>> &paths) {
    for (Node * node: nodes) {
        if (node->isLeaf) {
            Node * curr;

            curr = node;
            vector<Node *> path;

            while (curr->getId() != root->getId()) {
                path.push_back(curr);
                curr = curr->parent;
            }

            paths.push_back(path);
        }
    }
}


// Wrapper function for testing unknown dataset
void runHiddenDataset() {
    // Import Dataset
    vector<vector<string>> data = read_csv("train_refined.csv");

    vector<vector<string>> train = shuffleDataFrame(seperateHeader(data).second);

    Tree t(train);

    t.setSplitCriterion("gini");
    // t.setPurityThreshold(1);
    t.toggleChiSquared();
    t.setConfidence(0.9);

    t.train();

    vector<vector<string>> test = seperateTargets(seperateHeader(read_csv("test_refined.csv")).second, 0).first;
  
    t.testUnseen(test);
}

// Wrapper function for testing known dataset
void runKnownDataset(string dataset, double splitPercent, string splitCriterion, string leafTest, double leafTestValue) {
    // Import Dataset

    vector<vector<string>> data = read_csv(dataset);

    pair<vector<vector<string>>, vector<vector<string>>> train_test = train_test_split(shuffleDataFrame(seperateHeader(data).second),splitPercent);

    vector<vector<string>> train = train_test.first;
    vector<vector<string>> test = train_test.second;

    Tree t(train);

    t.setSplitCriterion(splitCriterion);

    // switch (leafTest) {
    //     case "purity":
    //         t.setPurityThreshold(leafTestValue);
    //         break;
    //     case "chisqr":
    //         t.toggleChiSquared();
    //         t.setConfidence(leafTestValue);
    //         break;
    //     default:
    //         t.setPurityThreshold(leafTestValue);
    //         break;
    // }

    

    t.train();

    int target_column = ((int) train.at(0).size()) - 1;   // Last column is target

    t.test(test, target_column);
    t.test(train, target_column);
}


// int main(int argc, char** argv) {
//     cout<< "Start" << endl;

//     string datasetFile = "test_refined.csv";
//     double splitPercent = 0.8;
//     string splitCriterion = "gini";
//     string leafTest = "chisqr";
//     double leafTestValue = 0.9;

//     if (argc == 5) {
//         datasetFile = argv[0];
//         splitPercent = stod(argv[1]);
//         splitCriterion = argv[2];
//         leafTest = argv[3];
//         leafTestValue = stod(argv[4]);

//         runKnownDataset(datasetFile, splitPercent, splitCriterion, leafTest, leafTestValue);
//     } else {
//         cout << "Incorrect number of arguments. Defaults will be used" << endl;
//         runKnownDataset(datasetFile, splitPercent, splitCriterion, leafTest, leafTestValue);
//     }
    
//     return 0;
// }