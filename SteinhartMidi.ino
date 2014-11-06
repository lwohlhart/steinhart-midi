#include <midi_Settings.h>
#include <midi_Namespace.h>
#include <midi_Message.h>
#include <midi_Defs.h>
#include <MIDI.h>
#include "State.h"
#include "SequencerEngine.h"

MIDI_CREATE_DEFAULT_INSTANCE();

#define ENCODER_PIN_A  2
#define ENCODER_PIN_B  4

#define LED_SRCLK 5
#define LED_RCLK 6
#define LED_DATA 7

#define MUX_SELECT_PIN_0 8
#define MUX_SELECT_PIN_1 9
#define MUX_SELECT_PIN_2 10

#define MUX_INPUT_PIN_1 A0
#define MUX_INPUT_PIN_2 A1
#define MUX_INPUT_PIN_3 A2
#define MUX_INPUT_PIN_4 A3
#define MUX_INPUT_PIN_5 A4
#define MUX_INPUT_PIN_6 A5
#define MUX_INPUT_PIN_7 A6
#define MUX_INPUT_PIN_8 A7

#define LED 13   		    // LED pin on Arduino Uno
/*	LED SHIFT PINS   { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32 };
	Pins 0-7 
*/
const char _ccNumberByGlobalMuxPinID[32] = { 16, 24, 25, 17, 26, 18, 27, 19, 20, 28, 29, 21, 30, 22, 31, 23, 0, 8, 9, 1, 10, 2, 11, 3, 4, 12, 13, 5, 14, 6, 15, 7 };
const char _ledIndexByID[32] =	{ 7, 5, 2, 1, 8, 13, 12, 9, 6, 4, 3, 0, 10, 15, 14, 11, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 };
bool _ledStates[32] =			{ 0, 0, 0, 0, 0,  0,  0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  1,  1,  1,  0,  0,  0,  0,  0 };
// 
char _selectedMuxChannel = 0;
State muxStates[8];

SequencerEngine sequencer;


int _loop_count = 0;
int _notes[8] = {36,37,38,39,40,40,41,41};
int _notePos = 0;
int _midiClockCycle = 0;

void setup()
{
	pinMode(LED_SRCLK, OUTPUT);
	pinMode(LED_RCLK, OUTPUT);
	pinMode(LED_DATA, OUTPUT);

	pinMode(MUX_SELECT_PIN_0, OUTPUT);
	pinMode(MUX_SELECT_PIN_1, OUTPUT);
	pinMode(MUX_SELECT_PIN_2, OUTPUT);

	pinMode(LED, OUTPUT);

	pinMode(ENCODER_PIN_A, INPUT);
	pinMode(ENCODER_PIN_B, INPUT);
	digitalWrite(ENCODER_PIN_A, HIGH);       // turn on pullup resistor
	digitalWrite(ENCODER_PIN_B, HIGH);       // turn on pullup resistor

	attachInterrupt(0, updateRotaryEncoder, CHANGE);  // encoder pin on interrupt 0 - pin 2

	_selectedMuxChannel = 0;
	selectMuxChannel(_selectedMuxChannel);
	flushLedStates();

	muxStates[0].pinID = MUX_INPUT_PIN_1;
	muxStates[1].pinID = MUX_INPUT_PIN_2;
	muxStates[2].pinID = MUX_INPUT_PIN_3;
	muxStates[3].pinID = MUX_INPUT_PIN_4;
	muxStates[4].pinID = MUX_INPUT_PIN_5;
	muxStates[5].pinID = MUX_INPUT_PIN_6;
	muxStates[6].pinID = MUX_INPUT_PIN_7;
	muxStates[7].pinID = MUX_INPUT_PIN_8;
	Serial.begin(115200);
	sequencer.buttonDown(0);
	sequencer.buttonDown(1);
	sequencer.buttonDown(1);
	sequencer.shiftDown = true;
	sequencer.encoderValueChange(-1);
	sequencer.shiftDown = false;
	sequencer.buttonDown(1);
	bool* debugLedStates = sequencer.getLedStates();
	char* debugMessages = sequencer.getDebugMessages();
	for (int i = 0; i < 128; i++)
	{
		Serial.print((int)debugMessages[i]);
		Serial.print("|");
	}
	Serial.println("");
	for (int j = 0; j < 20; j++)
	{
		Serial.print((int)debugLedStates[j]);
		Serial.print("|");
	}
	Serial.println("");
	
	//MIDI.begin();
	/*
	MIDI.setHandleClock(clockCallback);
	MIDI.setHandleStart(startCallback);
	*/
	//MIDI.setHandleNoteOn(noteOnCallback);
	//MIDI.setInputChannel(1);	
}


