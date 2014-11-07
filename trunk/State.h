#pragma once
class State
{
	public:
		State();
		int pinID;
		int values[8];
		int updateValue(int valueIndex);
		void setBinary(bool bin, int index);
		void setBinary(bool bin);
	private:
		bool binary[8];
		int previousReturnValues[8];
		int levelToCC(int level);
		int ccToLevel(int cc);
};

