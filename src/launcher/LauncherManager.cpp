//
// Created by Michal Přikryl on 09.04.2025.
// Copyright (c) 2025 slynxcz. All rights reserved.
//
#include "LauncherManager.h"
#include "../logger/Logger.h"
#include "../Shared.h"
#include <windows.h>
#include <thread>
#include <chrono>

bool LauncherManager::StartLauncher(int waitSeconds) {
    Logger::Info("Booting Gaijin Launcher...");

    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    std::string path = Shared::GetUserPath("\\AppData\\Local\\WarThunder\\launcher.exe");
    LPCSTR launcherPath = path.c_str();

    if (!CreateProcessA(launcherPath, nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
        Logger::Error("Failed to boot Gaijin launcher.");
        return false;
    }

    Logger::Info("Waiting " + std::to_string(waitSeconds) + " seconds for updates...");
    std::this_thread::sleep_for(std::chrono::seconds(waitSeconds));

    Logger::Info("Terminating launcher (if still running)...");
    TerminateLauncher();

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);

    return true;
}

void LauncherManager::TerminateLauncher() {
    HWND hwnd = FindWindowA(nullptr, "Gaijin Smart Launcher");
    if (hwnd != nullptr) {
        PostMessage(hwnd, WM_CLOSE, 0, 0);
        Logger::Info("Sent WM_CLOSE to War Thunder Launcher.");
    } else {
        Logger::Warning("Launcher window not found, could not close it via WM_CLOSE.");
    }
}
