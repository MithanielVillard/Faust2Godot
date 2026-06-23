#include "GodotDsp.h"

#include <godot_cpp/classes/audio_server.hpp>

using namespace godot;

GodotDsp::GodotDsp()
{
    if (auto const res = m_dynLibrary.Open("bin/faustdsp"))
    {
        UtilityFunctions::printerr("Error while loading the lib faust dsp dynamic library : ", res.value().c_str());
        return;
    }

    auto const factory = m_dynLibrary.GetFunction<dsp*()>("DspFactory");
    m_dspInstance = factory();

    m_dspInstance->init(static_cast<int>(AudioServer::get_singleton()->get_mix_rate()));
}

GodotDsp::~GodotDsp()
{
    delete m_dspInstance;
}
