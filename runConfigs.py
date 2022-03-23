import os
import subprocess
import multiprocessing as mp 
from multiprocessing import Pool

def runShell(input1):
    input2=["./main.out",
     "wordToClassCount.mtx",
      "../vocabulary.txt",
       "../newsgrouplabels.txt",
        "customTest.csv"]
    input2.extend(input1)
    print("input: ", input2)
    process = subprocess.Popen(input2, stdout=subprocess.PIPE, stderr=None)
    process.wait()
    output = process.communicate()[0]
    print(output)
    print("Finished executing")
    

if __name__=='__main__':
    mp.set_start_method('spawn')

    configs = []
    current = 0.00001
    end = 1
    while(current < end):
        configs.append([str(current)])
        current += 0.00005
    with Pool(64) as p:
        p.map(runShell, configs)
    print("Finished")
