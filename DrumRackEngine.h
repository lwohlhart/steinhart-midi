#pragma once
#define DRUM_RACK_LEDS 20
class DrumRackEngine
{
public:
	DrumRackEngine();
	~DrumRackEngine();
	bool shiftDown;
	char octave;
	char velocity;	
	bool ledStates[DRUM_RACK_LEDS];

	char buttonDown(char buttonIndex);
	char* buttonUp(char buttonIndex);
	char* releaseAll();
	void encoderValueChange(int deltaValue);
	void updateLedStates(char* ledStates, const char* ledIndices);
private:
	bool activatedNotes[128];
	int _octaveInterpolation;
};

