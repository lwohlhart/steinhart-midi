

#include <midi_Settings.h>
#include <midi_Namespace.h>
#include <midi_Message.h>
#include <midi_Defs.h>
#include <MIDI.h>

MIDI_CREATE_DEFAULT_INSTANCE();

#define LED 13   		    // LED pin on Arduino Uno

int loop_count = 0;
void setup()
{
	pinMode(LED, OUTPUT);
	MIDI.begin(4);          // Launch MIDI and listen to channel 4	
}

void loop()
{
	//MIDI.sendNoteOn(36, 127, 1);  // Send a Note (pitch 42, velo 127 on channel 1)		
	//MIDI.sendNoteOff(36, 0, 1);   // Stop the note	
	delay(1);	
	midi::DataByte cc = (loop_count % 128);
	/*int mask = 128;
	for (size_t i = 0; i < 8; i++)
	{
		Serial.print((cc&mask) ? 1 : 0);
		mask >>= 1;
	}*/
	MIDI.sendControlChange((byte) 20, cc, 4);
	//Serial.println(cc);
	loop_count++;
	
}