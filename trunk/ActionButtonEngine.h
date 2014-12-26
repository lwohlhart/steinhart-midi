#pragma once
class ActionButtonEngine
{
public:
	ActionButtonEngine();
	~ActionButtonEngine();
	void buttonDown(char buttonID);
	void value1Change(int newValue);
	void value2Change(int newValue);
	bool active;
	char* getCCValues(unsigned long time);
private:
	bool _activeButtons[4];
	unsigned long _activationTimes[4];
	char _selectedButton;
	char _values1[4];
	char _values2[4];
	char _previousCCValues[4];
};

