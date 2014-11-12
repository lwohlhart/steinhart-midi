#pragma once
#include "Sequence.h"
class SequencerEngine
{
public:
	SequencerEngine();
	Sequence sequences[16];
	char selectedSequenceIndex;
	bool shiftDown;
	void buttonDown(int buttonIndex);
	void buttonLongPress(int buttonIndex);
	void encoderValueChange(int deltaValue);
	char* getDebugMessages();
	bool* getLedStates();
};

