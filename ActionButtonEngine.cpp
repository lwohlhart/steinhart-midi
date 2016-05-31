#include "ActionButtonEngine.h"
#include <math.h>
#include <Arduino.h>

ActionButtonEngine::ActionButtonEngine()
	: _selectedButton(0), _lastNoteToggleTime(0)
{
	active = false;
	memset(_targetAmplitudes, 63, 4);	
	memset(_currentFrequencies, 10, 4);	
	memset(_phases, 0, 4);
}


ActionButtonEngine::~ActionButtonEngine()
{
}


void ActionButtonEngine::buttonDown(char buttonID)
{
	if (millis() - _lastNoteToggleTime > 200)
	{		
		_lastNoteToggleTime = millis();
		buttonID = buttonID - 16;
		_selectedButton = buttonID;
		_activeButtons[buttonID] = !_activeButtons[buttonID];
		if (_activeButtons[buttonID])
		{
			_activationTimes[buttonID] = millis();
			_phases[buttonID] = 0;
		}
		else
		{
			_activationTimes[buttonID] = 0;
			_phases[buttonID] = 0;
		}
		active = (_activeButtons[0] || _activeButtons[1] || _activeButtons[2] || _activeButtons[3]);
	}
}



void ActionButtonEngine::updateLedStates(char* ledStates, const char* ledIndices)
{

	for (int i = 0; i < 4; i++)
	{
		ledStates[ledIndices[16 + i]] = _activeButtons[i];
	}
	/*
	unsigned char len = sequences[selectedSequenceIndex].getCurrentNoteLength();
	if (shiftDown)
	{
		ledStates[ledIndices[selectedSequenceIndex]] = 2;
		ledStates[ledIndices[16]] = (len >= 2);
		ledStates[ledIndices[17]] = (len >= 4);
		ledStates[ledIndices[18]] = (len >= 8);
		ledStates[ledIndices[19]] = (len >= 8);
	}
	else
	{
		if (_sequencePosition >= 0)
		{
			int pos = ((int)(_sequencePosition / len)) % 16;
			ledStates[ledIndices[pos]] = (ledStates[ledIndices[pos]] == 0) ? 1 : 40;
		}
		ledStates[ledIndices[16]] = sequences[selectedSequenceIndex].swing > 0;
		ledStates[ledIndices[17]] = sequences[selectedSequenceIndex].swing > 0.25;
		ledStates[ledIndices[18]] = sequences[selectedSequenceIndex].swing > 0.5;
		ledStates[ledIndices[19]] = sequences[selectedSequenceIndex].swing > 0.75;
	}*/
}



void ActionButtonEngine::value1Change(int newValue)
{
	_targetAmplitudes[_selectedButton] = newValue;
}

void ActionButtonEngine::value2Change(int newValue)
{	
	if (newValue == 0)
		_phases[_selectedButton] = 0;
	_currentFrequencies[_selectedButton] = (double) newValue / 64.0;
}

void ActionButtonEngine::getCCValues(char* values)
{	
	unsigned long time = millis();
	for (int i = 0; i < 4; i++)
	{
		if (_activeButtons[i])
		{
			unsigned long timeDelta = time - _activationTimes[i];
			_activationTimes[i] = time;
			if (i < 2)  // LFOs
			{
				_phases[i] += (double)timeDelta * 0.00628318530717958647692528676656 * _currentFrequencies[i];	
				while (_phases[i] > 2 * M_PI)
					_phases[i] -= 2 * M_PI;
				//Serial.println(_phases[i] * 1000);
				// 0.00628318530717958647692528676656 = 2*PI/1000				
				values[i] = 63 + (_targetAmplitudes[i] >> 1)*sin(_phases[i]); // todo   actually   make it frequencies[_values2[i]] and make frequency-table
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
}
