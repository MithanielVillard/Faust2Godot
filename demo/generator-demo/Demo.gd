extends MidiHandlerFaust

@export var frequency_slider: HSlider
@export var frequency_label: Label

@export var volume_slider: HSlider
@export var volume_label: Label

@export var audio_player: AudioStreamPlayer3D
var dsp : AudioStreamFaust

func _ready() -> void:
	dsp = audio_player.stream
	
	
func _on_midi_received(MidiEvent):
	# the dsp automatically update the parameters with the midi input
	# here we just have to update the UI to reflect those change
	frequency_slider.value = dsp.frequency
	frequency_label.text = str(dsp.frequency).pad_decimals(2)
	
	volume_slider.value = dsp.volume
	volume_label.text = str(dsp.volume).pad_decimals(2)


func _on_check_button_toggled(toggled_on: bool) -> void:
	audio_player.playing = toggled_on
	

func _on_volume_slider_changed(value: float) -> void:
	dsp.volume = value
	volume_label.text = str(dsp.volume).pad_decimals(2)



func _on_frequency_slider_value_changed(value: float) -> void:
	dsp.frequency = value
	frequency_label.text = str(dsp.frequency).pad_decimals(2)
