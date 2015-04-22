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

#ifndef M_PI
// Not provided by MS Visual C++
#define M_PI					3.14159265358979323846	/* pi */
#endif

// Number of oscillations per time step
#define ORDINARY_FREQUENCY		0.02					/* 1 oscillation every 50 time steps */

// How far should each input be shifted from the previous?
#define PHASE_SHIFT_PER_INPUT	0.1

// Number of sine wave oscillations to loop through
#define NUM_OSCILLATIONS		3

// Number of time steps it takes to go through a single oscillation
#define PERIOD					1 / ORDINARY_FREQUENCY

void doLearning(Network fred, std::string prefix);

/*
 * Convert values to strings. Needed because std::to_string is apparently missing
 * from <string>.
 */
template <typename T>
std::string to_string(T value)
{
	//create an output string stream
	std::ostringstream os ;

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

	std::string network_file_name;
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

	std::string input_file_name;
	if (argc > 2) {
		input_file_name = argv[2];
	}

	const int numNeuronsToUpdate = 12;
	const int updateNeurons[][2] = {{15,10},{15,11},{16,9},{16,11},{17,9},{17,10},{18,13},{18,14},{19,12},{19,14},{20,12},{20,13}};
	//const double weights[] = { 0.0, -0.1, -0.2, -0.35, -0.5, -0.65, -0.9, -1.25 };
	const int numWeights = 1;
	const double weights[] = { -.35 };

	double* beforeWeights = copyMatrix(fred.getNetworkWeights(), fred.getNetworkDimension());

	int weightNum;
	for (weightNum = 0; weightNum < numWeights; weightNum++) {
		int pairNum;
		double inhibWeight = weights[weightNum];
		printf("Using inhibitory weights of %.2f\n", inhibWeight);
		
		for (pairNum = 0; pairNum < numNeuronsToUpdate; pairNum++) {
			fred.updateWeight(updateNeurons[pairNum][0], updateNeurons[pairNum][1], inhibWeight);
		}
		fred.resetNeuronOutputs();

		doLearning(fred, "bilateral-" + to_string(inhibWeight));
		fred.PrintNetworkState();
	}

	printf("\nLearning changes:\n");
	printDifferences(beforeWeights, fred.getNetworkWeights(), fred.getNetworkDimension());

	return 0;
}

void doLearning(Network fred, std::string prefix) {
	const int numInputs = fred.getNumInputs();
	double* input = new double[numInputs];

	const std::string output_file = prefix + "-output_squash.txt";
	const std::string input_file = prefix + "-in.txt";

	int t;

	// Delete files
	std::remove(output_file.c_str());
	std::remove(input_file.c_str());

	printf("*** Begin network learning ***\n");
	const double angularFrequency = 2.0 * M_PI * ORDINARY_FREQUENCY;

	// Begin at the middle of the sine wave, and continue for the number of oscillations requested
	for (t = 0; t < NUM_OSCILLATIONS * PERIOD; t++) {
		//-----------------------------------------------------------
		int inputNum;
		for(inputNum = 0; inputNum < numInputs; inputNum++) {

			const double phase = inputNum * PHASE_SHIFT_PER_INPUT;

			// Offset the time by half a period to start on the down stroke
			input[inputNum] = sin((t + PERIOD / 2) * angularFrequency + phase);
			//printf("[%d], phase=%f, freq=%f, input=%f\n", inputNum, phase, angularFrequency, input[inputNum]);
		}
		//---------------------------------------------------------------------

		printf("t=%03d: ", t);
		fprintf(fred.getLogFile(), "-- t=%03d --\n", t);

		fred.setNetworkInput( input );

		fred.cycleNetwork();

		fred.cycleNetworkNormalizeHebbianLearning();

//		fred.printNetworkOuput();
		fred.printNetworkOutputState( );

		fred.writeNetworkInputToFile(input_file);
		fred.writeNetworkOutputStateToFile(output_file);

		fred.writeNetworkToFile(prefix + "-out.txt");
		//fred.writeNetworkWeightsToFile(prefix + "-weights.txt");
	}
	printf("*** End network learning ***\n");
}
