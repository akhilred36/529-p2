#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <utility> // pair
#include <stdexcept> // runtime_error
#include "tree.h"
#include "pythonpp.h"

class Forest{
    public:
        int numTrees;
        string pruneMethod; //Valid values are "X2inBuild", "purityThreshold"
        string criterion;

        Forest(vector<vector<string>> dataset, int target, int numBags, int minFeatureSize, string pruneMethod, string splitCriterion, double confidence);
        Forest(vector<vector<string>> data, int target, int numBags, int minFeatureSize);
        ~Forest();

        void train();
        void setSplitCriterion(string criterion);
        void setPruneMethod(string method);
        string predict(vector<string> features);

    private:
        vector<Tree *> trees;
        vector<vector<int>> datasetIndices;
        vector<vector<vector<string>>> datasets;
};