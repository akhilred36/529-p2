all:   # Add new files to this target's compil chain
	g++ -o main.out main.cpp node.cpp node.h pythonpp.cpp pythonpp.h  tree.cpp tree.h 

preprocess:
	g++ -I eigen/ -o preprocess.out preprocess.cpp chisqr.c chisqr.h gamma.c gamma.h pythonpp.cpp pythonpp.h -g -std=gnu++17 && rm *.vec && rm *.mtx && ./preprocess.out ../training.csv ../vocabulary.txt ../newsgrouplabels.txt 1

build:
	g++ -I eigen/ main.cpp pythonpp.cpp pythonpp.h chisqr.c chisqr.h gamma.c gamma.h -std=gnu++17 -g logisticRegressionClassifier.h NaiveBayesClassifier.h -o main.out

build_nb:
	g++ -I eigen/ -o main.out main.cpp pythonpp.cpp pythonpp.h chisqr.c chisqr.h gamma.c gamma.h -std=gnu++17 -g NaiveBayesClassifier.h 

run_nb:
	./main.out nb wordToClassCount.mtx ../vocabulary.txt ../newsgrouplabels.txt ../testing.csv 0.02

build_lr:
	g++ -I eigen/ main.cpp pythonpp.cpp pythonpp.h chisqr.c chisqr.h gamma.c gamma.h -fopenmp -std=gnu++17 -g logisticRegressionClassifier.h -o main.out 

run_lr:
	./main.out lr ../training.csv ../vocabulary.txt ../newsgrouplabels.txt 0.001 0.01 1 

debug:
	g++ -o main.out main.cpp chisqr.c chisqr.h gamma.c gamma.h pythonpp.cpp pythonpp.h -g -std=gnu++17

rf: #Compile files for Random Forest
	g++ pythonpp.h pythonpp.cpp randomForest.cpp randomForest.h tree.cpp tree.h node.cpp node.h chisqr.c chisqr.h gamma.c gamma.h -o rfTest.o -g -std=gnu++17

runrf: #Run Random Forest executable
	./rfTest.o && rm rfTest.o

run:
	./main.out && rm main.out

# Each class gets its own target for testing purposes
node:
	g++ -o testNode node.cpp node.h pythonpp.cpp pythonpp.h  

test_node:
	./testNode && rm testNode

tree:
	g++ -o testTree tree.cpp tree.h node.cpp node.h pythonpp.cpp pythonpp.h chisqr.c chisqr.h gamma.c gamma.h

test_tree:
	./testTree && rm testTree

# Nothing here yet
clean:
	rm *.o
