#include <filesystem>
#include <sstream>
#include <type_traits>

inline std::string GetErrorMessage(DWORD dw)
{
    LPVOID lpMsgBuf;

    FormatMessage(
        FORMAT_MESSAGE_ALLOCATE_BUFFER |
        FORMAT_MESSAGE_FROM_SYSTEM |
        FORMAT_MESSAGE_IGNORE_INSERTS,
        NULL,
        dw,
        MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
        (LPTSTR) &lpMsgBuf,
        0, NULL);

    return { static_cast<char const*>(lpMsgBuf)};
}

inline std::optional<std::string> DynLibrary::Open(std::filesystem::path const& libPath, auto const& decorator)
{
    std::filesystem::path p = libPath;
    p.remove_filename();

    auto d = decorator.template Get<OS::WIN>();

    //add decorator to the path
                                        //i.e /document/cpp/test.so
    std::stringstream ss;
    ss << d.prefix;                     //    /document/cpp/lib
    ss << libPath.stem().string();      //    /document/cpp/libmylib
    ss << d.extension;                  //    /document/cpp/libmylib.so

    p.replace_filename(ss.str());

    m_libHandle = LoadLibrary(p.string().c_str());
    if (!m_libHandle) return { GetErrorMessage(GetLastError()) };

    return {};
}

template <typename Func> requires std::is_function_v<Func>
inline Func* DynLibrary::GetFunction(std::string_view name)
{
    Func* object = reinterpret_cast<Func*>(GetProcAddress(m_libHandle, name.data()));

    if (!object)
    {
        std::cerr << "Function name not found" << std::endl;
        return nullptr;
    }

    return object;
}

template <typename V> requires (!std::is_function_v<V>)
inline V& DynLibrary::GetVariable(std::string_view name)
{
    V* object = reinterpret_cast<V*>(GetProcAddress(m_libHandle, name.data()));

    if (!object)
    {
        std::cerr << "Symbol name not located" << std::endl;
        return nullptr;
    }

    return *object;
}

inline DynLibrary::~DynLibrary()
{
    if (m_libHandle) FreeLibrary(m_libHandle);
}
