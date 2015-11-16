
/*
; Template File : Print the system name
; ====================================

;------------------------------------------------------------------------
; Author:  TESTTEST
;
; Project:   K-TEAM Project Management
; Goal:              This PRGM launches 2 processes.
;                    Process 0: "alive" it toggles the LED 0 every 500 ms.
;                    Process 1: it displays the ID of the system every 1 s.
;
;
;------------------------------------------------------------------------
*/

#include	<sys/kos.h>
#include 	<stdlib.h>
#include 	<stdio.h>
#include 	<math.h>
#include 	"stdafx.h"
#include	"Network.h"

#define		KNBSENSORS	15
#define		KNBMOTORS	2
#define		KSCALING	400
#define		KSPEED		20

#define		NET_INPUT	2

static int32 vIDProcess[3]; //change to match number of processes
uint32 sensBuf[KNBSENSORS];

/*
 * Process 0
 * ^^^^^^^^^
 *
 * This process publishes an association between "IR sens" and a Ptr
 * on the I.R. values.
 *
 */

static void
process_0 ()
{	
	printf("Process 0 Startup\n");
	int32 status;
	IRSENSOR *sensor;

	sensor = sens_get_pointer ();
	status = tim_define_association ("IR sens", (uint32 *) sensor);
	if (status < 0)
	{
			printf ("It is impossible to generate an IR sens association\r\n");
			exit (0);
    }
	//printf ("Sensors: %ld\n\r", status);
  exit (0);
}

/*
 * Process `
 * ^^^^^^^^^
 *
 * This process reads the IR sensor and then generates an event for process 1 to continue
 * every 1 s.
 *
 */

static void 
process_1 ()
{
	printf("Process 1 Startup\n");
	int32 status;
	IRSENSOR *sensor;

	
/* Wait for the IR sens association */

	do
    {
		status = tim_find_association ("IR sens");
    }
	while (status < 0);
	sensor = (IRSENSOR *) status;
  
	tim_generate_event();
	
	for (;;)
    {
      tim_wait_event(vIDProcess[2]);
	  tim_suspend_task (20);
	  
	  for (int i = 0; i < KNBSENSORS; i++){
		  sensBuf[i] = sensor->oProximitySensor[i]; //1020 seems to be max for IR sensor
		  //printf ("Sensors: %ld\n\r", sensBuf[i]);
	  }
	  tim_generate_event();
    }
}

/*
 * Process 2
 * ^^^^^^^^^
 *
 * This process sets up a network, then waits for process 1 to read in IR sensor
 *
 */


static void 
process_2 ()
{
  printf("PROCESS_2 Startup");
  
	double input[NET_INPUT];
	double output[2]; //temp, make sure it takes real output size
	
  //NetworkDimension = numberOfInputs + numberOfOutputs + numberOfInterNeurons; (default constructor is 2, 2, and 6 for 10)
	Network fred(2, 1, 2);
	double network [25]= {
		0.00,	0.00,	0.50,	0.50,	0.50,
		0.00,	0.00,	0.00,	-0.50,	0.50,
		0.00,	0.50,	0.00,	0.50,	0.50,
		0.00,	0.00,	0.00,	0.00,	0.00,
		0.00,	0.00,	0.00,	0.00,	0.00,
	};	fred.setNetworkWeightsFull(network);
	
	fred.PrintNetworkState();
	
	//Motor control starter, refer to manual
	mot_config_speed_1m (0, 3800, 800, 100);
	mot_config_speed_1m (1, 3800, 800, 100);
  
	for (;;){
		
		//for(int i= 0; i<NET_INPUT; i++){
		//	input[i]  =  1.0;
		//}
		
		//TEMPORARY TOSS IN
		 printf ("Sensors: %ld\n\r", sensBuf[2]);
		 printf ("Sensors: %ld\n\r", sensBuf[3]);
		 
		 //CURRENT ISSUE: Passing log(sensBuf[]) into network. Issue not repeating in network file alone
		
		double in1 = sensBuf[2]; 
		double in2 = sensBuf[3];
		input[0]= in1/1024; 
		input[1]= in2/1024; 
		
		double test = sensBuf[2];
		printf("Conversion %f\n\r", test/1024 );
		printf("INPUT 0: %f\n\r", input[0] ); 
		printf("INPUT 1: %f\n\r", input[1] ); 
		
		fred.setNetworkInput( input );
		fred.cycleNetwork();
		
		fred.getNetworkOuput(output);
		for(int i=0; i<=1; i++){
			printf("Output: %f\n\r", output[i]);
		}
		printf("\n");
		//fred.printNetworkOutput();
		
		//Motor control
		mot_new_speed_2m (int(output[0]*100), int(output[1]*100) ); //input values are temp
		//mot_new_speed_2m (100, 100 );
		//Event driven back and forth between IR sensor and network
		tim_generate_event(); //Alerts the IR process to begin 
		tim_wait_event(vIDProcess[1]); //Waits for the IR process to end
		//var_change_led (0);
	}

	printf("PROCESS_2 END  ");
	exit (0);
}


/*
* MAIN
* ====
*
* - Initialise the used managers: tim & bios are initialised at the start-up.
* - Launch all the processes.
* - Kill the "main". At this moment only the launched processes are executed.
*
*/

int
main (void)
{
  int32 status;

  /*
     --------------------------|------------------------------------------------|
   */

  static char prName_0[] = "User 0 process, EF-98                            Rev. 1.00\r\n";
  static char prName_1[] = "User 1 process, EF-98                            Rev. 1.00\r\n";
  static char prName_2[] = "User 2 process, EF-98                            Rev. 1.00\r\n";

  com_reset ();
  var_reset ();
  sens_reset ();
  str_reset ();
  mot_reset ();
  
  status = install_task (prName_0, 800, process_0);
  if (status == -1)
    exit (0);
  vIDProcess[0] = (uint32) status;

  status = install_task (prName_1, 800, process_1);
  if (status == -1)
    exit (0);
  vIDProcess[1] = (uint32) status;

  status = install_task (prName_2, 800, process_2);
  if (status == -1)
    exit (0);
  vIDProcess[2] = (uint32) status;

  
 return 0;
}