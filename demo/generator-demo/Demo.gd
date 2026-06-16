extends MidiHandlerFaust

@export var frequency_slider: HSlider
@export var frequency_label: Label

@export var volume_slider: HSlider
@export var volume_label: Label

@export var audio_player: AudioStreamPlayer3D
	
func _on_midi_received(MidiEvent):
	frequency_slider.value = audio_player.stream.frequency
	frequency_label.text = str(audio_player.stream.frequency).pad_decimals(2)
	
	volume_slider.value = audio_player.stream.volume
	volume_label.text = str(audio_player.stream.volume).pad_decimals(2)


func _on_check_button_toggled(toggled_on: bool) -> void:
	audio_player.playing = toggled_on


func _on_volume_slider_changed(value: float) -> void:
	pass
