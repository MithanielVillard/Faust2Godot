#pragma once

#include <godot_cpp/classes/input_event_midi.hpp>
#include <faust/midi/midi.h>

using namespace godot;

class GodotMidi : public midi_handler
{
public:
    GodotMidi();
    ~GodotMidi() override = default;

    bool startMidi() override { return true; };
    void stopMidi() override {};

    void OnMidiReceived(const InputEventMIDI* eventMidi);
};
