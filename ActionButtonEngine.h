#pragma once
class ActionButtonEngine
{
public:
	ActionButtonEngine();
	~ActionButtonEngine();
	void buttonDown(char buttonID);
	void updateLedStates(char* ledStates, const char* ledIndices);
	void value1Change(int newValue);
	void value2Change(int newValue);
	bool active;
	void getCCValues(char* values);
private:
	bool _activeButtons[4];
	unsigned long _activationTimes[4];
	double _phases[4];
	int _selectedButton;
	char _targetAmplitudes[4];	
	double _currentFrequencies[4];	
	char _previousCCValues[4];
	unsigned long _lastNoteToggleTime;
};

