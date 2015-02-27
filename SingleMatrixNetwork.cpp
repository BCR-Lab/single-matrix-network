// SIngleMatrixNetwork.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include "Network.h"
#include <math.h>
#include <stdio.h>

int main(int argc, char* argv[])
{
	double input[6];
	int i,j;

//	Network ted(2,16,2,"blank_network.txt");
//	ted.writeNetworkToFile("this_is_mine.txt");

//	Network fred;
	Network fred("ganglia5.txt");

	fred.PrintNetworkState();


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

	fred.writeNetworkOutputStateToFile( "output_squash.txt" );

	i = 0;
	while( i < 100 ){
		//-----------------------------------------------------------
		for(j = 0; j < 6; ++j ){	// temporary input function (STEP RESPONSE)
			input[j]  =  sin((i + j*3)*0.1);
//			input[j]  = 1.0;
		}
		//---------------------------------------------------------------------

		fred.setNetworkInput( input );

		fred.cycleNetwork();

		fred.cycleNetworkNormalizeHebbianLearning();

//		fred.printNetworkOuput();
		fred.printNetworkOutputState( );

		fred.writeNetworkOutputStateToFile( "output_squash.txt" );

		fred.writeNetworkToFile("ganglia3.txt");
		fred.writeNetworkWeightsToFile("weights.txt");
		++i;
	}

	//Test the read-write fucntions -------------
	fred.writeNetworkToFile("temp.txt");
//	fred.readNetworkFromFile("temp.txt");

	return 0;
}
