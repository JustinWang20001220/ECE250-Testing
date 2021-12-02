answer = open("out.txt", "r")
f = open("python_test.txt")

main_dag = {}
file_list = {}
include_list = {}

"""
Hi Guys, Joey here. If you see this that means you really want to see what's going on behind the scenes, please
The verification works similarily to your code in that it will create a dag and than compare your outputs generated
by your cpp file and ensure that compilation is in a topological order.

This is the Python Verification Code as part of the ECE 250 Testing Prject made by Joey Chen, Zefei Ou, and Zi'an Wang
"""

# Reading the input file and generate a few dictionaries that will be used to confirm topological sort
for x in f:
    if "#include" not in x:
        current_file = x.strip()
        main_dag[current_file] = []
        file_list[current_file] = 0
    else:
        file = x.split()[1].strip('<>')
        main_dag[current_file].append(file)
        if "FILE" not in x:
            include_list[file] = 0

for x in answer:
    line = x.strip()
    if "FILE" in line: # Procedure of Checking for File
        # First check if the file exists and if it hasn't been compiled already
        if file_list.get(line) == 0:
            file_list[line] = 1
        elif file_list.get(line) == 1:
            print("ERROR: You've already compiled " +line+ " before. There should not be any duplicates")
        else:
            print("ERROR: You're trying to compile " +line+ " but this file does not exist!")

        # Check if we are allowed to compile this file (The dependencies have been compiled)
        for dep in main_dag.get(line):
            if include_list.get(dep) != 1 and file_list.get(dep) != 1:
                print ("ERROR: You cannot compile " +line+ " because its depedency on "+dep+" which hasn't been compiled yet")
        
    else: #include of Checking Regular Import Files
        # Check if the file exists and if it hasn't been compiled already
        if include_list.get(line) == 0:
            include_list[line] = 1
        elif include_list.get(line) == 1:
            print("ERROR: You've already compiled "+line+ " before. There should not be any duplicates")
        else:
            print("ERROR: You're trying to compile "+line+ " but this file does not exist!")


# Check if all files and include files have been compiled
for key, value in include_list.items():
    if value != 1:
        print("ERROR: You did not compile " +key)

for key, value in file_list.items():
    if value != 1:
        print("ERROR: You did not compile " +key)


print("Testing Done!")
f.close()
answer.close()


