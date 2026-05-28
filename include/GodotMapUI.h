#pragma once
#include <faust/gui/MapUI.h>

namespace godot { class AudioEffectFaust; }

class GodotMapUI : public MapUI
{
public:

    GodotMapUI(godot::AudioEffectFaust* effectRef);
    ~GodotMapUI() override = default;

    void addButton(const char* label, float* zone) override;
    void addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) override;
    void addHorizontalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT min, FAUSTFLOAT max, FAUSTFLOAT step) override;

private:
    godot::AudioEffectFaust* m_pAudioEffect;
};
