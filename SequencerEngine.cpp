#include "SequencerEngine.h"
#include "Arduino.h"

SequencerEngine::SequencerEngine() : selectedSequenceIndex(0), shiftDown(false), lastNoteToggleTime(0), _sequencePosition(-1)
{

}


void SequencerEngine::buttonDown(int buttonIndex)
{
	if ((0 <= buttonIndex) && (buttonIndex < 16))
	{		
		if (shiftDown) 
		{
			selectedSequenceIndex = buttonIndex;
		}else if (millis() - lastNoteToggleTime > 200)
		{
			sequences[selectedSequenceIndex].toggleNote(buttonIndex);
			lastNoteToggleTime = millis();
		}
	}
}


void SequencerEngine::buttonLongPress(int buttonIndex)
{
	if (shiftDown)
		sequences[buttonIndex].clear();
}


void SequencerEngine::encoderValueChange(int deltaValue)
{
	if (shiftDown)
	{
		sequences[selectedSequenceIndex].changeCurrentNoteLength(deltaValue);
		/*Serial.print("new noteLength: ");
		Serial.println((int) sequences[selectedSequenceIndex].getCurrentNoteLength());*/
	}
	else
	{
		sequences[selectedSequenceIndex].swing = max(min(sequences[selectedSequenceIndex].swing + 0.1*deltaValue, 1), 0);
		/*Serial.print(sequences[selectedSequenceIndex].swing);
		Serial.println(" new swing");*/
	}
}


/*char * SequencerEngine::getDebugMessages()
{
	return sequences[selectedSequenceIndex].messages;
}*/


void SequencerEngine::updateLedStates(char* ledStates, const char* ledIndices)
{
	for (int i = 0; i < 16; i++)
	{
		ledStates[ledIndices[i]] = (shiftDown ? sequences[i].active : sequences[selectedSequenceIndex].ledStates[i]);
	}
	unsigned char len = sequences[selectedSequenceIndex].getCurrentNoteLength();
	if (shiftDown)
	{
		ledStates[ledIndices[selectedSequenceIndex]] = 2;	
		ledStates[ledIndices[23]] = (len >= 2);
		ledStates[ledIndices[24]] = (len >= 4);
		ledStates[ledIndices[25]] = (len >= 8);
		ledStates[ledIndices[26]] = (len >= 8);
	}
	else
	{
		if (_sequencePosition >= 0)
		{
			int pos = ((int)(_sequencePosition / len)) % 16;
			ledStates[ledIndices[pos]] = (ledStates[ledIndices[pos]] == 0) ? 1 : 40;
		}
		ledStates[ledIndices[23]] = sequences[selectedSequenceIndex].swing > 0;
		ledStates[ledIndices[24]] = sequences[selectedSequenceIndex].swing > 0.25;
		ledStates[ledIndices[25]] = sequences[selectedSequenceIndex].swing > 0.5;
		ledStates[ledIndices[26]] = sequences[selectedSequenceIndex].swing > 0.75;
	}
}


void SequencerEngine::setSequencePosition(int sequencePosition)
{
	_sequencePosition = sequencePosition % 128;
}

void SequencerEngine::getMessagesAtCurrentPosition(char* messages)
{
	//char messages[16];
	for (int i = 0; i < 16; i++)
	{
		if ((sequences[i].active)&&(_sequencePosition >= 0))
		{
			messages[i] = sequences[i].messages[_sequencePosition];
		}
		else
		{			
			messages[i] = 0;
		}		
	}	
	//return messages;
}