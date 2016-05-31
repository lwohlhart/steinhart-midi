// ArpeggiatorEngine.h

#ifndef _ARPEGGIATORENGINE_h
#define _ARPEGGIATORENGINE_h

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

#define ARPEGGIATOR_LEDS 20
class ArpeggiatorEngine
{
public:
	ArpeggiatorEngine();
	~ArpeggiatorEngine();
	bool shiftDown;
	char octave;
	char velocity;
	bool ledStates[ARPEGGIATOR_LEDS];

	char buttonDown(char buttonIndex);
	char* buttonUp(char buttonIndex);
	char* releaseAll();
	void encoderValueChange(int deltaValue);
	void updateLedStates(char* ledStates, const char* ledIndices);
private:
	bool activatedNotes[128];
	int _octaveInterpolation;
};



#endif

