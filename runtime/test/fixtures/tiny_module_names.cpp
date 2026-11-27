// Original synthetic guest. Name input is supplied independently of argv[0].
#include <windows.h>
#include <cstdio>
#include <cwchar>
#include <string>

extern "C" IMAGE_DOS_HEADER __ImageBase;

int wmain(int argc, wchar_t** argv) {
    if (argc != 2) return 20;
    BOOL substituted = FALSE;
    const UINT page = GetACP();
    const bool utf8 = page == CP_UTF8 || page == 54936;
    const DWORD flags = utf8 ? 0 : WC_NO_BEST_FIT_CHARS;
    const int size = WideCharToMultiByte(CP_ACP, flags, argv[1], -1, nullptr, 0,
                                         nullptr, utf8 ? nullptr : &substituted);
    if (!size) return 21;
    std::string name(static_cast<size_t>(size), '\0');
    if (!WideCharToMultiByte(CP_ACP, flags, argv[1], -1, name.data(), size,
                            nullptr, utf8 ? nullptr : &substituted)) return 22;
    bool exact = false, ascii_case = false;
    if (!substituted) {
        exact = GetModuleHandleA(name.c_str()) == reinterpret_cast<HMODULE>(&__ImageBase);
        std::wstring alias(argv[1]);
        for (wchar_t& c : alias) {
            if (c >= L'a' && c <= L'z') c = static_cast<wchar_t>(c - L'a' + L'A');
            else if (c >= L'A' && c <= L'Z') c = static_cast<wchar_t>(c - L'A' + L'a');
        }
        if (!WideCharToMultiByte(CP_ACP, flags, alias.c_str(), -1, name.data(), size,
                                nullptr, utf8 ? nullptr : &substituted)) return 24;
        ascii_case = GetModuleHandleA(name.c_str()) == reinterpret_cast<HMODULE>(&__ImageBase);
    }
    std::printf("NAME_PROBE entered=1 acp=%u representable=%u self_a=%u ascii_case_a=%u\n",
                page, !substituted ? 1u : 0u, exact ? 1u : 0u, ascii_case ? 1u : 0u);
    return substituted || (exact && ascii_case) ? 0 : 23;
}
