extends Panel

@export var key_id : int


func _on_midi_handler_faust_midi_received(MidiEvent: Variant) -> void:
	if MidiEvent.message == MIDI_MESSAGE_NOTE_ON and MidiEvent.pitch == key_id:
		modulate = Color(0.0, 0.5, 1.0)
	elif MidiEvent.message == MIDI_MESSAGE_NOTE_OFF and MidiEvent.pitch == key_id:
		modulate = Color(1, 1, 1)
