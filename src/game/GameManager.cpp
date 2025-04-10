//
// Created by Michal Přikryl on 09.04.2025.
// Copyright (c) 2025 slynxcz. All rights reserved.
//
#include "GameManager.h"
#include "../logger/Logger.h"
#include "../Shared.h"
#include <windows.h>
#include <string>
#include <fstream>
#include <cstdint>
#include <iostream>

bool GameManager::StartGame() {
    Logger::Info("Launching War Thunder with overlay...");

    std::string steamURL = Shared::GetSteamURL();

    if (steamURL.empty()) {
        Logger::Error("No Steam game URL found. Please provide a valid Steam URL.");

        Logger::Info("Please enter your Steam game URL (e.g., steam://rungameid/15553089753044746240): ");
        std::getline(std::cin, steamURL);

        Shared::SaveSteamURL(steamURL);

        Logger::Info("Steam URL saved.");
    }

    Logger::Info("Launching game with Steam URL: " + steamURL);

    HINSTANCE result = ShellExecuteA(nullptr, "open", steamURL.c_str(), nullptr, nullptr, SW_SHOWNORMAL);

    if (reinterpret_cast<std::uintptr_t>(result) <= 32) {
        Logger::Error("Failed to launch game through Steam.");
        return false;
    }

    Logger::Info("Game process started successfully.");
    return true;
}
