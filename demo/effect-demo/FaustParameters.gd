extends MarginContainer

var bus_idx 
var effect_idx
var audio_player: AudioStreamPlayer

# Called when the node enters the scene tree for the first time.
func _ready() -> void:
	audio_player = get_node("/root/Main/AudioStreamPlayer")
	bus_idx = AudioServer.get_bus_index("Faust")
	effect_idx = AudioServer.get_bus_effect(bus_idx, 0)


# Called every frame. 'delta' is the elapsed time since the previous frame.
func _process(delta: float) -> void:
	pass

func change_feedback(feedback: float) -> void:
	effect_idx.feedback = feedback
	
func change_milliseconds(milli: float) -> void:
	effect_idx.millisecond = milli
	
func play() -> void:
	audio_player.play()
