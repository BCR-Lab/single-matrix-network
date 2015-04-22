// SIngleMatrixNetwork.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Network.h"
#include <math.h>
#include <stdio.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
using namespace std;

void doLearning(Network fred, string input_file_name, string prefix);

/*
 * Convert values to strings. Needed because std::to_string is apparently missing
 * from <string>.
 */
template <typename T>
std::string to_string(T value)
{
	//create an output string stream
	ostringstream os ;

	//throw the value into the string stream
	os << value ;

	//convert the string stream into a string and return
	return os.str() ;
}

int main(int argc, char* argv[])
{

//	Network ted(2,16,2,"blank_network.txt");
//	ted.writeNetworkToFile("this_is_mine.txt");

//	Network fred;

//	Network fred("ganglia5.txt");

	string network_file_name;
    if (argc < 2) {
		network_file_name = "ganglia5.txt";
	} else {
		network_file_name = argv[1];
	}

	Network fred(network_file_name);

	fred.PrintNetworkState();
	printf("\n");

	// Initialize the weights to produce a test recurrent network
/*
	fred.setNetworkWeights(0.0);					// initialize all connections to zero
	fred.setNetworkWeightsRectangle(0.3,3,7,0,3);	// set feedforward connections from inputs to interneurons 
	fred.setNetworkWeightsRectangle(0.5,7,9,3,7);	// set feedforward connections from interneurons to output neurons 
	fred.setNetworkWeightsRectangle(-0.25,3,7,3,7);	// set recurrent connections from interneurons to interneurons
	fred.setNetworkWeightsDiagonalRange(0.1,0,9);	// set network autapses
  */

/*	
	//--- Identity Matrix ----
	fred.setNetworkWeights(0.0);
	fred.setNetworkWeightsDiagonalRange(1,0,8);
*/

	fred.writeNetworkOutputStateToFile(network_file_name + "-output_squash.txt" );

	string input_file_name;
	if (argc > 2) {
		input_file_name = argv[2];
		cout << "Reading inputs from " << input_file_name << endl;
	}

	fred.resetNeuronOutputs();

	doLearning(fred, input_file_name, network_file_name);
	fred.PrintNetworkState();

	return 0;
}

/*
 * Process each line in the input_file_name through the network
 */
void doLearning(Network fred, string input_file_name, string prefix) {
	const int num_inputs = fred.getNumInputs();
	double* input = new double[num_inputs];

	const string output_file_name = prefix + "-output_squash.txt";

	int i = 0;
	int t = 0;

	printf("*** Begin network learning ***\n");

	string input_line;
	ifstream input_file(input_file_name);
	if (!input_file.is_open()) {
		cerr << "Error opening input file " << input_file_name << endl;
	} else {
		while (getline(input_file, input_line)) {
			//cout << "Inputs: " << input_line << endl;

			// Load inputs into input array
			const char* line = input_line.c_str();
			int input_num = 0;
			for (i = 0; i < input_line.length(); i += 2) {
				sscanf(line, "%lf", &input[input_num]);
			}

			printf("t=%03d: ", t);
			fprintf(fred.getLogFile(), "-- t=%03d --\n", t);

			fred.setNetworkInput( input );

			fred.cycleNetwork();

			fred.cycleNetworkNormalizeHebbianLearning();

	//		fred.printNetworkOuput();
			fred.printNetworkOutputState( );

//			fred.writeNetworkInputToFile(input_file_name);
			fred.writeNetworkOutputStateToFile(output_file_name);

			fred.writeNetworkToFile(prefix + "-out.txt");
			//fred.writeNetworkWeightsToFile(prefix + "-weights.txt");

			t++;
		}
	}
	printf("*** End network learning ***\n");
}
