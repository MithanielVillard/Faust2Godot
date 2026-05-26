#pragma once
#include <faust/gui/MapUI.h>
#include <godot_cpp/classes/ref.hpp>

namespace godot { class AudioEffectFaust; }

class GodotMapUI : public MapUI
{
public:

    GodotMapUI(godot::AudioEffectFaust* effectRef);
    ~GodotMapUI() = default;

    void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step);

private:
    godot::AudioEffectFaust* m_pAudioEffect;
};
