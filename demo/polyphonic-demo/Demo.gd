extends MidiHandlerFaust

@export var audio_player: AudioStreamPlayer3D
	
func _on_midi_received(MidiEvent):
	print(MidiEvent)


func _on_check_button_toggled(toggled_on: bool) -> void:
	audio_player.playing = toggled_on


func _on_volume_slider_changed(value: float) -> void:
	pass
