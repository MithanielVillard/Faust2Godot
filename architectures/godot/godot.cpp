#include <faust/dsp/dsp.h>
#include <faust/gui/UI.h>
#include <faust/gui/meta.h>

#ifdef _WIN32
#define LIB_EXPORT __declspec(dllexport)
#else
#define LIB_EXPORT
#endif

#ifdef POLY
#include <faust/dsp/poly-dsp.h>

std::list<GUI*> GUI::fGuiList;
ztimedmap  GUI::gTimedZoneMap;
#endif

#ifdef GLOBAL_EFFECT
#include <faust/dsp/dsp-combiner.h>
#include "effect.h"
#endif

<<includeIntrinsic>>

<<includeclass>>


extern "C"
{
#ifdef POLY
#   ifdef GLOBAL_EFFECT
LIB_EXPORT dsp* DspFactory() { return new dsp_sequencer(new mydsp_poly(new mydsp, NVOICES, true, true), new effect()); }
#   else
LIB_EXPORT dsp* DspFactory() { return new mydsp_poly(new mydsp, NVOICES, true, true); }
#   endif

LIB_EXPORT unsigned int GetNbrVoices() { return NVOICES; }

#else
LIB_EXPORT dsp* DspFactory() { return new mydsp; }
#endif

#if GENERATOR_DSP
LIB_EXPORT bool IsGenerator() { return true; }
#else
LIB_EXPORT bool IsGenerator() { return false; }
#endif

}