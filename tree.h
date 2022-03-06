#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <utility> // pair
#include <stdexcept> // runtime_error
#include <sstream> // stringstream
#include "node.h"

using namespace std;


class Tree {

    public:
        int node_count;
        int depth = 0;
        vector<vector<Node *>> paths; 


        Tree(vector<vector<string>> data);
        Tree(string csv_file);
        ~Tree();

        void train();
        string predict(vector<string> features);
        void setSplitCriterion(string criterion);
        void setPurityThreshold(double purity);
        void toggleChiSquared();
        void setConfidence(double con);
        void setMaxDepth(int md);
        void test(vector<vector<string>> test, int target_column);
        void testUnseen(vector<vector<string>> test);

    private:
        Node* root;
        vector<Node *> nodes;
        Node* returnCorrectChild(Node* parent, string targetLabel);
        void buildPaths(vector<vector<Node *>> &paths);
        
        
    
};

void runHiddenDataset();
void runKnownDataset();