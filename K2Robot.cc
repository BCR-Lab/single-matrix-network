
/*
; K2Robot: Class implementation of khepera II Robot Controller
; ====================================
; Author: Stephan Kritikos
*/

#include 	<stdlib.h>
#include 	<stdio.h>

#include	"K2Robot.h"

//Defines the output that be interpreted as negative motor movement. Has to be between 0 and 1, takes everything from the threshold to the value
#define		NET_NEG				0.4
//Defines the output that will be completely ignored (to allow the robot to move forward instead of just spin) Note that this is applied to the output and if a threshold was already set in network initilization, this can be pointless
#define		NET_THRESHOLD		0.0
//Defines the output that will cause the motors to reverse
#define		NET_REVERSE			0.90
//Define the standard movement speed (between 0 to 60)
#define		BASE_SPEED			15
//Defines the reverse speed for the motors (between -60 and 0)
#define		NEG_SPEED			-30

//Defines the total number of sensors on the robot. Bare in mind that the thing has no pushing strength. If it's against a wall, that's a problem
#define		KNBSENSORS			15

//Defines the default settings for network and motor control:
#define		KP					3800
#define		KI					800
#define		KD					100

#define		STACK_SIZE			800

//BE SURE TO ADJUST THE NETWORK IN THE DEFAULT CONSTUCTOR IF CHANGED, THESE ARE DEFAULT NET VALUES
#define		NET_INPUT			2
#define		NET_OUTPUT			2
#define		NET_INTER_NEURONS	1

/*
Constructor
=======================
Default uses 5x5 matrix, sent into Network. 2 inputs, 2 outputs, 1 interneuron and no additional weights. Handles the IR sensor association naming.
Refer to the network file itself for more details.
*/
K2Robot::K2Robot() : kheperaNet(NET_INPUT, NET_INTER_NEURONS, NET_OUTPUT)
{
	printf("K2Robot Controller Constructor Initialized\n\r");
	
	int32 status;
	
	com_reset ();
	var_reset ();
	sens_reset ();
	str_reset ();
	mot_reset ();
	
	status = tim_define_association ("IR sens", (uint32 *) sens_get_pointer ()); //Defines association to IR to "IR sens"
	if (status < 0)
	{
		printf ("It is impossible to generate an IR sens association\r\n");
		exit (0);
    }
	sensor = (IRSENSOR *) tim_find_association ("IR sens"); //finds the pointer associated with "IR sens" and defines sensor to that
	
  //NetworkDimension = numberOfInputs + numberOfOutputs + numberOfInterNeurons; (default constructor in Network.cc (Not the one here in K2Robot) is 2 in, 2 out, and 6 inter for 10 total)
	double network [(NET_INPUT + NET_OUTPUT + NET_INTER_NEURONS)*(NET_INPUT + NET_OUTPUT + NET_INTER_NEURONS)]= 
	{
		0.00,	0.00,	0.50,	0.50,	0.50,
		0.00,	0.00,	0.00,	-0.50,	0.50,
		0.00,	0.50,	0.00,	0.50,	0.50,
		0.00,	0.00,	0.00,	0.00,	0.00,
		0.00,	0.00,	0.00,	0.00,	0.00,
	};	kheperaNet.setNetworkWeightsFull(network);
	
	kheperaNet.PrintNetworkState();
	
	input = new double[NET_INPUT];
	output = new double[NET_OUTPUT];
	num_inputs = NET_INPUT;
	num_outputs = NET_OUTPUT;
	num_inter_neurons = NET_INTER_NEURONS;
	
	
	//Motor control starter. KP for Porportional Weight (Linear adjustment to input), KI for intergal (Adjustment based on change history), KD (adjustment to error history)
	mot_config_speed_1m (0, KP, KI, KD);
	mot_config_speed_1m (1, KP, KI, KD);
	
};