void loop()
{

}
void shit()
{	
	//MIDI.read();
	for (int i = 0; i < 4; i++)  //   parse mux 0-3 
	{
		int changedValue = muxStates[i].updateValue(_selectedMuxChannel);
		if (changedValue != -1)
		{
			Serial.print(_selectedMuxChannel);
			Serial.print(": ");
			Serial.println(changedValue);
			/*midi::DataByte cc = (changedValue);
			MIDI.sendControlChange((byte)(_ccNumberByGlobalMuxPinID[i*8 + _selectedMuxChannel]), cc, MIDI_CHANNEL_OMNI);*/
		}
	}

	_selectedMuxChannel = (_selectedMuxChannel + 1) % 8;
	selectMuxChannel(_selectedMuxChannel);
	//MIDI.sendNoteOn(36, 127, 1);  // Send a Note (pitch 42, velo 127 on channel 1)		
	//MIDI.sendNoteOff(36, 0, 1);   // Stop the note	
	
	/*int mask = 128;
	for (size_t i = 0; i < 8; i++)
	{
		Serial.print((cc&mask) ? 1 : 0);
		mask >>= 1;
	}*/
	//Serial.println(cc);	
	//startCallback()
}

void updateRotaryEncoder() {
	if (digitalRead(ENCODER_PIN_A) == digitalRead(ENCODER_PIN_B)) {
		//encoder0Pos++;  // right turn 
	}
	else {
		//encoder0Pos--;  // left turn
	}
}

void selectMuxChannel(int channelNumber)
{		
	digitalWrite(MUX_SELECT_PIN_0, channelNumber & 1);
	digitalWrite(MUX_SELECT_PIN_1, channelNumber & 2);
	digitalWrite(MUX_SELECT_PIN_2, channelNumber & 4);
}

void flushLedStates()
{
	digitalWrite(LED_RCLK, LOW);
	for (int outputIndex = 23; outputIndex >= 0; outputIndex--)
	{
		digitalWrite(LED_DATA, _ledStates[outputIndex]);
		digitalWrite(LED_SRCLK, HIGH);
		digitalWrite(LED_SRCLK, LOW);
	}
	digitalWrite(LED_RCLK, HIGH);
}

void noteOnCallback(byte channel, byte note, byte velocity)
{
	digitalWrite(LED, HIGH);
	delay(10*note);
	digitalWrite(LED, LOW);
	//MIDI.sendNoteOn(40, 127, 1);  // Send a Note (pitch 42, velo 127 on channel 1)		
	//delay(1000);
	//MIDI.sendNoteOff(40, 0, 1);   // Stop the note	
}

void clockCallback()
{
	_midiClockCycle++;
	if (_midiClockCycle % 24 == 0)
	{	
		if (_notePos >= 0)
			MIDI.sendNoteOff(_notes[_notePos], 0, 1);   // Stop the note	
		_notePos = (_notePos + 1) % 8;
		MIDI.sendNoteOn(_notes[_notePos], 127, 1);  // Send a Note (pitch 42, velo 127 on channel 1)				
	}
}

void startCallback()
{
	_midiClockCycle = -1;
	_notePos = -1;
	//clockCallback();
}