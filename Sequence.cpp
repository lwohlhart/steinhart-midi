#include "Sequence.h"
#include <string.h>
#include <Arduino.h>

Sequence::Sequence() : swing(0.0), _currentNoteLength(8), active(false), _currentNoteLengthInterpolation(7)
{
	clear();
}

void Sequence::toggleNote(char noteIndex)
{		
	for (int ind = noteIndex * _currentNoteLength; ind < 128; ind += 16 * _currentNoteLength)
	{
		setNote(ind);
	}
	
	updateMessages();
	updateLeds();	
}

void Sequence::setNote(int ind)
{
	bool free = true;
	for (int i = ind; i < ind + _currentNoteLength; i++) // check if there are notes ahead that would overlap
	{
		if (notes[i] != 0)
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
}


void Sequence::clear()
{
	//memset(notes, 0, SEQUENCE_ENTRIES);
	//memset(messages, 0, SEQUENCE_ENTRIES);
	active = false;
	updateMessages();
	updateLeds();
}


void Sequence::updateMessages()
{	
	memset(messages, 0, SEQUENCE_ENTRIES);
	int lastNoteDuration = 0;
	int lastNoteIndex = -1;
	for (int i = 0; i < 129; i++)
	{
		int position = i % 128;
		if ((lastNoteIndex >= 0)&&((i - lastNoteIndex) >= lastNoteDuration))
		{
			messages[position] = NOTE_STOP;
			lastNoteIndex = -1;
			lastNoteDuration = 0;
		}
		if (notes[position] != 0)
		{
			active = true;
			if ((messages[position] == -1)||((lastNoteIndex >= 0)&&( lastNoteDuration >= (i - lastNoteIndex)))) // last note was just turned of or still wants to be played
				messages[position] = NOTE_REPLAY;
			else
				messages[position] = NOTE_PLAY;
			lastNoteIndex = i;
			lastNoteDuration = notes[position];

		}
	}
}


void Sequence::updateLeds()
{
	memset(ledStates, 0, SEQUENCE_LEDS);
	for (int i = 0; i < _currentNoteLength*16; i++)
	{
		if (notes[i] != 0)
			ledStates[(int)i / _currentNoteLength] = 1;
	}
	ledStates[16] = (_currentNoteLength >= 1);
	ledStates[17] = (_currentNoteLength >= 2);
	ledStates[18] = (_currentNoteLength >= 4);
	ledStates[19] = (_currentNoteLength >= 8);
}

unsigned char Sequence::getCurrentNoteLength()
{
	return _currentNoteLength;
}

void Sequence::changeCurrentNoteLength(int delta)
{
	_currentNoteLengthInterpolation = max(min(_currentNoteLengthInterpolation + delta, 7), 0);
	if (_currentNoteLengthInterpolation <= 1)
		_currentNoteLength = 1;
	else if (_currentNoteLengthInterpolation <= 3)
		_currentNoteLength = 2;
	else if (_currentNoteLengthInterpolation <= 5)
		_currentNoteLength = 4;
	else
		_currentNoteLength = 8;
	updateLeds();
}
