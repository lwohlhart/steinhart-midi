#include "DrumRackEngine.h"
#include "Arduino.h"


DrumRackEngine::DrumRackEngine()	
	: shiftDown(false), octave(4), velocity(64), _octaveInterpolation(8)
{

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
	return noteReleaseMessages;
}

void DrumRackEngine::encoderValueChange(int deltaValue)
{
	if (shiftDown)
	{
		_octaveInterpolation = min(_octaveInterpolation + deltaValue, 16);
		_octaveInterpolation = max(_octaveInterpolation, 0);
		octave = (int) floor(_octaveInterpolation / 2);		
	}
	else
	{
		velocity = max(min(velocity + deltaValue*8, 127), 0);
	}
}

void DrumRackEngine::updateLedStates(char* ledStates, const char* ledIndices)
{
	for (int i = 0; i < 16; i++)
	{
		ledStates[ledIndices[i]] = activatedNotes[i + octave * 16];
	}
	if (shiftDown)
	{

		ledStates[ledIndices[16]] = ((0 < octave) && (octave <= 4));
		ledStates[ledIndices[17]] = ((1 < octave) && (octave <= 5));
		ledStates[ledIndices[18]] = ((2 < octave) && (octave <= 6));
		ledStates[ledIndices[19]] = ((3 < octave) && (octave <= 7));
		/*
		ledStates[ledIndices[16]] = (((octave & 1) == 0) ? 0 : 1);
		ledStates[ledIndices[17]] = (((octave & 2) == 0) ? 0 : 1);
		ledStates[ledIndices[18]] = (((octave & 4) == 0) ? 0 : 1);
		ledStates[ledIndices[19]] = (((octave & 8) == 0) ? 0 : 1);
		*/
	}
	else
	{
		ledStates[ledIndices[16]] = (velocity > 0);
		ledStates[ledIndices[17]] = (velocity > 32);
		ledStates[ledIndices[18]] = (velocity > 64);
		ledStates[ledIndices[19]] = (velocity > 96);
	}
}