#include "faust2godot.h"
#include "AudioEffectFaust.h"
#include "AudioStreamFaust.h"

#include <gdextension_interface.h>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

using namespace godot;

void InitializeFaust2GodotModule(ModuleInitializationLevel level)
{
    if (level == MODULE_INITIALIZATION_LEVEL_SCENE)
    {
#ifdef GENERATOR_DSP
        GDREGISTER_CLASS(AudioStreamPlaybackFaust);
        GDREGISTER_CLASS(AudioStreamFaust);
#else
        GDREGISTER_CLASS(AudioEffectFaustInstance);
        GDREGISTER_CLASS(AudioEffectFaust);
#endif
    }

    if (level == godot::MODULE_INITIALIZATION_LEVEL_EDITOR)
    {
        // GDREGISTER_CLASS(ExtButton)
        // GDREGISTER_CLASS(RandomIntEditor);
        // GDREGISTER_CLASS(InspectorPlugin);
        // GDREGISTER_CLASS(Plugin);

        // EditorPlugins::add_by_type<Plugin>();
    }
}

void UninitializeFaust2GodotModule(ModuleInitializationLevel level)
{
    if (level != MODULE_INITIALIZATION_LEVEL_SCENE) return;
}

extern "C"
{
    //GDExtension entry point
    GDExtensionBool GDE_EXPORT faust2godot_init(GDExtensionInterfaceGetProcAddress p_get_proc_address,
        const GDExtensionClassLibraryPtr p_library,
        GDExtensionInitialization *r_initialization)
    {
    	GDExtensionBinding::InitObject init_obj(p_get_proc_address, p_library, r_initialization);

        init_obj.register_initializer(InitializeFaust2GodotModule);
        init_obj.register_terminator(UninitializeFaust2GodotModule);
       	init_obj.set_minimum_library_initialization_level(MODULE_INITIALIZATION_LEVEL_EDITOR);

        return init_obj.init();
    }
}
