#include "minimal_turtlebot/turtlebot_controller.h"
#include <cmath>
#include <iostream>
using namespace std;
int nearest;

void turtlebot_controller(turtlebotInputs turtlebot_inputs, uint8_t *soundValue, float *vel, float *ang_ve){
	int count = 0;
	if (turtlebot_inputs.ranges[count]>=0 && count<640 && turtlebot_inputs.ranges[count+1]<turtlebot_inputs.ranges[count])
	{
		nearest = count;
	}
	cout<<nearest<<endl;
}