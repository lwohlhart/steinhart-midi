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

void ActionButtonEngine::value1Change(int newvValue)
{
	_values1[_selectedButton] = newvValue;
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
				values[i] = 63 + (_values1[i] >> 1)*sin(passedTime / 1000 * _values2[i]); // todo   actually   make it frequencies[_values2[i]] and make frequency-table
			else	//ramps
			{
				
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
