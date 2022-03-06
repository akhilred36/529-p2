#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <utility>       // pair
#include <stdexcept>    // runtime_error
#include <sstream>     // stringstream
#include "pythonpp.h"

using namespace std;

// Holds subdatasets and their associated labels
struct LabeledDataset {
  string label;
  vector<vector<string>> data;
};

class Node{

    public:
        //Vector to hold node pointers
        static int id_count;
        static int max_depth;
        static string criterion;
        static double datasetPurity;
        static double confidence;
        static bool chiSquared;

        int id_num;
        int target;
        int level;
        Node * parent;
        Node * mostDiverseChild;
        double gain;
        vector<Node *> children;
        int attribute;
        struct LabeledDataset dataset;
        bool isLeaf;
        string majorityLabel;
        vector<string> uniqueAttributes;


        Node(string label, vector<vector<string>> data);

        Node(LabeledDataset data);

        ~Node();

        
        void initializeChildren();

        int getId();

        string getLabel();

        void setLevel();

        int getLevel();


    private:
        vector<LabeledDataset> splitDataset(LabeledDataset data);

        double dataSetPurityTest();

};

