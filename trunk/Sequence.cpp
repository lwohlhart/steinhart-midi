#include "Sequence.h"
#include <string.h>
#include <Arduino.h>

Sequence::Sequence()
{
	swing = 0.0;
	_currentNoteLength = 8;
	active = false;
}


int Sequence::toggleNote(int noteIndex)
{
	bool free = true;
	int ind = noteIndex * _currentNoteLength;
	for (int i = ind; i < ind + _currentNoteLength; i++) // check if there are notes ahead that would overlap
	{
		if(notes[i] != 0)
		{
			free = false;
			break;
		}
	}
	memset(notes + ind, 0, _currentNoteLength);
	if (free)
	{
		notes[ind] = _currentNoteLength;
		// check if there are notes behind that have to be shortened
		for (int j = (ind - 1); j > max(0, ind - 8); j--)
		{
			while (notes[j] > (ind - j))
			{
				notes[j] >>= 1;
			}
		}
	}
	updateMessages();
	updateLeds();
	return 0;
}


void Sequence::clear()
{
	memset(notes, 0, SEQUENCE_ENTRIES);
	memset(messages, 0, SEQUENCE_ENTRIES);
	active = false;
}


void Sequence::updateMessages()
{
	memset(ledStates, 0, SEQUENCE_LEDS);
	memset(messages, 0, SEQUENCE_ENTRIES);
	int lastNoteDuration = 0;
	int lastNoteIndex = -1;
	for (int i = 0; i < 128; i++)
	{
		if ((lastNoteIndex >= 0)&&((i - lastNoteIndex) >= lastNoteDuration))
		{
			messages[i] = -1;
			lastNoteIndex = -1;
			lastNoteDuration = 0;
		}
		if (notes[i] != 0)
		{
			active = true;
			if ((messages[i] == -1)||((lastNoteIndex >= 0)&&( lastNoteDuration >= (i - lastNoteIndex)))) // last note was just turned of or still wants to be played
				messages[i] = 2;
			else
				messages[i] = 1;
			lastNoteIndex = i;
			lastNoteDuration = notes[i];

		}
	}
}


void Sequence::updateLeds()
{
	for (int i = 0; i < _currentNoteLength*16; i++)
	{
		if (notes[i] != 0)
			ledStates[(int)i / _currentNoteLength] = 1;
	}

}

unsigned char Sequence::getCurrentNoteLength()
{
	return _currentNoteLength;
}

void Sequence::setCurrentNoteLength(unsigned char newNoteLength)
{
	_currentNoteLength = max(min(newNoteLength,8),1);
	updateLeds();
}