/*
Constructor
=======================
Takes an array of doubles containing the setup for your network. Refer to the network file itself for more details

NOTE: in_num, out_num, neurons_num are not initial inputs/outputs, but the number you expect to send and receive.
NOTE: When changing outputs, 0 and 1 are treated as the motor inputs
*/
K2Robot::K2Robot(double network[], int in_num, int out_num, int neurons_num, std::string sensor_name) : kheperaNet(in_num, neurons_num, out_num)
{
	printf("K2Robot Controller Constructor Initialized (Custom network)\n\r");
	
	int32 status;
	
	com_reset ();
	var_reset ();
	sens_reset ();
	str_reset ();
	mot_reset ();
	
	status = tim_define_association (sensor_name, (uint32 *) sens_get_pointer ()); //Defines association to IR
	if (status < 0)
	{
		printf ("It is impossible to generate an IR sens association\r\n");
		exit (0);
    }
	sensor = (IRSENSOR *) tim_find_association (sensor_name); //finds the pointer associated with "IR sens" and defines sensor to that
	
  //NetworkDimension = numberOfInputs + numberOfOutputs + numberOfInterNeurons; (default constructor in Network.cc (Not the one here in K2Robot) is 2 in, 2 out, and 6 inter for 10 total)
	kheperaNet.setNetworkWeightsFull(network);
	kheperaNet.PrintNetworkState();
	
	input = new double[in_num];
	output = new double[out_num];
	num_inputs = in_num;
	num_outputs = out_num;
	num_inter_neurons = neurons_num;
	
	//Motor control starter. KP for Porportional Weight (Linear adjustment to input), KI for intergal (Adjustment based on change history), KD (adjustment to error history)
	mot_config_speed_1m (0, KP, KI, KD);
	mot_config_speed_1m (1, KP, KI, KD);

};

/*
Constructor
=======================
Takes an array of doubles containing the setup for your network. Refer to the network file itself for more details
Takes int variables in_KP, in_KI, in_KD to set respective weights on motor control.
	KP for Porportional Weight (Linear adjustment to input), KI for intergal (Adjustment based on change history), KD (adjustment to error history)
	
NOTE: in_num, out_num, neurons_num are not initial inputs/outputs, but the number you expect to send and receive.
NOTE: When changing outputs, 0 and 1 are treated as the motor inputs
*/
K2Robot::K2Robot(double network[], int in_num, int out_num, int neurons_num, std::string sensor_name, int in_KP, int in_KI, int in_KD )  : kheperaNet(in_num, neurons_num, out_num)
{
	printf("K2Robot Controller Constructor Initialized (Custom network and Motor Weights)\n\r");
	
	int32 status;
	
	com_reset ();
	var_reset ();
	sens_reset ();
	str_reset ();
	mot_reset ();
	
	status = tim_define_association (sensor_name, (uint32 *) sens_get_pointer ()); //Defines association to IR to "IR sens"
	if (status < 0)
	{
		printf ("It is impossible to generate an IR sens association\r\n");
		exit (0);
    }
	sensor = (IRSENSOR *) tim_find_association (sensor_name); //finds the pointer associated with "IR sens" and defines sensor to that
	
  //NetworkDimension = numberOfInputs + numberOfOutputs + numberOfInterNeurons; (default constructor in Network.cc (Not the one here in K2Robot) is 2 in, 2 out, and 6 inter for 10 total)
	kheperaNet.setNetworkWeightsFull(network);
	kheperaNet.PrintNetworkState();
	
	input = new double[in_num];
	output = new double[out_num];
	num_inputs = in_num;
	num_outputs = out_num;
	num_inter_neurons = neurons_num;
	
	
	//Motor control starter. KP for Porportional Weight (Linear adjustment to input), KI for intergal (Adjustment based on change history), KD (adjustment to error history)
	mot_config_speed_1m (0, in_KP, in_KI, in_KD);
	mot_config_speed_1m (1, in_KP, in_KI, in_KD);

};

