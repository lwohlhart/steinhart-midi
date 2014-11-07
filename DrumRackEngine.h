#pragma once
class DrumRackEngine
{
public:
	DrumRackEngine();
	~DrumRackEngine();
	int octave;
	int velocity;

	int buttonDown(int buttonIndex);
	int buttonUp(int buttonIndex);
};

