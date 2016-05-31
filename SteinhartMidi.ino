#include "C:\Lucas\Studium\Bakk Arbeit\Workspace\libraries\MIDI\MIDI.h"
#include "C:\Lucas\Studium\Bakk Arbeit\Workspace\libraries\MIDI\midi_Settings.h"
#include "C:\Lucas\Studium\Bakk Arbeit\Workspace\libraries\MIDI\midi_Namespace.h"
#include "C:\Lucas\Studium\Bakk Arbeit\Workspace\libraries\MIDI\midi_Message.h"
#include "C:\Lucas\Studium\Bakk Arbeit\Workspace\libraries\MIDI\midi_Defs.h"
#include "State.h"
#include "SequencerEngine.h"
#include "DrumRackEngine.h"
#include "ArpeggiatorEngine.h"
#include "ActionButtonEngine.h"

USING_NAMESPACE_MIDI
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

#define SEQUENCER_MIDI_CHANNEL 5
#define DRUM_RACK_MIDI_CHANNEL 5
#define CC_MIDI_CHANNEL 4

#define C1_OFFSET 36

#define LED 13   		    // LED pin on Arduino Uno

#define ENCODER_R_LED_INDEX 8
#define ENCODER_G_LED_INDEX 9
#define ENCODER_B_LED_INDEX 10

#define SHIFT_LED_INDEX 11
#define PLAY_LED_INDEX 12
/*	LED SHIFT PINS   { 1, 2, 3, 4, 5, 6, 7, 8, 9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32 };
	Pins 0-7 
*/
const char _ccNumberByGlobalMuxPinID[32] = { 17, 25, 26, 18, 27, 19, 28, 20, 21, 29, 30, 22, 31, 23, 32, 24, 1, 9, 10, 2, 11, 3, 12, 4, 5, 13, 14, 6, 15, 7, 16, 8 };
const char _buttonIDByGlobalMuxPinID[32] = { 8,  0, 1, 9, 2, 10, 3, 11, 12,  4,  5, 13,  6, 14,  7, 15, 40,	 40,  40, 40,  16, 18, 19, 17, 20, 21, 40, 40, 40, 40, 40, 40 }; // todo  enter correct ids
//const char _ledIndexByID[32] =	{ 7, 5, 2, 1, 8, 13, 12, 9, 6, 4, 3, 0, 10, 15, 14, 11, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31 }; // todo update after Romans workover
const char _ledIndexByID[32] = { 23, 21, 18, 17, 24, 29, 28, 25, 22, 20, 19, 16, 26, 31, 30, 27, 0, 1, 2, 3, ENCODER_R_LED_INDEX, ENCODER_G_LED_INDEX, ENCODER_B_LED_INDEX, 4, 5, 6, 7, SHIFT_LED_INDEX, PLAY_LED_INDEX, 13, 14, 15 };


char _ledStates[32] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
unsigned long _buttonLongPressStartTime[16] = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };


unsigned int _selectedMuxChannel = 0;
unsigned int _selectedEngine = 0;
State _muxStates[8];

SequencerEngine _sequencer;
DrumRackEngine _drumRack;
ArpeggiatorEngine _arpEngine;

ActionButtonEngine _actionEngine;

bool _playing = false;

int _midiClockPosition = 0;
long _tempTime = 0;


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
	_muxStates[7].setBinary(true);			// make shiftbutton and playbutton mux binary

	MIDI.begin();	
	//Serial.begin(31250);
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
	_sequencer.buttonDown(1);	
	_sequencer.buttonDown(4);
	_sequencer.buttonDown(5);
	*/

	
	MIDI.setHandleClock(clockCallback);
	MIDI.setHandleStart(startCallback);
	MIDI.setHandleStop(stopCallback);
	MIDI.setHandleContinue(continueCallback);
	
	//MIDI.setHandleNoteOn(noteOnCallback);
	//MIDI.setInputChannel(1);	
}

void loop()
{
	MIDI.read();
	processCCValues();
	processButtonEventsAndStateSettings();
	processActionEngine();
	_selectedMuxChannel = (_selectedMuxChannel + 1) % 8;
	selectMuxChannel(_selectedMuxChannel);
	
	/*if (millis() - _tempTime > 20)
	{
		clockCallback();
		_tempTime = millis();
	}*/
	if (_selectedMuxChannel == 0)
	{
		bool shiftDown = (_muxStates[7].values[0] > 60);
		_sequencer.shiftDown = shiftDown;
		_drumRack.shiftDown = shiftDown;
		//_arpeggiator.shiftDown = shiftDown;
		char tempEngine = _selectedEngine;
		if (_muxStates[6].values[0] > 60)
		{
			_selectedEngine = 0;
			_sequencer.updateLedStates(_ledStates, _ledIndexByID);
		}
		else if (_muxStates[6].values[3] > 60)
		{
			_selectedEngine = 2;
			//_arpeggiator.updateLedStates(_ledStates);
		}
		else
		{
			_selectedEngine = 1;
			_drumRack.updateLedStates(_ledStates, _ledIndexByID);
		}
		if (tempEngine != _selectedEngine)
		{
			switch (tempEngine)
			{
			case 0:
				break;
			case 1:
			{
				releaseDrumRackNotes(_drumRack.releaseAll());
				break;
			}
			case 2:
				break;
			default:
				break;
			}
			/*Serial.print("new engine selected :");
			Serial.print((int)_selectedEngine);
			Serial.println();
			*/
		}
		updateGeneralLeds();		
		flushLedStates();
	}
}

