from __future__ import with_statement
import Live
from _Framework.ControlSurface import ControlSurface
from _Framework.DeviceComponent import DeviceComponent
from _Framework.MixerComponent import MixerComponent # Class encompassing several channel strips to form a mixer
from _Framework.SliderElement import SliderElement
from _Framework.TransportComponent import TransportComponent
from _Framework.InputControlElement import *
from _Framework.ButtonElement import ButtonElement
from _Framework.ButtonMatrixElement import ButtonMatrixElement
from _Framework.SessionComponent import SessionComponent

class steinhartmidi(ControlSurface):

	def __init__(self, c_instance):
		super(steinhartmidi, self).__init__(c_instance)
		#with self.component_guard():
		self.current_track_offset = 0
		self.current_scene_offset = 0
		global active_mode
		active_mode = "_mode1"
		self._set_active_mode()
		self.show_message("Have fun with SteinhartMIDI!")

	def _mode1(self):
		self.show_message("_mode1 is active")
		# mixer
		global mixer
		num_tracks = 32
		num_returns = 7
		self.mixer = MixerComponent(num_tracks, num_returns)
		# sends
		send0_controls = (
			SliderElement(MIDI_CC_TYPE, 3, 17),
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
		);
		self.mixer.channel_strip(0).set_send_controls(tuple(send0_controls))
		send1_controls = (
			SliderElement(MIDI_CC_TYPE, 3, 18),
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
		);
		self.mixer.channel_strip(1).set_send_controls(tuple(send1_controls))
		send2_controls = (
			SliderElement(MIDI_CC_TYPE, 3, 19),
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
		);
		self.mixer.channel_strip(2).set_send_controls(tuple(send2_controls))
		send3_controls = (
			SliderElement(MIDI_CC_TYPE, 3, 20),
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
		);
		self.mixer.channel_strip(3).set_send_controls(tuple(send3_controls))
		send4_controls = (
			SliderElement(MIDI_CC_TYPE, 3, 21),
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
		);
		self.mixer.channel_strip(4).set_send_controls(tuple(send4_controls))
		send5_controls = (
			SliderElement(MIDI_CC_TYPE, 3, 22),
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
		);
		self.mixer.channel_strip(5).set_send_controls(tuple(send5_controls))
		send6_controls = (
			SliderElement(MIDI_CC_TYPE, 3, 23),
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
		);
		self.mixer.channel_strip(6).set_send_controls(tuple(send6_controls))
		send7_controls = (
			SliderElement(MIDI_CC_TYPE, 3, 24),
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
		);
		self.mixer.channel_strip(7).set_send_controls(tuple(send7_controls))
		self.mixer.channel_strip(0).set_volume_control(SliderElement(MIDI_CC_TYPE, 3, 1))
		self.mixer.channel_strip(1).set_volume_control(SliderElement(MIDI_CC_TYPE, 3, 2))
		self.mixer.channel_strip(2).set_volume_control(SliderElement(MIDI_CC_TYPE, 3, 3))
		self.mixer.channel_strip(2).set_volume_control(SliderElement(MIDI_CC_TYPE, 3, 3))
		self.mixer.channel_strip(3).set_volume_control(SliderElement(MIDI_CC_TYPE, 3, 4))
		self.mixer.channel_strip(4).set_volume_control(SliderElement(MIDI_CC_TYPE, 3, 5))
		self.mixer.channel_strip(5).set_volume_control(SliderElement(MIDI_CC_TYPE, 3, 6))
		self.mixer.channel_strip(6).set_volume_control(SliderElement(MIDI_CC_TYPE, 3, 7))
		self.mixer.channel_strip(7).set_volume_control(SliderElement(MIDI_CC_TYPE, 3, 8))
		self.mixer.channel_strip(0).set_pan_control(SliderElement(MIDI_CC_TYPE, 3, 9))
		self.mixer.channel_strip(1).set_pan_control(SliderElement(MIDI_CC_TYPE, 3, 10))
		self.mixer.channel_strip(2).set_pan_control(SliderElement(MIDI_CC_TYPE, 3, 11))
		self.mixer.channel_strip(3).set_pan_control(SliderElement(MIDI_CC_TYPE, 3, 12))
		self.mixer.channel_strip(4).set_pan_control(SliderElement(MIDI_CC_TYPE, 3, 13))
		self.mixer.channel_strip(5).set_pan_control(SliderElement(MIDI_CC_TYPE, 3, 14))
		self.mixer.channel_strip(6).set_pan_control(SliderElement(MIDI_CC_TYPE, 3, 15))
		self.mixer.channel_strip(7).set_pan_control(SliderElement(MIDI_CC_TYPE, 3, 16))
		#self.mixer.channel_strip(0).set_mute_button(ButtonElement(True, MIDI_CC_TYPE, 3, 25))
		#self.mixer.channel_strip(1).set_mute_button(ButtonElement(True, MIDI_CC_TYPE, 3, 26))
		#self.mixer.channel_strip(2).set_mute_button(ButtonElement(True, MIDI_CC_TYPE, 3, 27))
		#self.mixer.channel_strip(3).set_mute_button(ButtonElement(True, MIDI_CC_TYPE, 3, 28))
		#self.mixer.channel_strip(4).set_mute_button(ButtonElement(True, MIDI_CC_TYPE, 3, 29))
		#self.mixer.channel_strip(5).set_mute_button(ButtonElement(True, MIDI_CC_TYPE, 3, 30))
		#self.mixer.channel_strip(6).set_mute_button(ButtonElement(True, MIDI_CC_TYPE, 3, 31))
		#self.mixer.channel_strip(7).set_mute_button(ButtonElement(True, MIDI_CC_TYPE, 3, 32))

	def _remove_mode1(self):
		# mixer
		global mixer
		send0_controls = (
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
		);
		self.mixer.channel_strip(0).set_send_controls(tuple(send0_controls))
		send1_controls = (
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
		);
		self.mixer.channel_strip(1).set_send_controls(tuple(send1_controls))
		send2_controls = (
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
		);		
		self.mixer.channel_strip(2).set_send_controls(tuple(send2_controls))
		send3_controls = (
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
		);
		self.mixer.channel_strip(3).set_send_controls(tuple(send3_controls))
		send4_controls = (
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
		);
		self.mixer.channel_strip(4).set_send_controls(tuple(send4_controls))
		send5_controls = (
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
		);
		self.mixer.channel_strip(5).set_send_controls(tuple(send5_controls))
		send6_controls = (
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
		);
		self.mixer.channel_strip(6).set_send_controls(tuple(send6_controls))
		send7_controls = (
			None,
			None,
			None,
			None,
			None,
			None,
			None,
			None,
		);
		self.mixer.channel_strip(7).set_send_controls(tuple(send7_controls))
		self.mixer.channel_strip(0).set_pan_control(None)
		self.mixer.channel_strip(1).set_pan_control(None)
		self.mixer.channel_strip(2).set_pan_control(None)
		self.mixer.channel_strip(3).set_pan_control(None)
		self.mixer.channel_strip(4).set_pan_control(None)
		self.mixer.channel_strip(5).set_pan_control(None)
		self.mixer.channel_strip(6).set_pan_control(None)
		self.mixer.channel_strip(7).set_pan_control(None)
		self.mixer.channel_strip(0).set_volume_control(None)
		self.mixer.channel_strip(1).set_volume_control(None)
		self.mixer.channel_strip(2).set_volume_control(None)
		self.mixer.channel_strip(2).set_volume_control(None)
		self.mixer.channel_strip(3).set_volume_control(None)
		self.mixer.channel_strip(4).set_volume_control(None)
		self.mixer.channel_strip(5).set_volume_control(None)
		self.mixer.channel_strip(6).set_volume_control(None)
		self.mixer.channel_strip(7).set_volume_control(None)
		#self.mixer.channel_strip(0).set_mute_button(None)
		#self.mixer.channel_strip(1).set_mute_button(None)
		#self.mixer.channel_strip(2).set_mute_button(None)
		#self.mixer.channel_strip(3).set_mute_button(None)
		#self.mixer.channel_strip(4).set_mute_button(None)
		#self.mixer.channel_strip(5).set_mute_button(None)
		#self.mixer.channel_strip(6).set_mute_button(None)
		#self.mixer.channel_strip(7).set_mute_button(None)

	def _set_active_mode(self):
		global active_mode
		# activate mode
		if active_mode == "_mode1":
			self._mode1()

	def _remove_active_mode(self):
		global active_mode
		# remove activate mode
		if active_mode == "_mode1":
			self._remove_mode1()

	def _activate_mode1(self,value):
		global active_mode
		if value > 0:
			self._remove_active_mode()
			active_mode = "_mode1"
			self._set_active_mode()

	def _activate_shift_mode1(self,value):
		global active_mode
		if value > 0:
			self._remove_active_mode()
			self._mode1()
		else:
			self._remove_mode1()
			self._set_active_mode()

	def tuple_index(self, tuple, obj):
		for i in xrange(0, len(tuple)):
			if (tuple[i] == obj):
				return i
		return(False)

	def disconnect(self):
		super(steinhartmidi, self).disconnect()
