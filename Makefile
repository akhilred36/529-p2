all:   # Add new files to this target's compil chain
	g++ -o main.out main.out.cpp node.cpp node.h pythonpp.cpp pythonpp.h  tree.cpp tree.h 

preprocess:
	g++ -o preprocess.out preprocess.cpp chisqr.c chisqr.h gamma.c gamma.h pythonpp.cpp pythonpp.h -g -std=gnu++17 && rm *.vec && rm *.mtx

build_nb:
	g++ -o main.out.out main.out.cpp pythonpp.cpp pythonpp.h chisqr.c chisqr.h gamma.c gamma.h -std=gnu++17 -g NaiveBayesClassifier.h 

run_nb:
	./main.out.out wordToClassCount.mtx testing.csv 0.0001

debug:
	g++ -o main.out.out main.out.cpp chisqr.c chisqr.h gamma.c gamma.h pythonpp.cpp pythonpp.h -g -std=gnu++17

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
