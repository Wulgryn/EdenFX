#pragma once
// Hordozható konzol-exit jelkezelő.
// POSIX: SIGINT/SIGTERM/SIGHUP -> sigaction (signal.h)
// Windows: SetConsoleCtrlHandler (X gomb / logoff / shutdown / Ctrl+C/Ctrl+Break)
//          Ha nem kérsz WinAPI-t: #define CONSOLE_EXIT_NO_WINAPI (csak Ctrl-C)
// Használat:
//   console_exit::install();
//   while (!console_exit::exit_requested()) { /* munka */ }
//   auto r = console_exit::reason(); /* miért állt le? */

#include <stdint.h>

namespace console_exit {

enum class Reason : int {
    None=0, CtrlC=1, Terminate=2, Hangup=3,   // POSIX
    CtrlBreak=4, Close=5, Logoff=6, Shutdown=7, // Windows
    Unknown=99
};

inline const char* to_string(Reason r) noexcept {
    switch (r) {
        case Reason::None: return "None";
        case Reason::CtrlC: return "CtrlC";
        case Reason::Terminate: return "Terminate(SIGTERM)";
        case Reason::Hangup: return "Hangup(SIGHUP)";
        case Reason::CtrlBreak: return "CtrlBreak";
        case Reason::Close: return "Close(X)";
        case Reason::Logoff: return "Logoff";
        case Reason::Shutdown: return "Shutdown";
        default: return "Unknown";
    }
}

#if defined(_WIN32) && !defined(CONSOLE_EXIT_NO_WINAPI)

// ------------------- Windows (WinAPI: X + Ctrl-C) -------------------
#include <windows.h>
#include <atomic>

inline std::atomic<int>  g_reason{static_cast<int>(Reason::None)};
inline std::atomic<bool> g_flag{false};

inline void set_exit(Reason r) noexcept {
    g_reason.store(static_cast<int>(r), std::memory_order_relaxed);
    g_flag.store(true, std::memory_order_relaxed);
}
inline bool   exit_requested() noexcept { return g_flag.load(std::memory_order_relaxed); }
inline Reason reason() noexcept { return static_cast<Reason>(g_reason.load(std::memory_order_relaxed)); }
inline void   reset() noexcept { g_reason.store(static_cast<int>(Reason::None)); g_flag.store(false); }

static BOOL WINAPI ConsoleCtrlHandler(DWORD ctrl) {
    switch (ctrl) {
        case CTRL_C_EVENT:        set_exit(Reason::CtrlC);     return TRUE;
        case CTRL_BREAK_EVENT:    set_exit(Reason::CtrlBreak); return TRUE;
        case CTRL_CLOSE_EVENT:    set_exit(Reason::Close);     return TRUE; // ablak X
        case CTRL_LOGOFF_EVENT:   set_exit(Reason::Logoff);    return TRUE;
        case CTRL_SHUTDOWN_EVENT: set_exit(Reason::Shutdown);  return TRUE;
        default:                  set_exit(Reason::Unknown);   return TRUE;
    }
}

inline void install() {
    SetConsoleCtrlHandler(ConsoleCtrlHandler, TRUE); // X/CtrlC/CtrlBreak/Logoff/Shutdown
}

#elif defined(_WIN32)

// --------------- Windows (WinAPI nélkül – csak Ctrl-C) ---------------
#include <signal.h>

inline volatile sig_atomic_t g_flag  = 0;
inline volatile sig_atomic_t g_rsn   = static_cast<sig_atomic_t>(Reason::None);

inline void set_exit(Reason r) noexcept { g_rsn = static_cast<sig_atomic_t>(r); g_flag = 1; }
inline bool   exit_requested() noexcept { return g_flag != 0; }
inline Reason reason() noexcept { return static_cast<Reason>(g_rsn); }
inline void   reset() noexcept { g_flag = 0; g_rsn = static_cast<sig_atomic_t>(Reason::None); }

extern "C" void console_exit_crt_handler(int sig) {
    switch (sig) {
        case SIGINT:  set_exit(Reason::CtrlC);     break;
        case SIGTERM: set_exit(Reason::Terminate); break;
        default:      set_exit(Reason::Unknown);   break;
    }
}

// MS CRT: Ctrl+C/Break jelként megy (SIGINT/SIGBREAK), de X gomb NEM. :contentReference[oaicite:2]{index=2}
inline void install() {
    signal(SIGINT,  console_exit_crt_handler);
    signal(SIGTERM, console_exit_crt_handler);
}

#else

// --------------------------- POSIX (Linux/macOS) ----------------------
#include <signal.h>
#include <string.h>

inline volatile sig_atomic_t g_flag  = 0;
inline volatile sig_atomic_t g_rsn   = static_cast<sig_atomic_t>(Reason::None);

inline void set_exit(Reason r) noexcept { g_rsn = static_cast<sig_atomic_t>(r); g_flag = 1; }
inline bool   exit_requested() noexcept { return g_flag != 0; }
inline Reason reason() noexcept { return static_cast<Reason>(g_rsn); }
inline void   reset() noexcept { g_flag = 0; g_rsn = static_cast<sig_atomic_t>(Reason::None); }

// POSIX: javasolt a signal() helyett a sigaction() használata. :contentReference[oaicite:3]{index=3}
extern "C" void console_exit_posix_handler(int sig) {
    switch (sig) {
        case SIGINT:  set_exit(Reason::CtrlC);     break;
        case SIGTERM: set_exit(Reason::Terminate); break;
#ifdef SIGHUP
        case SIGHUP:  set_exit(Reason::Hangup);    break; // terminál bezárásakor tipikusan SIGHUP érkezik
#endif
        default:      set_exit(Reason::Unknown);   break;
    }
}

inline void install_one(int sig) {
    struct sigaction sa;
    memset(&sa, 0, sizeof(sa));
    sa.sa_handler = console_exit_posix_handler;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    sigaction(sig, &sa, nullptr);
}

inline void install() {
    install_one(SIGINT);
    install_one(SIGTERM);
#ifdef SIGHUP
    install_one(SIGHUP); // terminál bezárása -> SIGHUP (klasszikus „hangup”) :contentReference[oaicite:4]{index=4}
#endif
}

#endif // platformok

} // namespace console_exit
