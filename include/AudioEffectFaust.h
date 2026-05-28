#include "defines.h"

#include <godot_cpp/classes/audio_effect_instance.hpp>
#include <godot_cpp/classes/audio_effect.hpp>
#include <godot_cpp/core/property_info.hpp>

class GodotDsp;
class GodotMapUI;

namespace godot
{
    class AudioEffectFaust;

    class AudioEffectFaustInstance : public AudioEffectInstance
    {
    GDCLASS(AudioEffectFaustInstance, AudioEffectInstance)

    public:
        AudioEffectFaustInstance();
        ~AudioEffectFaustInstance() override;

        void _process(void const* pSrcFrames, AudioFrame* pDstFrames, int32 frameCount) override;

        bool _process_silence() const override;

    protected:
        static void _bind_methods();

    private:
        friend AudioEffectFaust;

        Ref<AudioEffectFaust> m_base;
        uptr<GodotDsp> m_dsp;

        //Hardcoded 2 inputs and outputs. Seems to be a godot hard limitation
        float* m_input[2] {};
        float* m_output[2] {};
    };


    class AudioEffectFaust : public AudioEffect
    {
    GDCLASS(AudioEffectFaust, AudioEffect)

    public:
        AudioEffectFaust();
        ~AudioEffectFaust() override;

        Ref<AudioEffectInstance> _instantiate() override;

        void AddProperty(PropertyInfo const& property);

    protected:
        static void _bind_methods();
       	bool _set(const StringName &p_path, const Variant &p_value);
        bool _get(const StringName &p_path, Variant &r_ret) const;
        void _get_property_list(List<PropertyInfo> *p_list) const;

       	friend class AudioEffectFaustInstance;
        friend class ::GodotMapUI;

    private:
        List<PropertyInfo> m_propertyList;
        uptr<GodotMapUI> m_dspUI;
    };

}
