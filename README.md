# 529-p2
Naive Bayes Classifier and Logistic Regression implementation in C++

# Naive Bayes Classifier
## Compilation:
To compile the project, run  
``` bash
make preprocess
```
``` bash
make build_nb
```

## Prediction
To train and predict after compiling the project, run:  
``` bash
./main.out nb wordToClassCount.mtx <vocabularyFile> <labelsFile> <testing.csv> <betaValue> 
```

## Optimal Configuration for data preprocessing and hyperparameter tuning
Run the following code for optimal results.  
Note: It is assumed that the input train and test datasets have been preprocessed. It is also assumed that the file contains headers and that the target column is the last column.  
``` bash
./main.out nb wordToClassCount.mtx <vocabularyFile> <labelsFile> <testing.csv> 0.02
```
                                                                                          
                                                                                          
# Logistic Regression
## Compilation:
To compile the project, run  
``` bash
make preprocess
```
``` bash
make build_lr
```

## Prediction
To train and predict after compiling the project, run:  
``` bash
./main.out lr dataMatrix.mtx <vocabularyFile> <labelsFile> <learningRate> <penaltyTerm> <numberOfIterations>
```

## Optimal Configuration for data preprocessing and hyperparameter tuning
Run the following code for optimal results.  
Note: It is assumed that the input train and test datasets have been preprocessed <Insert Pre-processing steps. It is also assumed that the file contains headers and that the target column is the last column.  
``` bash
./main.out lr dataMatrix.mtx <vocabularyFile> <labelsFile> 0.001 0.01 500
```
