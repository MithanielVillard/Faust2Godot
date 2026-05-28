#include "GodotDsp.h"

#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "godot_cpp/classes/audio_server.hpp"

using namespace godot;

GodotDsp::GodotDsp()
{
    if (auto res = m_dspLib.Open("bin/linux/faustdsp"))
    {
        UtilityFunctions::printerr("Error while loading the lib faust dsp dynamic library : ", res.value().c_str());
        UtilityFunctions::print("Current working directory : ", std::filesystem::current_path().string().c_str());
        return;
    }

    auto factory = m_dspLib.GetFunction<dsp*()>("DspFactory");
    m_dspInstance = factory();

    m_dspInstance->init(AudioServer::get_singleton()->get_mix_rate());
}

GodotDsp::~GodotDsp()
{
    delete m_dspInstance;
}
