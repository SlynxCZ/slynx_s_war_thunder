﻿//
// Created by Michal Přikryl on 09.04.2025.
// Copyright (c) 2025 slynxcz. All rights reserved.
//
#include "LauncherManager.h"
#include "../logger/Logger.h"
#include "../Shared.h"
#include <windows.h>
#include <thread>
#include <chrono>
#include <conio.h>

bool LauncherManager::StartLauncher(int waitSeconds) {
    Logger::Info("Booting Gaijin Launcher...");

    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    std::string path = Shared::GetUserPath(R"(\AppData\Local\WarThunder\launcher.exe)");
    LPCSTR launcherPath = path.c_str();

    if (!CreateProcessA(launcherPath, nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
        Logger::Error("Failed to boot Gaijin launcher");
        return false;
    }

    Logger::Info("Waiting " + std::to_string(waitSeconds) + " seconds for updates...");
    Logger::Warning("Press [Enter] to cancel and keep launcher open");

    bool cancel = false;
    for (int i = 0; i < waitSeconds * 10; ++i) {
        if (_kbhit()) {
            int key = _getch();
            if (key == 13) {
                cancel = true;
                Logger::Warning("Update interrupt detected. Launcher will remain open");
                break;
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }

    if (!cancel) {
        Logger::Info("Terminating launcher (if still running)...");
        TerminateLauncher();

        CloseHandle(pi.hProcess);
        CloseHandle(pi.hThread);

        Logger::Info("Boot finished! Starting game...");
        return true;
    }

    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return false;
}

void LauncherManager::TerminateLauncher() {
    try {
        system("taskkill /f /im launcher.exe");
        Logger::Info("Sent taskkill to War Thunder Launcher.");
    } catch (...) {
        Logger::Warning("Launcher window not found, could not close it via taskkill or process termination.");
    }
}
