#pragma once
class State
{
	public:
		State();
		int pinID;
		int values[8];
		int updateValue(int valueIndex);
	private:
		int previousReturnValues[8];
		int levelToCC(int level);
		int ccToLevel(int cc);
};

