/*#include <midi_Settings.h>
#include <midi_Namespace.h>
#include <midi_Message.h>
#include <midi_Defs.h>
#include <MIDI.h>*/
//#include "../Arduino_MIDI_Library_v4.2/MIDI/MIDI.h"
#include "C:\Lucas\Studium\Bakk Arbeit\Workspace\libraries\MIDI\MIDI.h"
#include "C:\Lucas\Studium\Bakk Arbeit\Workspace\libraries\MIDI\midi_Settings.h"
#include "C:\Lucas\Studium\Bakk Arbeit\Workspace\libraries\MIDI\midi_Namespace.h"
#include "C:\Lucas\Studium\Bakk Arbeit\Workspace\libraries\MIDI\midi_Message.h"
#include "C:\Lucas\Studium\Bakk Arbeit\Workspace\libraries\MIDI\midi_Defs.h"
#include "State.h"
#include "SequencerEngine.h"
#include "DrumRackEngine.h"


MIDI_CREATE_DEFAULT_INSTANCE();

#define ENCODER_PIN_A  2
#define ENCODER_PIN_B  4

#define LED_SRCLK 5
#define LED_RCLK 6
#define LED_DATA 7

#define MUX_SELECT_PIN_0 8
#define MUX_SELECT_PIN_1 9
#define MUX_SELECT_PIN_2 10

// CC MUXES
#define MUX_INPUT_PIN_1 A0
#define MUX_INPUT_PIN_2 A1
#define MUX_INPUT_PIN_3 A4
#define MUX_INPUT_PIN_4 A3

// buttons  and playground
#define MUX_INPUT_PIN_5 A7
#define MUX_INPUT_PIN_6 A5
#define MUX_INPUT_PIN_7 A6
#define MUX_INPUT_PIN_8 A2

#define LED 13   		    // LED pin on Arduino Uno
/*	LED SHIFT PINS   { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32 };
	Pins 0-7 
*/
// old because cc shouldnt have 0's const char _ccNumberByGlobalMuxPinID[32] = { 16, 24, 25, 17, 26, 18, 27, 19, 20, 28, 29, 21, 30, 22, 31, 23, 0, 8, 9, 1, 10, 2, 11, 3, 4, 12, 13, 5, 14, 6, 15, 7 };
const char _ccNumberByGlobalMuxPinID[32] = { 17, 25, 26, 18, 27, 19, 28, 20, 21, 29, 30, 22, 31, 23, 32, 24, 1, 9, 10, 2, 11, 3, 12, 4, 5, 13, 14, 6, 15, 7, 16, 8 };
const char _buttonIDByGlobalMuxPinID[32] = {	 8,  0, 1, 9, 2, 10, 3, 11, 12,  4,  5, 13,  6, 14,  7, 15, 40,	 40,  40, 40,  16, 17,  18, 19, 20, 21, 40, 40, 40, 40, 40, 40 }; // todo  enter correct ids
const char _ledIndexByID[32] =	{ 7, 5, 2, 1, 8, 13, 12, 9, 6, 4, 3, 0, 10, 15, 14, 11, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 }; // todo update after Romans workover

bool _ledStates[32] =			{ 0, 0, 0, 0, 0,  0,  0, 0, 0, 0, 0, 0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0,  0 };
unsigned long _buttonLongPressStartTime[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


char _selectedMuxChannel = 0;
char _selectedEngine = 0;
State _muxStates[8];

SequencerEngine _sequencer;
DrumRackEngine _drumRack;
 


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

	_muxStates[0].pinID = MUX_INPUT_PIN_1;
	_muxStates[0].setBinary(true, 1);		// set the switches to binary {0,127}
	_muxStates[0].setBinary(true, 2);
	_muxStates[0].setBinary(true, 4);
	_muxStates[0].setBinary(true, 6);
	_muxStates[1].pinID = MUX_INPUT_PIN_2;
	_muxStates[1].setBinary(true, 1);		// set the switches to binary {0,127}
	_muxStates[1].setBinary(true, 2);
	_muxStates[1].setBinary(true, 4);
	_muxStates[1].setBinary(true, 6);
	_muxStates[2].pinID = MUX_INPUT_PIN_3;
	_muxStates[3].pinID = MUX_INPUT_PIN_4;

	_muxStates[4].pinID = MUX_INPUT_PIN_5;
	_muxStates[4].setBinary(true);			// make button mux binary
	_muxStates[5].pinID = MUX_INPUT_PIN_6;
	_muxStates[5].setBinary(true);			// make button mux binary
	_muxStates[6].pinID = MUX_INPUT_PIN_7;
	_muxStates[6].setBinary(true);			// make button mux binary
	_muxStates[6].setBinary(false, 1);	// make actionButton-Potentiometers non-binary
	_muxStates[6].setBinary(false, 2);	// make actionButton-Potentiometers non-binary
	_muxStates[7].pinID = MUX_INPUT_PIN_8;
	_muxStates[7].setBinary(true);			// make button and tristate mux binary

	MIDI.begin();	//Serial.begin(31250);
	/*_sequencer.buttonDown(0);
	_sequencer.buttonDown(1);
	_sequencer.buttonDown(1);
	_sequencer.shiftDown = true;
	_sequencer.encoderValueChange(-1);
	_sequencer.shiftDown = false;
	_sequencer.buttonDown(1);
	bool* debugLedStates = _sequencer.getLedStates();
	char* debugMessages = _sequencer.getDebugMessages();
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
	*/

	/*
	MIDI.setHandleClock(clockCallback);
	MIDI.setHandleStart(startCallback);
	*/
	//MIDI.setHandleNoteOn(noteOnCallback);
	//MIDI.setInputChannel(1);	
}

