#include "DrumRackEngine.h"
#include "Arduino.h"


DrumRackEngine::DrumRackEngine()	
	: shiftDown(false)
{

	octave = 0;
	velocity = 127;
}


DrumRackEngine::~DrumRackEngine()
{
}


char DrumRackEngine::buttonDown(char buttonIndex)
{
	char note = buttonIndex + octave * 16;
	activatedNotes[note] = true;
	return note;	
}

char* DrumRackEngine::buttonUp(char buttonIndex)
{ 
	char noteReleaseMessages[9];
	char messageIndex = 0;
	for (int i = 0; i < 8; i++)
	{
		int note = buttonIndex + 16 * i;
		if (activatedNotes[note])
		{
	
			noteReleaseMessages[messageIndex++] = note;
			activatedNotes[note] = false;
		}
	}
	noteReleaseMessages[messageIndex] = -1;
	return noteReleaseMessages;
}

char* DrumRackEngine::releaseAll()
{
	char noteReleaseMessages[129];
	char messageIndex = 0;
	for (int note = 0; note < 128; note++)
	{		
		if (activatedNotes[note])
		{

			noteReleaseMessages[messageIndex++] = note;
			activatedNotes[note] = false;
		}
	}
	noteReleaseMessages[messageIndex] = -1;
	updateLEDs();
	return noteReleaseMessages;
}

void DrumRackEngine::encoderValueChange(int deltaValue)
{
	if (shiftDown)
	{
		octave = max(min(octave + deltaValue, 8), 0);
	}
	else
	{
		velocity = max(min(velocity + deltaValue*8, 127), 0);
	}
	updateLEDs();
}

void DrumRackEngine::updateLEDs()
{
	for (int i = 0; i < 16; i++)
	{
		ledStates[i] = activatedNotes[i + octave * 16];
	}	
	if (shiftDown)
	{
		ledStates[16] = octave & 8;
		ledStates[17] = octave & 4;
		ledStates[18] = octave & 2;
		ledStates[19] = octave & 1;
	}
	else
	{
		ledStates[16] = (velocity > 0);
		ledStates[17] = (velocity > 32);
		ledStates[18] = (velocity > 64);
		ledStates[19] = (velocity > 96);
	}
}