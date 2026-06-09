#include "MidiHandlerFaust.h"

#include <godot_cpp/classes/input_event_midi.hpp>
#include <godot_cpp/classes/os.hpp>

#include "godot_cpp/classes/input.hpp"

using namespace godot;


void MidiHandlerFaust::_ready()
{
    if (m_pInstance)
    {
        UtilityFunctions::push_warning("Multiple Midi Handler detected on the scene. Only one is necessary, other are ignored.");
        return;
    }

    m_pInstance = this;
    OS::get_singleton()->open_midi_inputs();
    UtilityFunctions::print("Midi inputs : ", OS::get_singleton()->get_connected_midi_inputs());
}

void MidiHandlerFaust::_input(const Ref<InputEvent>& p_event)
{
    if (!m_pInstance) return;
    if (!p_event->is_class("InputEventMidi")) return;

    InputEventMIDI* event = static_cast<InputEventMIDI*>(p_event.ptr());

    HandleMidiInput(event);
    MidiCallback(event);
    emit_signal("midi_received", event);
}

void MidiHandlerFaust::_exit_tree()
{
    OS::get_singleton()->close_midi_inputs();
}

void MidiHandlerFaust::HandleMidiInput(InputEventMIDI* midiEvent)
{
    UtilityFunctions::print("Channel {}", midiEvent->get_channel());
    UtilityFunctions::print("Message {}", midiEvent->get_message());
}

void MidiHandlerFaust::_bind_methods()
{
    ADD_SIGNAL(MethodInfo("midi_received", PropertyInfo(Variant::OBJECT, "MidiEvent")));
}