int temp_led_value = 0;

void loop()
{
	//MIDI.read();
	processCCValues();
	processButtonEventsAndStateSettings();
	_selectedMuxChannel = (_selectedMuxChannel + 1) % 8;
	selectMuxChannel(_selectedMuxChannel);
	if (_selectedMuxChannel == 0)
	{	
		// whatever/wherever the encoding of the tristate might be
		if (_muxStates[6].values[0] > 60)
			_selectedEngine = 0;
		else if (_muxStates[6].values[3] > 60)
			_selectedEngine = 2;
		else		
			_selectedEngine = 1;		
	}

	for (int i = 0; i < 32; i++)
	{
		_ledStates[i] = (i*4 < temp_led_value);
	}
	int time = 16431240 >> 3*((millis() / 800) % 8);
	updateRotaryEngineIndicatorColor(_selectedEngine);
	flushLedStates();
}

void updateRotaryEngineIndicatorColor(int engine)
{
	_ledStates[8] = (engine != 0);
	_ledStates[9] = (engine != 1);
	_ledStates[10] = (engine != 2);
}

void processButtonEventsAndStateSettings()
{
	for (int i = 4; i <= 7; i++)  //   parse mux 4-7 (binary muxes: Buttons and Tristate)
	{
		int globalButtonId = _buttonIDByGlobalMuxPinID[(i - 4) * 8 + _selectedMuxChannel];		
		int buttonAction = _muxStates[i].updateValue(_selectedMuxChannel);
		if (buttonAction != -1) // buttonAction -1: nothing, 0: buttonUp, 127: buttonDown
		{
			if (buttonAction == 0)
			{
				//temp_led_value = 0;  // test
				/*Serial.print("buttonUp : ");
				Serial.print(globalButtonId);
				Serial.print(" mux");
				Serial.print(i);
				Serial.print(" channel");
				Serial.println((int) _selectedMuxChannel);*/
				if (globalButtonId < 16) // sequencer-button
				{
					_buttonLongPressStartTime[globalButtonId] = 0;
				}
			}
			else if (buttonAction > 0)
			{
				//temp_led_value = 127;  // test
				/*Serial.print("buttonDown : ");
				Serial.print(globalButtonId);
				Serial.print(" mux");
				Serial.print(i);
				Serial.print(" channel");
				Serial.println((int) _selectedMuxChannel);*/
				if (globalButtonId < 16) // sequencer-button
				{
					_buttonLongPressStartTime[globalButtonId] = millis();
				}
			}
		}

		if ((globalButtonId < 16) && (_buttonLongPressStartTime[globalButtonId] > 0)) // sequencer-button is down and we have to detect a longpress event
		{
			if ((unsigned long)(millis() - _buttonLongPressStartTime[globalButtonId]) >= 1500)
			{
				_buttonLongPressStartTime[globalButtonId] = 0;
				// button longPressEvent
				/*Serial.print("buttonLongPress : ");
				Serial.println(globalButtonId);*/
			}
		}
	}
}

void processCCValues()
{
	for (int i = 0; i <= 3; i++)  //   parse mux 0-3 
	{
		int changedValue = _muxStates[i].updateValue(_selectedMuxChannel);
		if (changedValue != -1)
		{
			temp_led_value = changedValue;
			midi::DataByte ccNumber = (midi::DataByte) (_ccNumberByGlobalMuxPinID[i * 8 + _selectedMuxChannel]);
			midi::DataByte ccValue = (changedValue);
			MIDI.sendControlChange(ccNumber, ccValue, (midi::Channel) 4);
			/*Serial.print("mux");
			Serial.print(i);
			Serial.print(" pin");
			Serial.print(int(_selectedMuxChannel));
			Serial.print(": ");			
			Serial.println(ccNumber);*/			
		}
	}

}

void updateRotaryEncoder() {
	
	if (digitalRead(ENCODER_PIN_A) == digitalRead(ENCODER_PIN_B)) {		
		if (_selectedEngine == 0)
			_sequencer.encoderValueChange(-1);
		else if (_selectedEngine == 1)
			_drumRack.encoderValueChange(-1);
		else
		{

		}							
	}
	else {
		if (_selectedEngine == 0)
			_sequencer.encoderValueChange(1);
		else if (_selectedEngine == 1)
			_drumRack.encoderValueChange(1);
		else
		{

		}		
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
	for (int outputIndex = 31; outputIndex >= 0; outputIndex--)
	{
		digitalWrite(LED_DATA, _ledStates[outputIndex]);
		digitalWrite(LED_SRCLK, HIGH);
		digitalWrite(LED_SRCLK, LOW);
	}
	digitalWrite(LED_RCLK, HIGH);
}

void noteOnCallback(byte channel, byte note, byte velocity)
{
	/*digitalWrite(LED, HIGH);
	delay(10*note);
	digitalWrite(LED, LOW);*/
	//MIDI.sendNoteOn(40, 127, 1);  // Send a Note (pitch 42, velo 127 on channel 1)		
	//delay(1000);
	//MIDI.sendNoteOff(40, 0, 1);   // Stop the note	
}

void clockCallback()
{
	/*
	_midiClockCycle++;
	if (_midiClockCycle % 24 == 0)
	{	
		if (_notePos >= 0)
			MIDI.sendNoteOff(_notes[_notePos], 0, 1);   // Stop the note	
		_notePos = (_notePos + 1) % 8;
		MIDI.sendNoteOn(_notes[_notePos], 127, 1);  // Send a Note (pitch 42, velo 127 on channel 1)				
	}*/
}

void startCallback()
{
	
	//clockCallback();
}