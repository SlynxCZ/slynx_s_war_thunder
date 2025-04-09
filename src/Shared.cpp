//
// Created by Michal Přikryl on 09.04.2025.
// Copyright (c) 2025 slynxcz. All rights reserved.
//
#include <windows.h>
#include "Shared.h"
#include "logger/Logger.h"
#include "launcher/LauncherManager.h"
#include "game/GameManager.h"
#include <string>

int main() {
    Logger::Info("Slynx's War Thunder Launcher, booting...");

    LauncherManager launcher;
    if (launcher.StartLauncher(20)) {
        Logger::Info("Boot finished! Starting game...");

        GameManager game;
        if (!game.StartGame()) {
            Logger::Error("Failed to launch the game.");
        }
    } else {
        Logger::Error("Launcher could not boot.");
    }

    system("pause");
    return 0;
}

std::string Shared::GetUserPath(const std::string &relativePath) {
    char* userProfile = std::getenv("USERPROFILE");
    if (!userProfile) return relativePath;

    return std::string(userProfile) + relativePath;
}

