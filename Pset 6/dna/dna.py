import csv
from sys import argv 
from sys import exit


def main():
    # If there are less than 3 command line arguments, exit
    if len(argv) != 3:
        exit("Usage: python dna.py database.csv sequence.txt")
    
    # Load STR numbers as keys in dictionary corresponding to a person
    database = []
    with open(argv[1], 'r') as file:
        reader = csv.reader(file)
        for row in reader:
            # Copy the STRs from the first row
            if row[0] == "name":
                STR = row[1:]
            else:
                # Change the strings in row into ints then load dictionary (person) and list (database)
                for i in range(1, len(row)):
                    row[i] = int(row[i])
                person = {
                    tuple(row[1:]): row[0]
                }
                database.append(person)
    
    # Open the sequence.txt file and load it into a string
    with open(argv[2], 'r') as file:
        sequence = file.read()
    
    # Create an empty array to keep count of STRs
    repeats = [0] * len(STR)
    
    # Finding repeats
    i = 0  # Sequence indexing variable
    while i < (len(sequence)):
        for j in range(len(STR)):  # j is the STR[] indexing variable
            if sequence[i:i+len(STR[j])] == STR[j]:  # Check if the character STR[i] is the start of an STR
                # Update the STR number and move i past this counted STR sequence instead of recounting the same section of the DNA sequence with an incrementation of i
                i = STRcounter(i, j, sequence, STR, repeats) - 1 
                break
            
        i += 1  # Add 1 to move indexing variable over
    
    # Searching database for a person with the same STR numbers
    repeats_t = tuple(repeats) 
    for n in range(len(database)):
        if repeats_t in database[n]:
            print(database[n][repeats_t])
            exit()
    print("No match")


def STRcounter(ind, loc, sequence, STR, repeats):
    counter = 0
            
    # Keep counting until the sequence stops repeating
    while True:
        counter += 1
        ind += len(STR[loc])
        if sequence[ind:ind+len(STR[loc])] != STR[loc]:
            break
                
    # If more repeats are found than previous searches, store the value 
    if counter > repeats[loc]:
        repeats[loc] = counter
    
    # Move the indexing variable past this sequence. "len(STR[loc] + 1" is included in the event of an overlapping STR
    return ind - len(STR[loc]) + 1


main()