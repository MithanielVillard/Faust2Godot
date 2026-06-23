#pragma once

#include <godot_cpp/classes/editor_property.hpp>
#include <godot_cpp/classes/v_slider.hpp>

namespace godot
{
    class VSlider;

    class VSliderEditorProperty : public EditorProperty
    {
    GDCLASS(VSliderEditorProperty, EditorProperty)

    public:
        void setup();
        void _update_property() override;

    protected:
        static void _bind_methods();
        void RefreshSliderValue() const;

    private:
        VSlider* m_pPropertyControl { nullptr};
        double m_currentValue { 0 };
        bool m_updating { false };
    };

}
