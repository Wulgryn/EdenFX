#ifndef WHATOS_HPP
#define WHATOS_HPP

#if defined(_WIN32) || defined(_WIN64)
    #define OS_WINDOWS
#elif defined(__linux__) || defined(__linux) || defined(linux) || defined(__gnu_linux__)
    #define OS_LINUX
    #define OS_UNIX
#elif defined(__APPLE__) || defined(__MACH__) || defined(__APPLE_CC__)
    #define OS_MAC
    #define OS_UNIX
#elif defined(__unix__) || defined(__unix) || defined(unix)
    #define OS_UNIX
#else
    #define OS_UNKNOWN
    #error "Unknown OS"
#endif

#endif // WHATOS_HPP