#pragma once
#include "Sequence.h"
class SequencerEngine
{
public:
	SequencerEngine();
	Sequence sequences[16];
	char selectedSequenceIndex;
	bool shiftDown;
	unsigned long lastNoteToggleTime;
	void buttonDown(int buttonIndex);
	void buttonLongPress(int buttonIndex);
	void encoderValueChange(int deltaValue);
	//char* getDebugMessages();
	void updateLedStates(char* ledStates, const char* ledIndices);
	void getMessagesAtCurrentPosition(char* messages);
	void setSequencePosition(int sequencePosition);
private:
	int _sequencePosition;
};

