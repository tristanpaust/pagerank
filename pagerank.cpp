#include <fstream>
#include <mpi.h>
#include <iostream>
#include <vector>
#include <stdio.h>

using namespace std;

// Actual pagerank function
vector <double> pagerank(int r, vector< vector<double> > credit_ranking, int myrank, vector<int> processID, vector< vector<int> > adjacency_list, vector<int> degrees)
{   
    for(int id=0; id<adjacency_list.size(); id++) 
    {
        if(!adjacency_list[id].empty()) 
        {
            if(processID[id]==myrank) 
            {
                double c = 0.0f;
                for (int i=0; i < adjacency_list[id].size(); i++) 
                {
                    c = c + credit_ranking[r-1][adjacency_list[id][i]]/degrees[adjacency_list[id][i]];
                } credit_ranking[r][id] = c;
            }
        }
    } 
    return credit_ranking[r];
}

// Reads file input and stores it, calls pagerank, manages mpi updates, write output files
int main(int argc, char* argv[]) {
	int nprocs, myrank;

    double start_time_process, end_time_process;
	double start_read, end_read; 
   
	MPI_Init(&argc, &argv);
    MPI_Comm_size(MPI_COMM_WORLD, &nprocs);
    MPI_Comm_rank(MPI_COMM_WORLD, &myrank);

    start_time_process = MPI_Wtime();
    start_read = MPI_Wtime(); 

    ifstream first_file(argv[1]); 
    ifstream second_file(argv[2]);

    int rounds = 5;
    int highest_value;
    int value1, value2;
	int max;

    while (second_file >> highest_value) {
    	if (highest_value > max)
    		max = highest_value;
    }

    second_file.clear();
    second_file.seekg(0, ios::beg);

    vector< vector <int> > adjacency_list(max+1);

    while(first_file >> value1 >> value2) {
    	adjacency_list[value1].push_back(value2);
    	adjacency_list[value2].push_back(value1);
    }

    int value3, value4, value5;

    vector<int> processID(max+1);
    vector<int> degrees(max+1);

    // Make list to identify processes and degrees for every node
    while (second_file >> value3 >> value4 >> value5) {
            processID[value3]=value5;
            degrees[value3]=value4;  
    }

    // Stop reading and storing time and output it
    end_read = MPI_Wtime();
    cout << "Time to read and store input files @ process " << myrank << ": " << end_read - start_read <<endl;
    
    // Initialize vector that holds ranking
    vector< vector< double > > credit_ranking(rounds+1, vector<double>(max+1, 0.0f));
    for (int i = 0; i <= max; i++) {
        credit_ranking[0][i]=1.0f;
    }

    double round_start, round_end;
    vector<double> final_credit;
    for(int r = 1; r <= rounds; r++) {        
        // Start measuring time for one round
        round_start = MPI_Wtime();
        
        // Call pagerank funtion
        vector<double> process_credit = pagerank(r, credit_ranking, myrank, processID, adjacency_list, degrees);
        vector<double> world_credit(process_credit.size());

        // Update global list of credit rankings
        MPI_Allreduce(process_credit.data(), world_credit.data(), credit_ranking[1].size(), MPI_DOUBLE, MPI_SUM, MPI_COMM_WORLD);
        credit_ranking[r]=world_credit;
        
        // Stop total time and output it
        round_end = MPI_Wtime();
        
        cout << "Time for round " << r << " @ process " << myrank << ": " << round_end - round_start << endl;
    }

    cout << "Test credit, should be 0.00526958: " << credit_ranking[1][20] << endl;

    // Create output stream, iterate through rounds and credit values, write to file, close stream
    // Changed from C++ ostream to C fprintf for performance reasons
    FILE* process_output;
    string output_name = "pagerank.out." + to_string(myrank);
    process_output = fopen(output_name.c_str(), "w");

    for(int i = 0; i<adjacency_list.size(); i++) {
        if(processID[i] == myrank) {
            fprintf(process_output, "%d\t%d\t", i, degrees[i]);
            for (int j = 1; j<=rounds; j++) {
                fprintf(process_output, "\t%lf\t", credit_ranking[j][i]);
            }
            fprintf(process_output, "\n");
        }
    }
    fclose(process_output);

    //Stop total time
    end_time_process = MPI_Wtime();

    // Output total time to complete this process
    cout << "Total time @ process " << myrank << ": " << end_time_process - start_time_process << endl;
    
    // Terminate process
    MPI_Finalize();
    return 0;
}