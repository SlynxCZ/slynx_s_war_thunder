//
// Created by Michal Přikryl on 09.04.2025.
// Copyright (c) 2025 slynxcz. All rights reserved.
//
#include "GameManager.h"
#include "../logger/Logger.h"
#include <windows.h>
#include <string>
#include <cstdint>

bool GameManager::StartGame() {
    Logger::Info("Launching War Thunder via Steam with overlay...");

    const char* steamUri = "steam://rungameid/15553089753044746240";

    HINSTANCE result = ShellExecuteA(
        nullptr,
        "open",
        steamUri,
        nullptr,
        nullptr,
        SW_SHOWDEFAULT
    );

    if (reinterpret_cast<std::uintptr_t>(result) <= 32) {
        Logger::Error("Failed to launch the game via Steam");
        return false;
    }

    Logger::Info("Steam launch initiated successfully");
    return true;
}