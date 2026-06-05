#include "GodotDsp.h"
#include "faust2godot.h"

#include <godot_cpp/classes/audio_server.hpp>

using namespace godot;

GodotDsp::GodotDsp()
{
    auto factory = GetFaustDSP().GetFunction<dsp*()>("DspFactory");
    m_dspInstance = factory();

    m_dspInstance->init(static_cast<int>(AudioServer::get_singleton()->get_mix_rate()));
}

GodotDsp::~GodotDsp()
{
    delete m_dspInstance;
}
