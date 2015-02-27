// Network.h: interface for the Network class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_NETWORK_H__8E7C932B_D833_4E1F_9EDC_ED09AFCF876A__INCLUDED_)
#define AFX_NETWORK_H__8E7C932B_D833_4E1F_9EDC_ED09AFCF876A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#define MAX_NET_DIMENSION 100
#define MAX_NET_INPUTS 10		// Cannot exceed the size of the net
#define MAX_NET_OUTPUTS 10		// Cannot exceed the size of the net
#define MAX_DUMMY_STRING_LENGTH 30

class Network  
{
public:
	Network();								// default network constructor.
	Network::Network( char * file_name );	// Construct the network from a stored file.
	Network::Network(int inputs, int interneurons, int outputs, char * file_name ); // construct a blank network to user specified size and write it to a file for later editing
	virtual ~Network();

	// Members---------------------
	// Additions to the members should be added to the read and write netork functions

private:
	int numberOfInputs;
	int numberOfOutputs;
	int numberOfInterNeurons;
	int networkDimension;

	double neuronActivation[MAX_NET_DIMENSION];		// Individual neurons have individual activation levels that are tracked through timesteps an are not visibile as output ( transformed to output by a function)
	double neuronOutput[MAX_NET_DIMENSION];			// The output of individual neurons, used as inputs to the other neurons in the network throught the connection weights matrix
	double neuronThresholds[MAX_NET_DIMENSION];		// Individual Neurons each have a speficifed activation threshold
	double neuronLearningRate[MAX_NET_DIMENSION];	// Individual Neurons each have a speficifed learning rate -- rate of change of connection strength per time step
	short int neuronRefractoryState[MAX_NET_DIMENSION];	// Individual Neurons each have a speficifed period during which output is blocked -- should be 0 or greater.
	double neuronWeightTotal[MAX_NET_DIMENSION];	// Individual Neurons each have a speficifed total weight strength in their input connections.
	double networkWeights[MAX_NET_DIMENSION*MAX_NET_DIMENSION];
	double networkInputs[MAX_NET_INPUTS];
	double networkOutputs[MAX_NET_OUTPUTS];
	short int  plasticWeightsMask[MAX_NET_DIMENSION*MAX_NET_DIMENSION]; // a filter. Plastic weights are = 1, fixed = 0. THis allows for the specification of some fixed and some plastic weights in the same neuron. This could be a binary array ( type bool) to save space. 

	// Functions -------------------------

	Network::instantiateDefaultNetwork( void );
	Network::setNetworkOuput( void );
	Network::copyNeuronActivationsToNeuronOutputs( void );
	Network::copyNetworkInputsToInputNeuronOutputs( void );
	Network::thresholdNeuronOutputs( void );
	//******Network::squashNeuronOutputs( void );
	Network::squashNeuronOutputs( double offset, double expSlope);
	Network::setNeuronOutput( double value );
	Network::setNeuronThresholds( double value );
	Network::setNeuronLearningRate( double value );
	Network::setNeuronRefractoryState( int value );
	Network::setPlasticWeightsMask( short int value ); // in general it is good to set this to 1 and let the learning rate determine plasticity.  This is to be used for special cases
	Network::setNeuronActivation( double value );
	Network::setNetworkOutputs( double value );
	Network::networkActivation( void  );
	Network::hebbianWeightUpdate( void  );
	Network::hebbianExcitatoryWeightUpdate( void );
	Network::hebbianInhibitoryWeightUpdate( void );
	Network::normalizeNeuronWeights( void );			// Update weight totals to neuron-specific values
	Network::normalizeNeuronWeights( double value );	// Uptdate weight totals to specificed values
	Network::normalizeNonDiagonalNeuronWeights( void );
	Network::normalizeNonDiagonalInhibitoryNeuronWeights( void );
	Network::normalizeNonDiagonalExcitatoryNeuronWeights( void );
	Network::setNeuronWeightTotal( double value);
	int Network::computeWeightIndex( int source_neuron_number, int target_neuron_number );

public:
	Network::cycleNetwork( void );
	Network::cycleNetworkSquash(  double offset, double expSlope );
	Network::cycleNetworkNormalizeHebbianLearning( void );
	Network::printNetworkOuput( void );
	Network::printNetworkOutputState( void );
	Network::setNetworkWeightsDiagonalRange( double value, int start_row_col, int end_row_col );
	Network::setNetworkWeightsUpperLowerTriangleAndDiagonal( double diagonal_value, double upper_triangle_value, double lower_triangle_value);
	Network::setNetworkWeightsRectangle( double value, int start_row, int end_row, int start_column, int end_column );
	Network::setNetworkWeightsUpperTriangle( double value, int start_row, int end_row, int start_column, int end_column );
	Network::setNetworkWeightsLowerTriangle( double value, int start_row, int end_row, int start_column, int end_column );
	Network::writeNetworkOutputStateToFile( char * file_name );
	Network::writeNetworkActivationStateToFile( char * file_name );
	Network::writeNetworkWeightsToFile( char * file_name );
	Network::setNetworkInput( double *vector);
	Network::getNetworkOuput( double * vector );
	Network::readNetworkFromFile( char * file_name );
	Network::writeNetworkToFile( char * file_name );
	Network::setNetworkWeights( double value );
	Network::PrintNetworkState( void);

};

#endif // !defined(AFX_NETWORK_H__8E7C932B_D833_4E1F_9EDC_ED09AFCF876A__INCLUDED_)
