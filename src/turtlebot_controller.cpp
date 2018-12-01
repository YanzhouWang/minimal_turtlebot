#include "minimal_turtlebot/turtlebot_controller.h"
#include <cmath>
#include <iostream>
using namespace std;

#define VEL 0.15
#define ANGULAR_VEL 0.5
#define PI 3.1415926

static uint64_t timer_start;
//uint64_t backward_time = 2e9;
uint64_t rotation_time = 2e9;

int Bumper_state = 0, Bumper_state_previous;
int Cliff_state = 0;
int WheelDrop_state = 0;
int IMU_state = 0;
double alpha;

//camera info: ranges[638] points directly forward

//debugging variables
int nearest;
int count = 0;

void turtlebot_controller(turtlebotInputs turtlebot_inputs, uint8_t *soundValue, float *vel, float *ang_vel)
{
	//START MOVING
	*vel = VEL;

	//BUMPER STATE CHECK
	if (turtlebot_inputs.leftBumperPressed)
	{
		Bumper_state = 1;
		timer_start = turtlebot_inputs.nanoSecs;
	}
	else if (turtlebot_inputs.centerBumperPressed)
	{
		Bumper_state = 2;
		timer_start = turtlebot_inputs.nanoSecs;

	}
	else if (turtlebot_inputs.rightBumperPressed)
	{
		Bumper_state = 3;
		timer_start = turtlebot_inputs.nanoSecs;

	}
	//cout << Bumper_state << endl;

	switch (Bumper_state) {
	case 1:
		*vel = 0;
		*vel = -VEL;
		*ang_vel = -ANGULAR_VEL;
		if (turtlebot_inputs.nanoSecs - timer_start >= rotation_time)
		{
			*ang_vel = 0;
			*vel = 0;
			Bumper_state = 0;
			Bumper_state_previous = 1;
		}
		//cout << "LEFT" << endl;
		break;

	case 2:
		*vel = 0;
		*vel = -VEL;
		if (Bumper_state_previous == 1)
		{
			*ang_vel = -ANGULAR_VEL;

		}
		else if (Bumper_state_previous == 3)
		{
			*ang_vel = ANGULAR_VEL;
		}
		else {
			*ang_vel = ANGULAR_VEL;
		}

		if (turtlebot_inputs.nanoSecs - timer_start >= rotation_time)
		{
			*vel = 0;
			*ang_vel = 0;
			Bumper_state = 0;
		}
		//cout << "CENTER" << endl;
		break;

	case 3:
		*vel = 0;
		*vel = -VEL;
		*ang_vel = ANGULAR_VEL;
		if (turtlebot_inputs.nanoSecs - timer_start >= rotation_time)
		{
			*ang_vel = 0;
			*vel = 0;
			Bumper_state = 0;
			Bumper_state_previous = 3;
		}
		//cout << "RIGHT" << endl;
		break;
	}


	//CLIFF STATE CHECK
	if (turtlebot_inputs.sensor0State)
	{
		Cliff_state = 1;
		timer_start = turtlebot_inputs.nanoSecs;
	}
	else if (turtlebot_inputs.sensor1State)
	{
		Cliff_state = 2;
		timer_start = turtlebot_inputs.nanoSecs;

	}
	else if (turtlebot_inputs.sensor2State)
	{
		Cliff_state = 3;
		timer_start = turtlebot_inputs.nanoSecs;

	}

	switch (Cliff_state) {
	case 1:
		*vel = 0;
		*vel = -VEL;
		*ang_vel = -ANGULAR_VEL;
		if (turtlebot_inputs.nanoSecs - timer_start >= rotation_time)
		{
			*vel = 0;
			*ang_vel = 0;
			Cliff_state = 0;
		}
		//cout << "LEFT" << endl;
		break;

	case 2:
		*vel = 0;
		*vel = -VEL;
		*ang_vel = ANGULAR_VEL;
		if (turtlebot_inputs.nanoSecs - timer_start >= rotation_time)
		{
			*vel = 0;
			*ang_vel = 0;
			Cliff_state = 0;
		}
		//cout << "CENTER" << endl;
		break;

	case 3:
		*vel = 0;
		*vel = -VEL;
		*ang_vel = ANGULAR_VEL;
		if (turtlebot_inputs.nanoSecs - timer_start >= rotation_time)
		{
			*vel = 0;
			*ang_vel = 0;
			Cliff_state = 0;
		}
		//cout << "CENTER" << endl;
		break;
	}


	//DROP STATE CHECK
	if (turtlebot_inputs.leftWheelDropped || turtlebot_inputs.rightWheelDropped)
	{
		WheelDrop_state = 1;
	}

	switch (WheelDrop_state) {
	case 1:
		cout << "wheel dropped" << endl;
		*vel = 0;
		*ang_vel = 0;
		*soundValue = 4;
	}


	//IMU STATE CHECK
	alpha = PI / 180 * abs(atan2(turtlebot_inputs.linearAccelZ, sqrt((turtlebot_inputs.linearAccelX * turtlebot_inputs.linearAccelX) + (turtlebot_inputs.linearAccelY * turtlebot_inputs.linearAccelY))));
	if (alpha >= 20)
	{
		*vel = 0;
		*ang_vel = 0;
		*soundValue = 4;
	}
	

	//CAMERA INPUT CHECK
	/*for (int i = 0; i < 640; ++i)
	{
		if (turtlebot_inputs.ranges[i] >= 0 && turtlebot_inputs.ranges[i] < turtlebot_inputs.ranges[i + 1])
		{
			cout << i << endl;
		}
	}
	cout<<turtlebot_inputs.ranges[638]<<endl;*/
}