#include "GodotMapUI.h"
#include "AudioEffectFaust.h"

#include <godot_cpp/classes/global_constants.hpp>
#include <godot_cpp/core/property_info.hpp>
#include <godot_cpp/variant/variant.hpp>
#include <format>

using namespace godot;

GodotMapUI::GodotMapUI(AudioEffectFaust* effectRef) : m_pAudioEffect(effectRef) {}

void GodotMapUI::addVerticalSlider(const char* label, FAUSTFLOAT* zone, FAUSTFLOAT init, FAUSTFLOAT fmin, FAUSTFLOAT fmax, FAUSTFLOAT step)
{
    m_pAudioEffect->AddProperty(PropertyInfo(
        Variant::FLOAT,
        label,
        godot::PROPERTY_HINT_RANGE,
        std::format("{},{},{}", fmin, fmax, step).c_str()));

    m_pAudioEffect->_set(label, init);
}
