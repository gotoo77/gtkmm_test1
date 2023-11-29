//
// Created by gotoo77 on 05/08/2023.
//

#ifndef DIRPATH_HPP
#define DIRPATH_HPP

#include <string>

#ifdef _WIN32 // Windows

#include <windows.h>

#ifdef _WIN32 // Windows
const char dir_sep = '\\';
#else // Linux, Unix, macOs etc...
const char dir_sep = '/';
#endif

static std::string get_exe_path() {
    char buffer[MAX_PATH];
    GetModuleFileName(nullptr, buffer, MAX_PATH);
    return buffer;
}

static std::string get_exe_dirpath() {
    auto path = get_exe_path();
    // remove exe
    size_t pos = path.find_last_of("\\/");
    if (pos != std::string::npos) {
        return path.substr(0, pos + 1);
    }

    return {};
}

#elif defined(__linux__) // Linux

#include <string>
#include <unistd.h>

static std::string get_exe_path() {
    char buffer[1024];
    ssize_t len = readlink("/proc/self/exe", buffer, sizeof(buffer) - 1);
    if (len != -1) {
        buffer[len] = '\0';
        return buffer;
    }
    return {};
}

static std::string get_exe_dirpath() {
    std::string exePath = get_exe_path();
    if (!exePath.empty()) {
        size_t pos = exePath.find_last_of('/');
        if (pos != std::string::npos) {
            return exePath.substr(0, pos + 1);
        }
    }
    return {};
}

#else // Unsupported platform

static std::string get_exe_path() {
    return {};
}

static std::string get_exe_dirpath() {
    return {};
}

#endif

#endif // DIRPATH_HPP
