#include "State.h"
#include <Arduino.h>
State::State(int pin)
{
	pinID = pin;	
	values[0] =  0;
}

int State::updateValue(int valueIndex)
{
	int temp = values[valueIndex];
	values[valueIndex] = (analogRead(pinID) >> 3);
	if (temp != values[valueIndex])
		return values[valueIndex];
	else
		return -1;
}

