#include <filesystem>
#include <sstream>
#include <stdexcept>
#include <type_traits>


inline std::optional<std::string> DynLibrary::Open(std::filesystem::path const& libPath, auto const& decorator)
{
#if defined OS_LINUX
    auto d = decorator.template Get<OS::LINUX>();
 #elif defined OS_MAC
    auto d = decorator.template Get<OS::MAC>();
 #endif

    std::filesystem::path p = libPath;
    p.remove_filename();

    //add decorator to the path
                                        //i.e /document/cpp/test.so
    std::stringstream ss;
    ss << d.prefix;                     //    /document/cpp/lib
    ss << libPath.stem().string();      //    /document/cpp/libmylib
    ss << d.extension;                  //    /document/cpp/libmylib.so

    p.replace_filename(ss.str());

    m_libHandle = dlopen(p.string().c_str() , RTLD_LAZY);
    if (!m_libHandle) return { dlerror() };

    dlerror();
    return {};
}

template <typename Func> requires std::is_function_v<Func>
inline Func* DynLibrary::GetFunction(std::string_view name)
{
    auto* object = static_cast<long*>(dlsym(m_libHandle, name.data()));

    char* error = dlerror();
    if (error != nullptr && !object) {
      std::cerr << error << std::endl;
      return nullptr;
    }

    return reinterpret_cast<Func*>(object);
}

template <typename V> requires (!std::is_function_v<V>)
inline V& DynLibrary::GetVariable(std::string_view name)
{
    auto* object = static_cast<long*>(dlsym(m_libHandle, name.data()));

    char* error = dlerror();
    if (error != nullptr && !object) {
      std::cerr << error << std::endl;
      throw std::invalid_argument("Symbol name not found");
    }

    return *reinterpret_cast<V*>(object);
}

inline DynLibrary::~DynLibrary()
{
    if (m_libHandle) dlclose(m_libHandle);
}
