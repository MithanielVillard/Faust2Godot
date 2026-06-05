#pragma once
#include "DynLibrary.h"

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>

DynLibrary& GetFaustDSP();
void InitializFaust2GodotModule(godot::ModuleInitializationLevel level);
void UninitializeFaust2GodotModule(godot::ModuleInitializationLevel level);
