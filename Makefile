all:   # Add new files to this target's compil chain
	g++ -o Main main.cpp node.cpp node.h pythonpp.cpp pythonpp.h  tree.cpp tree.h 

rf: #Compile files for Random Forest
	g++ pythonpp.h pythonpp.cpp randomForest.cpp randomForest.h tree.cpp tree.h node.cpp node.h chisqr.c chisqr.h gamma.c gamma.h -o rfTest.o -g -std=gnu++17

runrf: #Run Random Forest executable
	./rfTest.o && rm rfTest.o

run:
	./Main && rm Main

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
