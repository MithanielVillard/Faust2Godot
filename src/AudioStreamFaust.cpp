#include "AudioStreamFaust.h"

using namespace godot;

AudioStreamFaust::AudioStreamFaust()
{
    m_dspUI = std::make_unique<GodotMapUI>(this);
    m_dsp   = std::make_unique<GodotDsp>();
    m_dsp->buildUserInterface(m_dspUI.get());
}

Ref<AudioStreamPlayback> AudioStreamFaust::_instantiate_playback() const
{
    Ref<AudioStreamPlaybackFaust> instance;
    instance.instantiate();
    instance->m_base = Ref<AudioStreamFaust>(this);

    return instance;
}

void AudioStreamFaust::AddProperty(PropertyInfo const& property)
{
    m_propertyList.push_back(property);
}

void AudioStreamFaust::SetProperty(StringName const& name, Variant const& value)
{
    _set(name, value);
}

std::optional<Variant> AudioStreamFaust::GetProperty(StringName const& name)
{
    Variant out;
    return _get(name, out) ? out : std::optional<Variant>();
}

List<PropertyInfo>& AudioStreamFaust::GetPropertyList()
{
    return m_propertyList;
}

String AudioStreamFaust::_get_stream_name() const
{
    return "AudioStreamFaust";
}

void AudioStreamFaust::_bind_methods()
{
}

bool AudioStreamFaust::_set(const StringName &p_path, const Variant &p_value)
{
    if (p_path == String("resource_name")) return false;
    if (p_path == String("script")) return false;

    if (m_dspUI)
    {
        m_dspUI->setParamValue(String(p_path).utf8().get_data(), p_value);
        return true;
    }
    return false;
}

bool AudioStreamFaust::_get(const StringName &p_path, Variant &r_ret) const
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

void AudioStreamFaust::_get_property_list(List<PropertyInfo> *p_list) const
{
    *p_list = m_propertyList;
}


AudioStreamPlaybackFaust::AudioStreamPlaybackFaust()
{
    m_output[0] = new float[m_bufferSize];
    m_output[1] = new float[m_bufferSize];
}

AudioStreamPlaybackFaust::~AudioStreamPlaybackFaust()
{
    delete[] m_output[0];
    delete[] m_output[1];
}

int32_t AudioStreamPlaybackFaust::_mix(AudioFrame* p_buffer, float p_rate_scale, int32_t p_frames)
{
    if (!m_active) return 0;

    // for (int i = 0; i < p_frames; i++) {
    //     float res = sin(2.0 * Math_PI * double(m_pos + i) / (double(m_base->m_sampleRate) / 440));
    //     AudioFrame const frame(res, res);
    //     p_buffer[i] = frame;
    // }
    //
    // m_pos += p_frames;

    m_base->m_dsp->Compute(p_frames, nullptr, m_output);

    for (int i = 0; i < p_frames; i++)
    {
        p_buffer[i].left  = m_output[0][i];
        p_buffer[i].right = m_output[1][i];
    }

    return p_frames;
}

void AudioStreamPlaybackFaust::_bind_methods() {}
