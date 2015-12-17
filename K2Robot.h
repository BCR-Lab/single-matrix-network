// RobotControl.h: interface for the RobotControl class
//
//////////////////////////////////////////////////////////////////////

#ifndef ROBOTCONTROL_H
#define ROBOTCONTROL_H

#include 	<math.h>
#include	<sys/kos.h>

#include 	"Network.h"
#include 	"stdafx.h"

class K2Robot
{
public:
	K2Robot();		// default constructor.
	K2Robot::K2Robot(double network[], int in_num, int out_num, int neurons_num, std::string sensor_name); //Constructor with specific network in mind
	K2Robot::K2Robot(double network[], int in_num, int out_num, int neurons_num, std::string sensor_name, int in_KP, int in_KI, int in_KD ); //Constructor with specific network in mind, with custom weights for motor control
	K2Robot::K2Robot(double network[], int in_num, int out_num, int neurons_num, std::string sensor_name, int in_KP, int in_KI, int in_KD, double outThresh, double learnRate, int refractState ); //Constructor where you define everything relevant

	virtual ~K2Robot();
	
	void feed_network(int *sensorChoice); //Specify in an array which sensors to use to feed into the network. NOTE: Will cut off if one is smaller than the other
	void feed_motors();
	void print_sensors(int start, int end);
	void set_motor_speed(int left, int right);

private:
    IRSENSOR *sensor;
	
	Network kheperaNet;
	
	double *input;
	double *output; 
	
	
	int num_inputs; //Number of inputs for the Network to handle
	int num_outputs; //Number of outputs for the Network to return
	int num_inter_neurons; //Number of interneurons
	
	
};

#endif 
