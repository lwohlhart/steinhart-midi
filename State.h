#pragma once
class State
{
	public:
		State(int pin);
		int values[8];
		int updateValue(int valueIndex);
	private:
		int pinID;
};