void updateGeneralLeds()
{
	_actionEngine.updateLedStates(_ledStates, _ledIndexByID);
	_ledStates[ENCODER_R_LED_INDEX] = 0;
	_ledStates[ENCODER_G_LED_INDEX] = 0;
	_ledStates[ENCODER_B_LED_INDEX] = 0;
	_ledStates[SHIFT_LED_INDEX] = 0;
	switch (_selectedEngine)
	{
	case 0:
		_ledStates[ENCODER_R_LED_INDEX] = 1;
		if (_sequencer.shiftDown)
		{
			_ledStates[ENCODER_B_LED_INDEX] = 2;
			_ledStates[SHIFT_LED_INDEX] = 1;
		}
		break;
	case 1:
		_ledStates[ENCODER_G_LED_INDEX] = 1;
		if (_drumRack.shiftDown)
		{
			_ledStates[ENCODER_R_LED_INDEX] = 2;
			_ledStates[SHIFT_LED_INDEX] = 1;
		}
		break;
	case 2:
		_ledStates[ENCODER_B_LED_INDEX] = 1;
		if (_drumRack.shiftDown) // //if (_arpeggiator.shiftDown)
		{
			_ledStates[ENCODER_G_LED_INDEX] = 2;
			_ledStates[SHIFT_LED_INDEX] = 1;
		}
		break;
	default:
		break;
	}
	_ledStates[PLAY_LED_INDEX] = (_playing) ? 1 : 0;
}

void processButtonEventsAndStateSettings()
{
	for (int i = 4; i <= 7; i++)  //   parse mux 4-7 (binary muxes: Buttons and Tristate)
	{
		int globalButtonId = _buttonIDByGlobalMuxPinID[(i - 4) * 8 + _selectedMuxChannel];		
		int buttonAction = _muxStates[i].updateValue(_selectedMuxChannel);
		if (buttonAction != -1) // buttonAction -1: nothing, 0: buttonUp, 127: buttonDown
		{		
			if (i == 6)
			{
				// action button engine Potis
				if(_selectedMuxChannel == 1)
				{ 				
					_actionEngine.value1Change(buttonAction);					
					continue;
				}
				else if (_selectedMuxChannel == 2)
				{
					_actionEngine.value2Change(buttonAction);					
					continue;
				}

			}
			if (buttonAction == 0)
			{							
				//Serial.print("buttonUp : "); Serial.print(globalButtonId); Serial.print(" mux"); Serial.print(i); Serial.print(" channel"); Serial.println((int) _selectedMuxChannel);
				if (globalButtonId < 16) // sequencer-button
				{
					switch (_selectedEngine)
					{
					case 0: // Sequencer
						//_sequencer.buttonUp(globalButtonId);
						break;
					case 1: // Drumrack
					{
						releaseDrumRackNotes(_drumRack.buttonUp(globalButtonId));
						break;
					}
					case 2: // Arpeggiator

						break;
					default:
						break;
					}
					_buttonLongPressStartTime[globalButtonId] = 0;
				}else
				{					
				}
			}
			else if (buttonAction > 0)
			{
				/*Serial.print("buttonDown : "); Serial.print(globalButtonId); Serial.print(" mux"); Serial.print(i); Serial.print(" channel"); Serial.println((int) _selectedMuxChannel);*/
				if (globalButtonId < 16) // sequencer-button
				{
					switch (_selectedEngine)
					{
					case 0: // Sequencer
						_sequencer.buttonDown(globalButtonId);
						break;
					case 1: // Drumrack
					{
						char drumRackPressMessage = _drumRack.buttonDown(globalButtonId);
						MIDI.sendNoteOn((midi::DataByte) drumRackPressMessage, (midi::DataByte) _drumRack.velocity, (midi::Channel) DRUM_RACK_MIDI_CHANNEL);
						break;
					}
					case 2: // Arpeggiator

						break;
					default:
						break;
					}
					_buttonLongPressStartTime[globalButtonId] = millis();
				}
				else
				{
					//Serial.println(globalButtonId);
					if (globalButtonId == 21)
					{
						_playing = !_playing;
						if (_playing)
							MIDI.sendRealTime((midi::MidiType) midi::Start);
						else
							MIDI.sendRealTime((midi::MidiType) midi::Stop);
					}
					else
					{
						_actionEngine.buttonDown(globalButtonId);
						//Serial.println(globalButtonId);
					}
				}
			}
		}

		if ((globalButtonId < 16) && (_buttonLongPressStartTime[globalButtonId] > 0)) // sequencer-button is down and we have to detect a longpress event
		{
			if ((unsigned long)(millis() - _buttonLongPressStartTime[globalButtonId]) >= 1500)
			{
				_buttonLongPressStartTime[globalButtonId] = 0;
				// button longPressEvent
				if(_selectedEngine == 0)
				{
					_sequencer.buttonLongPress(globalButtonId); // clear sequence
				}
				//Serial.print("buttonLongPress : "); Serial.println(globalButtonId);
			}
		}
	}
}

