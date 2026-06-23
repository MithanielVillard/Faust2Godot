#include "inspector/VSliderEditorProperty.h"

void godot::VSliderEditorProperty::setup()
{
    m_pPropertyControl = memnew(VSlider);

    add_child(m_pPropertyControl);
    add_focusable(m_pPropertyControl);
    RefreshSliderValue();
}

void godot::VSliderEditorProperty::_update_property()
{
    double const newValue = get_edited_object()->get(get_edited_property());

    m_updating = true;
    m_currentValue = newValue;
    RefreshSliderValue();
    m_updating = false;
}

void godot::VSliderEditorProperty::RefreshSliderValue() const
{
    m_pPropertyControl->set_value(m_currentValue);
}
void godot::VSliderEditorProperty::_bind_methods(){}
