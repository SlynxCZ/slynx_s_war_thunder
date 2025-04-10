#include "GameManager.h"
#include "../logger/Logger.h"
#include <windows.h>
#include <string>
#include <cstdint>
#include <fstream>
#include <filesystem>
#include <iostream>

bool GameManager::StartGame() {
    Logger::Info("Launching War Thunder via Steam with overlay...");

    const std::string configPath = "C:/Slynx-War-Thunder-Launcher/config.ini";
    std::string steamUri;

    if (!std::filesystem::exists(configPath)) {
        Logger::Warning("No config.ini found at C:/Slynx-War-Thunder-Launcher/. Prompting for Steam URI...");

        std::cout << "[INPUT REQUIRED] Please enter your Steam URI (e.g., steam://rungameid/...): ";
        std::getline(std::cin, steamUri);

        if (steamUri.empty()) {
            Logger::Error("No Steam URI entered. Aborting.");
            return false;
        }

        create_directories(std::filesystem::path(configPath).parent_path());

        std::ofstream out(configPath);
        if (out.is_open()) {
            out << steamUri;
            out.close();
            Logger::Info("Steam URI saved to config.ini");
        } else {
            Logger::Error("Failed to write Steam URI to config.ini");
            return false;
        }
    } else {
        // Read from config.ini
        std::ifstream in(configPath);
        if (in.is_open()) {
            std::getline(in, steamUri);
            in.close();

            if (steamUri.empty()) {
                Logger::Error("Steam URI in config.ini is empty.");
                return false;
            }

            Logger::Info("Steam URI loaded from config.ini");
        } else {
            Logger::Error("Failed to read from config.ini");
            return false;
        }
    }

    HINSTANCE result = ShellExecuteA(
        nullptr,
        "open",
        steamUri.c_str(),
        nullptr,
        nullptr,
        SW_SHOWDEFAULT
    );

    if (reinterpret_cast<std::uintptr_t>(result) <= 32) {
        Logger::Error("Failed to launch the game via Steam.");
        return false;
    }

    Logger::Info("Steam launch initiated successfully.");
    return true;
}
