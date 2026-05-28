#include "AudioEffectFaust.h"
#include "GodotDsp.h"
#include "GodotMapUI.h"

#include <godot_cpp/core/property_info.hpp>

using namespace godot;

AudioEffectFaust::AudioEffectFaust()
{
    m_dspUI = std::make_unique<GodotMapUI>(this);
}

AudioEffectFaust::~AudioEffectFaust() = default;

Ref<AudioEffectInstance> AudioEffectFaust::_instantiate()
{
    Ref<AudioEffectFaustInstance> ins;
    ins.instantiate();

    ins->m_base  = Ref<AudioEffectFaust>(this);
    ins->m_dsp   = std::make_unique<GodotDsp>();
    ins->m_dsp->buildUserInterface(m_dspUI.get());

    return ins;
}

void AudioEffectFaust::AddProperty(PropertyInfo const& property)
{
    m_propertyList.push_back(property);
}

void AudioEffectFaust::_bind_methods() {}

bool AudioEffectFaust::_set(const StringName &p_path, const Variant &p_value)
{
    if (p_path == String("resource_name")) return false;

    if (m_dspUI)
    {
        m_dspUI->setParamValue(String(p_path).utf8().get_data(), p_value);
        return true;
    }
    return false;
}

bool AudioEffectFaust::_get(const StringName &p_path, Variant &r_ret) const
{
    if (p_path == String("resource_path")) return false;
    if (p_path == String("resource_name")) return false;
    if (p_path == String("resource_local_to_scene")) return false;
    if (p_path == String("script")) return false;

    if (m_dspUI)
    {
        r_ret = m_dspUI->getParamValue(String(p_path).utf8().get_data());
        return true;
    }
    return false;
}

void AudioEffectFaust::_get_property_list(List<PropertyInfo> *p_list) const
{
    *p_list = m_propertyList;
}

//====== CALL FAUST PROCESS =======

AudioEffectFaustInstance::AudioEffectFaustInstance()
{
    m_input[0] = new float[1024];
    m_input[1] = new float[1024];

    m_output[0] = new float[1024];
    m_output[1] = new float[1024];
}

AudioEffectFaustInstance::~AudioEffectFaustInstance()
{
    delete[] m_input[0];
    delete[] m_input[1];

    delete[] m_output[0];
    delete[] m_output[1];
}

void AudioEffectFaustInstance::_process(void const* pSrcFrames, AudioFrame* pDstFrames, int32 frameCount)
{
    AudioFrame* src = (AudioFrame*)pSrcFrames;

    for (int i = 0; i < frameCount; i++)
    {
         m_input[0][i] = src[i].left;
         m_input[1][i] = src[i].right;
    }

    m_dsp->Compute(frameCount, m_input, m_output);

    for (int i = 0; i < frameCount; i++)
    {
        pDstFrames[i].left  = m_output[0][i];
        pDstFrames[i].right = m_output[1][i];
    }
}

bool AudioEffectFaustInstance::_process_silence() const
{
    return true;
}

void AudioEffectFaustInstance::_bind_methods() {}
