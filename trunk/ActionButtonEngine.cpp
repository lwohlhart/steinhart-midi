#include "ActionButtonEngine.h"
#include <math.h>
#include <Arduino.h>

ActionButtonEngine::ActionButtonEngine()
	: _selectedButton(0)
{
	active = false;
}


ActionButtonEngine::~ActionButtonEngine()
{
}


void ActionButtonEngine::buttonDown(char buttonID)
{
	_selectedButton = buttonID;
	_activeButtons[buttonID] = !_activeButtons[buttonID];
	if (_activeButtons[buttonID])
		_activationTimes[buttonID] = millis();
	else
		_activationTimes[buttonID] = 0;
	active = (_activeButtons[0] || _activeButtons[1] || _activeButtons[2] || _activeButtons[3]);
}

void ActionButtonEngine::value1Change(int newValue)
{
	_values1[_selectedButton] = newValue;
}

void ActionButtonEngine::value2Change(int newValue)
{
	_values2[_selectedButton] = newValue;
}

char* ActionButtonEngine::getCCValues(unsigned long time)
{
	char values[4];
	for (int i = 0; i < 4; i++)
	{
		if (_activeButtons[i])
		{
			unsigned long passedTime = time - _activationTimes[i];
			if (i < 2)  // LFOs
			{
				// 0.00628318530717958647692528676656 = 1/1000*2*PI
				values[i] = 63 + (_values1[i] >> 1)*sin(passedTime * 0.00628318530717958647692528676656 * _values2[i]); // todo   actually   make it frequencies[_values2[i]] and make frequency-table
			}
			else	//ramps
			{
				values[i] = 63;// todo
			}
			if (values[i] == _previousCCValues[i])
				values[i] = -1;
			else
				_previousCCValues[i] = values[i];
		}else
			values[i] = -1;
	}
	return values;
}
