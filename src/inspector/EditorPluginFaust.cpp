#include "inspector/EditorPluginFaust.h"

void godot::EditorPluginFaust::_enter_tree()
{
    m_inspectorPlugin.instantiate();
    add_inspector_plugin(m_inspectorPlugin);
}

void godot::EditorPluginFaust::_exit_tree()
{
    remove_inspector_plugin(m_inspectorPlugin);
    m_inspectorPlugin.unref();
}
