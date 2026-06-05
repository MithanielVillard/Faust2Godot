#include <faust/dsp/dsp.h>
#include <faust/gui/UI.h>
#include <faust/gui/meta.h>

#ifdef _WIN32
#define LIB_EXPORT __declspec(dllexport)
#else
#define LIB_EXPORT
#endif

<<includeIntrinsic>>

<<includeclass>>


extern "C"
{
    LIB_EXPORT dsp* DspFactory() { return new mydsp; }
    LIB_EXPORT bool IsGenerator()
    {
#if GENERATOR_DSP
        return true;
#else
        return false;
#endif
    }
}