/*
Constructor, full important network definitions and robot definition
=======================
Takes an array of doubles containing the setup for your network. Refer to the network file itself for more details
Takes int variables in_KP, in_KI, in_KD to set respective weights on motor control.
	KP for Porportional Weight (Linear adjustment to input), KI for intergal (Adjustment based on change history), KD (adjustment to error history)
Takes values that define the thresholds for output, learning rate, refractory state

NOTE: in_num, out_num, neurons_num are not initial inputs/outputs, but the number you expect to send and receive.
NOTE: When changing outputs, 0 and 1 are treated as the motor inputs
*/
K2Robot::K2Robot(double network[], int in_num, int out_num, int neurons_num, std::string sensor_name, int in_KP, int in_KI, int in_KD, double outThresh, double learnRate, int refractState )  : kheperaNet(in_num, neurons_num, out_num, outThresh, learnRate, refractState)
{
	printf("K2Robot Controller Constructor Initialized (Custom network and Motor Weights)\n\r");
	
	int32 status;
	
	com_reset ();
	var_reset ();
	sens_reset ();
	str_reset ();
	mot_reset ();
	
	status = tim_define_association (sensor_name, (uint32 *) sens_get_pointer ()); //Defines association to IR to "IR sens"
	if (status < 0)
	{
		printf ("It is impossible to generate an IR sens association\r\n");
		exit (0);
    }
	sensor = (IRSENSOR *) tim_find_association (sensor_name); //finds the pointer associated with "IR sens" and defines sensor to that
	
  //NetworkDimension = numberOfInputs + numberOfOutputs + numberOfInterNeurons; (default constructor in Network.cc (Not the one here in K2Robot) is 2 in, 2 out, and 6 inter for 10 total)
	kheperaNet.setNetworkWeightsFull(network);
	kheperaNet.PrintNetworkState();
	
	input = new double[in_num];
	output = new double[out_num];
	num_inputs = in_num;
	num_outputs = out_num;
	num_inter_neurons = neurons_num;
	
	
	//Motor control starter. KP for Porportional Weight (Linear adjustment to input), KI for intergal (Adjustment based on change history), KD (adjustment to error history)
	mot_config_speed_1m (0, in_KP, in_KI, in_KD);
	mot_config_speed_1m (1, in_KP, in_KI, in_KD);

};

K2Robot::~K2Robot()
{
	delete[] input;
	delete[] output;
	delete sensor;
	printf("K2Robot Controller Destroyed\n\r");
};

/*
Function: feed_network
=======================
Feeds input into the network. Assumes input is some list of which sensors you want to use to feed into the network. Can be multiple of the same sensor
It will not take more sensor readings than there are inputs in the network, which is culled by just cutting off the loop
*/
void K2Robot::feed_network(int *sensorChoice)
{
	for (int i = 0; i < num_inputs; i++) //Reads in sensor data, passes it into network. NOTE: Manual says this process takes 20ms
	{
		input[i] = double(sensor->oProximitySensor[sensorChoice[i]]) / 1024.0; //divides it down to be between 0 and 1, could use to be improved
		printf("INPUT %i: %f\n\r", i, input[i] ); 
	}
	
	kheperaNet.setNetworkInput(input);
	kheperaNet.cycleNetwork();
}

/*
Function: feed_motors
=======================
Feeds the network output into the motor control
*/
void K2Robot::feed_motors()
{
	kheperaNet.getNetworkOuput(output);
	
	//Is the output < than the NET_THRESHOLD? If yes, is it in the NET_NEG range?
	//If yes, is it greater than NET_REVERSE? If yes, than it sets the speed to the NEG_SPEED. If it is lower than the negeative threshold, it moves forward based on network output. 
	//If it is not greater than NET_THRESHOLD, it defaults to a base speed to prevent spinning

	mot_new_speed_2m (	output[1] < NET_REVERSE || output[0] < NET_REVERSE ? ( output[1] > NET_THRESHOLD ? (output[1] > NET_NEG ? int(output[1]*60) : int(output[1]* -60 * (1/NET_NEG) )): BASE_SPEED) : NEG_SPEED,
						output[1] < NET_REVERSE || output[0] < NET_REVERSE ? ( output[0] > NET_THRESHOLD ? (output[0] > NET_NEG ? int(output[0]*60) : int(output[0]* -60 * (1/NET_NEG) )): BASE_SPEED) : NEG_SPEED); // RIGHT, LEFT, max is 60, min is -60
	
	
	printf("Motor Speed: L: %li R: %li\n\r", mot_get_speed(0), mot_get_speed(1));	
	for(int i=0; i<num_outputs; i++)
	{
		printf("Output: %f\n\r", output[i]);
	}
};


/*
Function: print_sensors
=======================
Prints the current readings from the sensors from your choice of start to finish
Not rocket science
*/
void K2Robot::print_sensors(int start, int end)
{
	for(int i = start; i <= end; i++)
	{
		printf ("Sensor %i: %f\n\r", i, double(sensor->oProximitySensor[i]) );
	}
};

/*
Function: set_motor_speed
=======================
Set function to control motor speed. 
Right now it contains nothing that couldn't be done outside the network, but it's here for expansion
*/
void K2Robot::set_motor_speed(int left, int right)
{
	mot_new_speed_2m (right, left);
};


