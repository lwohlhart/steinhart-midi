#pragma once
#define SEQUENCE_ENTRIES 128
#define SEQUENCE_LEDS 20
class Sequence
{
public:
	Sequence();
	double swing;
	unsigned char notes[SEQUENCE_ENTRIES];
	char messages[SEQUENCE_ENTRIES];
	bool ledStates[SEQUENCE_LEDS];
	bool active;

	void toggleNote(char noteIndex);
	void clear();
	void updateMessages();
	unsigned char getCurrentNoteLength();
	void changeCurrentNoteLength(int delta);
private:
	unsigned char _currentNoteLength; // 8,4,2,1 (1/4, 1/8, 1/16, 1/32)
	void updateLeds();

};

