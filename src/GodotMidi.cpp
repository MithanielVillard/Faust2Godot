#include "GodotMidi.h"
#include "MidiHandlerFaust.h"

#include <godot_cpp/classes/input_event_midi.hpp>

using namespace godot;

GodotMidi::GodotMidi(): midi_handler("GodotMidi") {}

bool GodotMidi::startMidi()
{
    MidiHandlerFaust* midi_handler = MidiHandlerFaust::GetInstance();
    if (!midi_handler) return false;

    midi_handler->MidiCallbacks.emplace_back(
        [&](const InputEventMIDI* eventMidi){ OnMidiReceived(eventMidi); }
    );

    m_midiListIt = midi_handler->MidiCallbacks.end();
    return true;
}

void GodotMidi::stopMidi()
{
    MidiHandlerFaust* midi_handler = MidiHandlerFaust::GetInstance();
    if (!midi_handler) return;

    midi_handler->MidiCallbacks.erase(m_midiListIt);
}

void GodotMidi::OnMidiReceived(InputEventMIDI const* eventMidi)
{
    switch (eventMidi->get_message())
    {
    default:
        break;
    case MIDI_MESSAGE_NOTE_ON:
        handleKeyOn(0, eventMidi->get_channel(), eventMidi->get_pitch(), eventMidi->get_velocity());
        break;
    case MIDI_MESSAGE_NOTE_OFF:
        handleKeyOff(0, eventMidi->get_channel(), eventMidi->get_pitch(), eventMidi->get_velocity());
        break;
    case MIDI_MESSAGE_PITCH_BEND:
        handlePitchWheel(0, eventMidi->get_channel(), eventMidi->get_pitch());
        break;
    case MIDI_MESSAGE_AFTERTOUCH:
    case MIDI_MESSAGE_CHANNEL_PRESSURE:
        handleAfterTouch(0, eventMidi->get_channel(), eventMidi->get_pressure());
        break;
    case MIDI_MESSAGE_CONTROL_CHANGE:
        handleCtrlChange(0, eventMidi->get_channel(), eventMidi->get_controller_number(), eventMidi->get_controller_value());
        break;
    case MIDI_MESSAGE_START:
        handleStart(0);
        break;
    case MIDI_MESSAGE_STOP:
        handleStop(0);
        break;
    case MIDI_MESSAGE_PROGRAM_CHANGE:
        handleProgChange(0, eventMidi->get_channel(), eventMidi->get_instrument());
        break;
    case MIDI_MESSAGE_SYSTEM_EXCLUSIVE:
        //GODOT cannot get sysex message
        std::vector<unsigned char> temp;
        handleSysex(0, temp);
        break;
    }
}
