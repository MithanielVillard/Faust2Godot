#pragma once
#include "defines.h"

#include <cstddef>
#include <filesystem>
#include <optional>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <iostream>

#ifdef OS_WIN
#include <windows.h>
using LibHandle = HINSTANCE;
#endif

#if defined (OS_MAC) || defined (OS_LINUX)
#include <dlfcn.h>
using LibHandle = void*;
#endif

class DynLibrary
{
public:
     template <OS _Os> struct Entry;
     template <typename... Entries> struct Decorator;
     struct EntryTag {};
     struct Decoration
     {
         std::string_view prefix;
         std::string_view extension;
     };

     using DefaultDeco_t = DynLibrary::Decorator<
        DynLibrary::Entry<OS::WIN>,
        DynLibrary::Entry<OS::MAC>,
        DynLibrary::Entry<OS::LINUX>
    >;

     DynLibrary() = default;
     DynLibrary(std::filesystem::path const& libPath);
     DynLibrary(std::filesystem::path const& libPath, auto const& decorator);
     ~DynLibrary();

     std::optional<std::string> Open(std::filesystem::path const& libPath); //Return error string if an error occured
     std::optional<std::string> Open(std::filesystem::path const& libPath, auto const& decorator); //Return error string if an error occured

     template <typename Func> requires std::is_function_v<Func>
     Func* GetFunction(std::string_view name);

     template <typename V> requires (!std::is_function_v<V>)
     V& GetVariable(std::string_view name);

     static const DefaultDeco_t DefaultDecorator;
private:
    LibHandle m_libHandle;
};

//==== Dynlibrary common function implementation ======

inline DynLibrary::DynLibrary(std::filesystem::path const& libPath)
{
    if (auto result = Open(libPath, DefaultDecorator))
        std::cerr << "Error while loading dynamic library : " << result.value() << std::endl;
}

inline DynLibrary::DynLibrary(std::filesystem::path const& libPath, auto const& decorator)
{
    if (auto result = Open(libPath, decorator))
        std::cerr << "Error while loading dynamic library : " << result.value() << std::endl;
}

inline std::optional<std::string> DynLibrary::Open(std::filesystem::path const& libPath)
{
    return Open(libPath, DefaultDecorator);
}


//==== OS Decorator Implementation ======

template <OS _Os>
struct DynLibrary::Entry : public EntryTag
{
   constexpr Entry(std::string_view prefix, std::string_view extension) :
   decoration {prefix, extension} {}

   static constexpr OS Os { _Os };
   Decoration decoration;
};

template <typename... Entries>
struct DynLibrary::Decorator
{
    static_assert((std::is_base_of_v<EntryTag, Entries> && ...), "Parameters should be of type Entry");

public:
    constexpr Decorator(Entries... params) : decorations(std::make_tuple(params...)) {}

    template <OS _Os>
    constexpr Decoration Get() const { return GetImpl<_Os, 0>(); }

private:
    template <OS _Os, std::size_t I>
    constexpr Decoration GetImpl() const
    {
        static_assert(I < sizeof...(Entries) && I >= 0, "No entries found for this Os");

        if constexpr (std::tuple_element_t<I, decltype(decorations)>::Os == _Os)
            return std::get<I>(decorations).decoration;
        else
            return GetImpl<_Os, I+1>();
    }

private:
    std::tuple<Entries...> decorations;
};

constexpr DynLibrary::DefaultDeco_t DynLibrary::DefaultDecorator {
  Entry<OS::WIN>   {"", ".dll"},
  Entry<OS::MAC>   {"lib", ".dylib"},
  Entry<OS::LINUX> {"lib", ".so"}
};

#ifdef OS_WIN
#include "windows/DynLibrary.inl"
#endif

#if defined (OS_MAC) || defined (OS_LINUX)
#include "unix/DynLibrary.inl"
#endif
