#include "State.h"
#include <Arduino.h>
State::State()
{
	previousReturnValues[0] = 0;
	previousReturnValues[1] = 0;
	previousReturnValues[2] = 0;
	previousReturnValues[3] = 0;
	previousReturnValues[4] = 0;
	previousReturnValues[5] = 0;
	previousReturnValues[6] = 0;
	previousReturnValues[7] = 0;	
}

int State::updateValue(int valueIndex)
{
	values[valueIndex] = analogRead(pinID);
	if (binary[valueIndex])
	{	
		int temp; 
		if(previousReturnValues[valueIndex] == 0) // button is in up-state and can detect down
			temp = ((values[valueIndex] > 150) ? 127 : 0);
		else
			temp = ((values[valueIndex] < 50) ? 0 : 127);
		if (temp != previousReturnValues[valueIndex])
		{
			previousReturnValues[valueIndex] = temp;
			return temp;
		}
		else
			return -1;
	}else
	{
		if ((values[valueIndex] < (ccToLevel(previousReturnValues[valueIndex]) - 2)) || (values[valueIndex] > (ccToLevel(previousReturnValues[valueIndex] + 1) + 1)))
			return previousReturnValues[valueIndex] = levelToCC(values[valueIndex]);
		else
			return -1;
	}
}

int State::levelToCC(int level) // custom mapping of voltage levels 0-1023 to CC values 0-127
{
	if (level < 3)
		return 0;
	else if (level > 1020)
		return 127;
	else
		return (1 + (level - 3) / 8.095);	
}

// turn this into   getLowerEdge and getUpperEdge
int State::ccToLevel(int cc) // custom mapping of cc values 0-127 to voltage levels 0-1023  to improve the start and end point of the potentiometers 
{
	if (cc == 0)
		return 0;
	else if (cc == 127)
		return 1020;
	else
		return ((cc - 1) * 8.095 + 3);
}

void State::setBinary(bool bin)
{
	for (int i = 0; i < 8; i++)
	{
		binary[i] = bin;
	}
}

void State::setBinary(bool bin, int index)
{
	binary[index] = bin;
}