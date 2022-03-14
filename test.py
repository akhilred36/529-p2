import csv
 
# csv file name
filename = "wordToClassCount.mtx"
 
# initializing the titles and rows list
fields = []
rows = []
 
# reading csv file
with open(filename, 'r') as csvfile:
    # creating a csv reader object
    csvreader = csv.reader(csvfile)
     
    # extracting field names through first row
    # fields = next(csvreader)
 
    # extracting each data row one by one
    for row in csvreader:
        print(len(row))
        rows.append(row)
 
    # get total number of rows
    print("Total no. of rows: %d"%(csvreader.line_num))