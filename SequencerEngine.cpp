#include "SequencerEngine.h"
#include "Arduino.h"

SequencerEngine::SequencerEngine()
{
	selectedSequenceIndex = 0;
	shiftDown = false;
}


SequencerEngine::~SequencerEngine()
{
}


void SequencerEngine::buttonDown(int buttonIndex)
{
	if (shiftDown)
		selectedSequenceIndex = buttonIndex;
	else
		sequences[selectedSequenceIndex].toggleNote(buttonIndex);
}


void SequencerEngine::buttonLongPress(int buttonIndex)
{
	if (shiftDown)
		sequences[selectedSequenceIndex].clear();
}


void SequencerEngine::encoderValueChange(int deltaValue)
{
	if (shiftDown)
	{
		unsigned char noteLength = sequences[selectedSequenceIndex].getCurrentNoteLength();
		noteLength = (deltaValue < 0) ? noteLength >> 1 : noteLength << 1;
		sequences[selectedSequenceIndex].setCurrentNoteLength(noteLength);
		Serial.print("new noteLength: ");
		Serial.println((int) sequences[selectedSequenceIndex].getCurrentNoteLength());
	}
	else
	{
		sequences[selectedSequenceIndex].swing = max(min(sequences[selectedSequenceIndex].swing + 0.1*deltaValue, 1), 0);
	}
}


char * SequencerEngine::getDebugMessages()
{
	return sequences[selectedSequenceIndex].messages;
}




bool* SequencerEngine::getLedStates()
{
	return sequences[selectedSequenceIndex].ledStates;
}
