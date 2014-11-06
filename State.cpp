#include "State.h"
#include <Arduino.h>
State::State()
{
	values[0] = 0;
	previousReturnValues[0] = 0;
}

int State::updateValue(int valueIndex)
{
	values[valueIndex] = analogRead(pinID);
	if (valueIndex == 0)
	{
		/*
		Serial.print(values[valueIndex]);
		Serial.print(" cc = ");
		Serial.println(levelToCC(values[valueIndex]));
		*/
	}
	// - 2 means a conservative threshold that reduces bouncing values at the edges of each cc-value

	
		/*
	if ((values[valueIndex] < ((previousReturnValues[valueIndex] << 3) - 2)) || (values[valueIndex] > (((previousReturnValues[valueIndex] + 1) << 3) + 1)))
		return previousReturnValues[valueIndex] = (values[valueIndex] >> 3);
	else
		return -1;
		*/

	if ((values[valueIndex] < (ccToLevel(previousReturnValues[valueIndex]) - 2)) || (values[valueIndex] > (ccToLevel(previousReturnValues[valueIndex] + 1) + 1)))
		return previousReturnValues[valueIndex] = levelToCC(values[valueIndex]);
	else
		return -1;
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