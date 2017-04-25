#include <fstream>
#include <vector>
#include <set>
#include <iostream>
#include <utility>

using namespace std;

// Explanation of basic functionality
// Called if error occurs
void usage() 
{
    cerr << "Usage: " << endl
    	 << "./pagerank [filename][rounds] " << endl
         << "filename" << endl
         << "    the name of the file you want to perform a PageRank on " << endl
         << "rounds" << endl
         << "    the amount of rounds you want PageRank to run for " << endl;
};

// Main pagerank function, called after error checking
int pagerank(char*  args[]) 
{
	ifstream file(args[1]);

	set<int> nodes;
	vector< pair<int,int> > adjacency_list;
	
	// First and second line from the input file
	int value1, value2;
	// second command line argument, specifies amount of rounds
	int rounds = stoi(args[2]);

	// Read input stream and push to node list for unique identifies, as well as adjacency list for neighbor pairs
	while(file >> value1 >> value2)
	{
		nodes.insert(value1);
		nodes.insert(value2);
		adjacency_list.push_back(pair<int, int> (value1, value2));
	}

	cout << "Set size: " << nodes.size() << endl;
	cout << "Nodes: " << endl;
	for (set<int>::iterator it=nodes.begin(); it!=nodes.end(); ++it){
		cout << *it << endl;
    }
    cout << "Adjacency List: " << endl;
    for ( pair<int,int> 	&edge : adjacency_list ){
   		cout << edge.first << "\t" << edge.second << endl;
	}
    return 0;
};

// Helper function to check if second argument (the file input) exists and is accessible
bool fileExists(char file[])
{
	ifstream test_file(file);

    if (!test_file.is_open()) {
    	test_file.close();
        return false;
    }
    test_file.close();
    return true;
};

// Helper function to check if third argument (the amount of rounds) is an integer value
// Taken from http://stackoverflow.com/questions/29248585/c-checking-command-line-argument-is-integer-or-not
bool isNumber(char number[])
{
    int i = 0;
    
    //checking for negative numbers
    if (number[0] == '-')
        i = 1;
    for (; number[i] != 0; i++)
    {
        //if (number[i] > '9' || number[i] < '0')
        if (!isdigit(number[i]))
            return false;
    }
    return true;
};

// Check if amount of arguments passed is exactly 3 (the program itself, file name and amount of rounds desired)
// Exit gracefully if not, display error message and call usage(), which explains basic functionality 
int main(int argc, char* argv[])
{
    if (argc == 1){
    	cerr << "" << endl
    		 << "Zero arguments were passed, two are needed" << endl
    		 << "" << endl;
    	usage();
        exit(1);
    }	
    if (argc == 2){
    	cerr << "" << endl
    		 << "One argument was passed, two are needed" << endl
    		 << "" << endl;
    	usage();
        exit(1);
    }
    if (argc > 3){
    	cerr << "" << endl
    		 << "More than two arguments were passed, two are needed" << endl
    		 << "" << endl;
    	usage();
        exit(1);
    }
    if (fileExists(argv[1]) == false) {
    	cerr << "" << endl
    		 << "The provided file either can't be opened or does not exist" << endl
    		 << "" << endl;
    	usage();
        exit(1);
    }
    if (isNumber(argv[2]) == false) {
    	cerr << "" << endl
    		 << "The argument provided for the amount of rounds is not an integer" << endl
    		 << "" << endl;
    	usage();
        exit(1);
    }
    else {
       	pagerank(argv);
    }
};