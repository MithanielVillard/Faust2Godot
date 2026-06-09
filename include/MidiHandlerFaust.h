#pragma once

#include <functional>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/input_event.hpp>

namespace godot
{
    class InputEventMIDI;

    class MidiHandlerFaust : public Node
    {
    GDCLASS(MidiHandlerFaust, Node)

    public:
        MidiHandlerFaust() = default;
        ~MidiHandlerFaust() override = default;

        void _ready() override;

        void _input(const Ref<InputEvent>& p_event) override;
        void _exit_tree() override;

        static MidiHandlerFaust* GetInstance() { return m_pInstance; }

        std::function<void(InputEventMIDI*)> MidiCallback;

    protected:
        static void _bind_methods();

        void HandleMidiInput(InputEventMIDI* midiEvent);

        inline static MidiHandlerFaust* m_pInstance;
    };

}

