//
// Created by Michal Přikryl on 09.04.2025.
// Copyright (c) 2025 slynxcz. All rights reserved.
//
#include "GameManager.h"
#include "../logger/Logger.h"
#include "../Shared.h"
#include <windows.h>
#include <string>

bool GameManager::StartGame() {
    Logger::Info("Launching War Thunder with overlay...");

    STARTUPINFO si = { sizeof(si) };
    PROCESS_INFORMATION pi;

    std::string path = Shared::GetUserPath("\\AppData\\Local\\WarThunder\\win64\\aces.exe");
    LPCSTR gamePath = path.c_str();

    if (!CreateProcessA(gamePath, nullptr, nullptr, nullptr, FALSE, 0, nullptr, nullptr, &si, &pi)) {
        Logger::Error("Failed to launch the game.");
        return false;
    }

    Logger::Info("Game process started successfully.");
    CloseHandle(pi.hProcess);
    CloseHandle(pi.hThread);
    return true;
}
