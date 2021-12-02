#include <iostream>
#include <fstream>
#include <cstdlib>
#include <string>

#include "DAG.h"
#include "DAG.cpp"

int main(int argc, char** argv)
{
	//first input argument is always the program name...
	char* fileName = argv[1];
	// std::string fileName = "input.txt";
	//open the file
	std::ifstream fin(fileName);
	
	if(!fin)
	{
		std::cout<< "File not found" << std::endl;
		return EXIT_FAILURE;
	}
	
	DAG graph;
	std::string line;
	std::string filename = "";
	std::vector<std::string> includes;
	while(std::getline(fin,line))
	{
		//this is just to get you going.  You'll need to do much more here...
		std::cout << line << std::endl;

		if(line.find("#include") != 0) {
			if(filename != "")
				graph.add_file(filename, includes);
				includes.clear();
			filename = line;
			continue;
		} else {
			std::size_t start_pos = line.find('<');
			std::size_t end_pos = line.find('>');
			includes.push_back(line.substr(start_pos + 1, end_pos - start_pos - 1));
		}
	}

	graph.add_file(filename, includes);

	std::ofstream fout("out.txt");
	// iterate through the DAG in a topological order
	for (DAG::Iterator itr = graph.begin(); itr != graph.end(); ++itr) {
		fout << *itr << std::endl;
	}
		
	
	return EXIT_SUCCESS;
}