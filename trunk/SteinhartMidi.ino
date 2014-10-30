

#include <midi_Settings.h>
#include <midi_Namespace.h>
#include <midi_Message.h>
#include <midi_Defs.h>
#include <MIDI.h>
#include "State.h"

MIDI_CREATE_DEFAULT_INSTANCE();

#define MUX_INPUT_PIN_1 A0
#define MUX_INPUT_PIN_2 A1
#define MUX_INPUT_PIN_3 A2
#define MUX_INPUT_PIN_4 A3


#define MUX_SELECT_PIN_0 8
#define MUX_SELECT_PIN_1 9
#define MUX_SELECT_PIN_2 10

#define LED 13   		    // LED pin on Arduino Uno

int _selectedMuxChannel = 0;

int _loop_count = 0;
int _notes[8] = {36,37,38,39,40,40,41,41};
int _notePos = 0;
int _midiClockCycle = 0;
State muxState(MUX_INPUT_PIN_1);

void setup()
{
	pinMode(MUX_SELECT_PIN_0, OUTPUT);
	pinMode(MUX_SELECT_PIN_1, OUTPUT);
	pinMode(MUX_SELECT_PIN_2, OUTPUT);

	pinMode(LED, OUTPUT);
	_selectedMuxChannel = 0;
	selectMuxChannel(_selectedMuxChannel);
	//Serial.begin(115200);
	//muxState = new State(MUX_INPUT_PIN_1);
		 
	/*
	states[0] = new State(MUX_INPUT_PIN_1);
	states[1] = new State(MUX_INPUT_PIN_2);
	states[2] = new State(MUX_INPUT_PIN_3);
	states[3] = new State(MUX_INPUT_PIN_4);*/
	MIDI.begin();
	/*
	MIDI.setHandleClock(clockCallback);
	MIDI.setHandleStart(startCallback);
	*/
	//MIDI.setHandleNoteOn(noteOnCallback);
	MIDI.setInputChannel(1);	
}


void loop()
{	
	//MIDI.read();
	
	int changedValue = muxState.updateValue(_selectedMuxChannel);
	if (changedValue != -1)
	{
		/*
		Serial.print(_selectedMuxChannel);
		Serial.print(": ");
		Serial.println(changedValue);*/
		midi::DataByte cc = (changedValue);
		MIDI.sendControlChange((byte)(20+_selectedMuxChannel), cc, 4);

	}

	_selectedMuxChannel = (_selectedMuxChannel + 1) % 8;
	selectMuxChannel(_selectedMuxChannel);

	//MIDI.sendNoteOn(36, 127, 1);  // Send a Note (pitch 42, velo 127 on channel 1)		
	//delay(500);	
	//MIDI.sendNoteOff(36, 0, 1);   // Stop the note	
	//delay(500);
	
	/*int mask = 128;
	for (size_t i = 0; i < 8; i++)
	{
		Serial.print((cc&mask) ? 1 : 0);
		mask >>= 1;
	}*/
	//Serial.println(cc);	
	//loop_count++;
	//delay(100);
	//startCallback()
}

void selectMuxChannel(int channelNumber)
{		
	digitalWrite(MUX_SELECT_PIN_0, channelNumber & 1);
	digitalWrite(MUX_SELECT_PIN_1, channelNumber & 2);
	digitalWrite(MUX_SELECT_PIN_2, channelNumber & 4);
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

void processMidiCC()
{

	midi::DataByte cc = (_loop_count % 128);
	MIDI.sendControlChange((byte) 20, cc, 4);

}