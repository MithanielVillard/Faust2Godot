#ifdef _WIN32
#define LIB_EXPORT __declspec(dllexport)
#else
#define LIB_EXPORT
#endif

extern "C"
{
    LIB_EXPORT int Addition(int a, int b) { return a + b; }
    LIB_EXPORT double pi_value = 3.14159;
}
