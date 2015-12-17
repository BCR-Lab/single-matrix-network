
/*
; RobotDriver: Main driver for robot, initializes a Khepera II robot controller using the class RobotControl
; ====================================

;------------------------------------------------------------------------
; Author:  Stephan Kritikos
;
; Project:   Khepera Net
; Goal:		 Initializes a robot controller that uses the Network class, using the K2Robot class
;					  
; 
;
;
;------------------------------------------------------------------------
*/

#include 	<stdlib.h>
#include 	<stdio.h>

#include 	"stdafx.h"
#include	"K2Robot.h"

#define		KP					3800
#define		KI					800
#define		KD					100

#define		NET_INPUT			2
#define		NET_OUTPUT			2
#define		NET_INTER_NEURONS	1

#define		IR_name				"IR sens" //name for IR association. Doesn't matter what you name it, but enjoy the option to do so


/*
MAIN
====

Ignoring Khepera II process handling. Too many scheduler pains

Refer to K2Robot class for khepera II controls


*/

int main (void)
{
	//Ripped from ganglia3.txt, not made for this robot, just to test
	std::string name = IR_name;	
	/*
	double networkGanglia [(NET_INPUT + NET_OUTPUT + NET_INTER_NEURONS)*(NET_INPUT + NET_OUTPUT + NET_INTER_NEURONS)]= 
	{
		0.100000, 0.000000, 1.000000, 0.681766, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 
		0.000000, 0.100000, 0.000000, 0.318234, 1.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 
		0.000000, 0.000000, 0.100000, 0.000000, 0.000000, 1.000000, 0.000000, 0.000000, 0.750000, 0.000000, 
		0.000000, 0.000000, 0.000000, 0.100000, 0.000000, 0.000000, 1.000000, 0.000000, 0.250000, 0.250000, 
		0.000000, 0.000000, 0.000000, 0.000000, 0.100000, 0.000000, 0.000000, 1.000000, 0.000000, 0.750000, 
		0.000000, 0.000000, 0.000000,-0.500000,-0.500000, 0.100000, 0.000000, 0.000000, 0.000000, 0.000000, 
		0.000000, 0.000000,-0.500000, 0.000000,-0.500000, 0.000000, 0.100000, 0.000000, 0.000000, 0.000000, 
		0.000000, 0.000000,-0.500000,-0.500000, 0.000000, 0.000000, 0.000000, 0.100000, 0.000000, 0.000000, 
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.100000, 0.000000, 
		0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.000000, 0.100000 
	};
	*/
	//My basic test network
	double network [(NET_INPUT + NET_OUTPUT + NET_INTER_NEURONS)*(NET_INPUT + NET_OUTPUT + NET_INTER_NEURONS)]= 
	{
		0.00,	0.00,	0.50,	0.50,	0.50,
		0.00,	0.00,	0.00,	-0.50,	0.50,
		0.00,	0.50,	0.00,	0.50,	0.50,
		0.00,	0.00,	0.00,	0.00,	0.00,
		0.00,	0.00,	0.00,	0.00,	0.00,
	};
	K2Robot robot(network, NET_INPUT, NET_OUTPUT, NET_INTER_NEURONS, name, KP, KI, KD, 0.07, 0.0, 0);
	
	int sensorChoice[2] = {2,3};
	
	//for (int j = 0; j<10; j++)
	for (;;)
	{
		robot.feed_network(sensorChoice);
		robot.feed_motors();
		
		printf("\n\r");
		//var_change_led (0);
	}

 printf("MAIN END\n\r");
 exit (0);
 return 0;
}