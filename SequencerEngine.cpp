#include "SequencerEngine.h"
#include "Arduino.h"

SequencerEngine::SequencerEngine() : selectedSequenceIndex(0), shiftDown(false)
{
	//selectedSequenceIndex = 0;
	//shiftDown = false;
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
		sequences[selectedSequenceIndex].changeCurrentNoteLength(deltaValue);
		//Serial.print("new noteLength: ");
		//Serial.println((int) sequences[selectedSequenceIndex].getCurrentNoteLength());
	}
	else
	{
		sequences[selectedSequenceIndex].swing = max(min(sequences[selectedSequenceIndex].swing + 0.1*deltaValue, 1), 0);
		Serial.print(sequences[selectedSequenceIndex].swing);
		Serial.println(" new swing");
	}
}


char * SequencerEngine::getDebugMessages()
{
	return sequences[selectedSequenceIndex].messages;
}


bool* SequencerEngine::getLedStates()
{
	bool* ret = sequences[selectedSequenceIndex].ledStates;
	if (shiftDown)
	{
		for (int i = 0; i < 16; i++)
		{
			ret[i] = sequences[i].active;
		}
	}
	return ret;
}