void releaseDrumRackNotes(char* releaseMessages)
{
	int i = 0;
	while (releaseMessages[i] >= 0)
	{
		MIDI.sendNoteOff((midi::DataByte) releaseMessages[i], (midi::DataByte) _drumRack.velocity, (midi::Channel) DRUM_RACK_MIDI_CHANNEL);
		++i;
	}
}

void processCCValues()
{
	for (int i = 0; i <= 3; i++)  //   parse mux 0-3 
	{
		int newValue = _muxStates[i].updateValue(_selectedMuxChannel);
		if (newValue != -1)
		{
			midi::DataByte ccNumber = (midi::DataByte) (_ccNumberByGlobalMuxPinID[i * 8 + _selectedMuxChannel]);
			MIDI.sendControlChange(ccNumber, (midi::DataByte) newValue, (midi::Channel) CC_MIDI_CHANNEL);
			//Serial.print("mux"); Serial.print(i); Serial.print(" pin"); Serial.print(int(_selectedMuxChannel)); Serial.print(": "); Serial.println(ccNumber);			
		}
	}
}

void processActionEngine()
{
	if (_actionEngine.active)
	{
		char actionCC[4];
		_actionEngine.getCCValues(actionCC);
		for (size_t i = 0; i < 4; i++)
		{
			if (actionCC[i] != -1)
			{
				//Serial.print(millis()); Serial.print(";"); Serial.println((int)actionCC[i]);				
				MIDI.sendControlChange((midi::DataByte) (33 + i), (midi::DataByte) actionCC[i], (midi::Channel) CC_MIDI_CHANNEL);
			}
		}
	}
}

void updateRotaryEncoder() 
{
	int val = (digitalRead(ENCODER_PIN_A) == digitalRead(ENCODER_PIN_B)) ? -1 : 1;	
	switch (_selectedEngine)
	{
		case 0:
			_sequencer.encoderValueChange(val);
			break;
		case 1:
			_drumRack.encoderValueChange(val);
			break;
		case 2:
			break;
		default:
			break;
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
		bool val = _ledStates[outputIndex];
		if (_ledStates[outputIndex] >= 2)
		{
			val = ((int)(millis()*(int)_ledStates[outputIndex] / 1000) % 2);
		}
		if ((8 <= outputIndex) && (outputIndex <= 10))
			digitalWrite(LED_DATA, !val);
		else
			digitalWrite(LED_DATA, val);
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
	_midiClockPosition++; 
	if(_midiClockPosition % 3 == 0)
	{
	// todo make some tuple class for the "message" with note value, and message
		char msg[16];
		_sequencer.setSequencePosition(_midiClockPosition / 3);
		_sequencer.getMessagesAtCurrentPosition(msg);
		for (size_t i = 0; i < 16; i++)
		{
			if ((msg[i] == Sequence::NOTE_PLAY) || (msg[i] == Sequence::NOTE_REPLAY))
			{
				/*if (i <= 3)  // debug noteplaying
				{
					_ledStates[4 + i] = 1;
				}*/
				digitalWrite(LED, 1);

				MIDI.sendNoteOn(i + C1_OFFSET, 127, (midi::Channel) SEQUENCER_MIDI_CHANNEL);
			}
			else if (msg[i] == Sequence::NOTE_STOP)
			{
				/*if (i <= 3)  // debug noteplaying
				{
					_ledStates[4 + i] = 0;
				}*/
				digitalWrite(LED, 0);

				MIDI.sendNoteOff(i + C1_OFFSET, 127, (midi::Channel) SEQUENCER_MIDI_CHANNEL);
			}
		}
	}
	
	//Serial.write(_midiClockPosition);
	/*if (_midiClockPosition % 24 == 0)
	{	
		if (_notePos >= 0)
			MIDI.sendNoteOff(_notes[_notePos], 0, 1);   // Stop the note	
		_notePos = (_notePos + 1) % 8;
		MIDI.sendNoteOn(_notes[_notePos], 127, 1);  // Send a Note (pitch 42, velo 127 on channel 1)				
	}*/
}

void startCallback()
{
	_midiClockPosition = -1;
	//clockCallback();
	_playing = true;
}

void continueCallback()
{
	_playing = true;
}

void stopCallback()
{
	_playing = false;
	_midiClockPosition = 0;
	_sequencer.setSequencePosition(-1);
}