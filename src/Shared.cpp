//
// Created by Michal Přikryl on 09.04.2025.
// Copyright (c) 2025 slynxcz. All rights reserved.
//
#include <windows.h>
#include <string>
#include <sstream>
#include "Shared.h"
#include "logger/Logger.h"
#include "launcher/LauncherManager.h"
#include "game/GameManager.h"
#include <shlobj.h>

int main() {
    if (!Shared::IsRunningAsAdmin()) {
        Logger::Info("Requesting administrator privileges...");

        if (!Shared::RequestAdminPrivileges()) {
            Logger::Error("Failed to acquire admin privileges.");
            return -1;
        }

        Logger::Info("Exiting original process...");
        ExitProcess(0);
    }

    Logger::Info("Slynx's War Thunder Launcher, booting...");

    std::string expectedPath = "C:\\Slynx-War-Thunder-Launcher";
    std::string targetPath = "C:/Slynx-War-Thunder-Launcher/Slynx's War Thunder Launcher.exe";
    std::string path = Shared::GetUserPath("\\Desktop\\Slynx's War Thunder Launcher.lnk");
    LPCSTR shortcutPath = path.c_str();

    Logger::Warning(Shared::GetExecutablePath());

    if (!Shared::CheckAppDirectory(expectedPath)) {
        Logger::Error("Incorrect directory. Regenerating files and creating shortcut...");

        Shared::RegenerateFilesAndCreateShortcut(targetPath, shortcutPath);

        system("pause");
        return -1;
    }

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

std::string Shared::GetExecutablePath() {
    char buffer[MAX_PATH];
    GetModuleFileNameA(nullptr, buffer, MAX_PATH);
    return std::string(buffer);
}

bool Shared::CheckAppDirectory(const std::string &expectedPath) {
    std::string currentPath = GetExecutablePath();
    size_t pos = currentPath.find_last_of("\\\\");
    std::string currentDir = currentPath.substr(0, pos);

    if (currentDir == expectedPath) {
        Logger::Info("Correct directory: " + currentDir);
        return true;
    }
    Logger::Error("Incorrect directory. Expected: " + expectedPath + " but got: " + currentDir);
    return false;
}

bool Shared::CopyAndRenameExecutable(const std::string &sourcePath, const std::string &targetPath) {
    std::string targetDir = targetPath.substr(0, targetPath.find_last_of("\\/"));
    if (CreateDirectoryA(targetDir.c_str(), nullptr) || GetLastError() == ERROR_ALREADY_EXISTS) {
    } else {
        std::stringstream message;
        message << "Failed to create directory: " << targetDir;
        Logger::Error(message.str());
        return false;
    }

    if (GetFileAttributesA(sourcePath.c_str()) == INVALID_FILE_ATTRIBUTES) {
        std::stringstream message;
        message << "Source file does not exist: " << sourcePath;
        Logger::Error(message.str());
        return false;
    }

    if (DeleteFileIfExists(targetPath)) {
        if (CopyFileA(sourcePath.c_str(), targetPath.c_str(), FALSE)) {
            std::stringstream message;
            message << "File copied and renamed successfully, root path of new launcher: " << targetPath;
            Logger::Info(message.str());
            return true;
        } else {
            std::stringstream message;
            message << "Error copying file: " << GetLastError();
            Logger::Error(message.str());
            return false;
        }
    }

    return false;
}

std::string Shared::GetUserPath(const std::string &relativePath) {
    char *userProfile = std::getenv("USERPROFILE");
    if (!userProfile) return relativePath;

    return std::string(userProfile) + relativePath;
}

bool Shared::CreateShortcut(const std::string &targetPath, const std::string &shortcutPath) {
    IShellLinkW *pShellLink = nullptr;
    IPersistFile *pPersistFile = nullptr;

    std::wstring targetPathW(targetPath.begin(), targetPath.end());
    std::wstring shortcutPathW(shortcutPath.begin(), shortcutPath.end());

    std::string dirPath = shortcutPath.substr(0, shortcutPath.find_last_of("\\/"));
    if (CreateDirectoryA(dirPath.c_str(), nullptr) || GetLastError() == ERROR_ALREADY_EXISTS) {
    } else {
        std::stringstream message;
        message << "Failed to create directory for shortcut: " << dirPath;
        Logger::Error(message.str());
        return false;
    }

    HRESULT hResult = CoInitialize(nullptr);
    if (FAILED(hResult)) {
        Logger::Error("CoInitialize failed");
        return false;
    }

    hResult = CoCreateInstance(CLSID_ShellLink, nullptr, CLSCTX_INPROC_SERVER, IID_IShellLinkW,
                               reinterpret_cast<void **>(&pShellLink));
    if (FAILED(hResult)) {
        Logger::Error("CoCreateInstance failed for ShellLink");
        CoUninitialize();
        return false;
    }

    pShellLink->SetPath(targetPathW.c_str());

    hResult = pShellLink->QueryInterface(IID_IPersistFile, reinterpret_cast<void **>(&pPersistFile));
    if (FAILED(hResult)) {
        Logger::Error("QueryInterface failed for PersistFile");
        pShellLink->Release();
        CoUninitialize();
        return false;
    }

    hResult = pPersistFile->Save(shortcutPathW.c_str(), TRUE);
    if (FAILED(hResult)) {
        Logger::Error("Failed to save shortcut. HRESULT: " + std::to_string(hResult));
        pPersistFile->Release();
        pShellLink->Release();
        CoUninitialize();
        return false;
    }

    pPersistFile->Release();
    pShellLink->Release();
    CoUninitialize();

    std::stringstream message;
    message << "Shortcut created successfully at " << shortcutPath;
    Logger::Info(message.str());
    return true;
}

bool Shared::IsRunningAsAdmin() {
    BOOL isAdmin = FALSE;
    PSID adminGroup = nullptr;
    SID_IDENTIFIER_AUTHORITY NtAuthority = SECURITY_NT_AUTHORITY;

    if (AllocateAndInitializeSid(&NtAuthority, 2, SECURITY_BUILTIN_DOMAIN_RID, DOMAIN_ALIAS_RID_ADMINS, 0, 0, 0, 0, 0,
                                 0, &adminGroup)) {
        CheckTokenMembership(nullptr, adminGroup, &isAdmin);
        FreeSid(adminGroup);
    }

    return isAdmin == TRUE;
}

bool Shared::RequestAdminPrivileges() {
    std::string executablePath = Shared::GetExecutablePath();

    SHELLEXECUTEINFO sei = {sizeof(SHELLEXECUTEINFO)};
    sei.fMask = SEE_MASK_NOCLOSEPROCESS;
    sei.hwnd = nullptr;
    sei.lpVerb = "runas";
    sei.lpFile = executablePath.c_str();
    sei.nShow = SW_SHOWNORMAL;

    if (!ShellExecuteEx(&sei)) {
        Logger::Error("Failed to request admin privileges.");
        return false;
    }

    WaitForSingleObject(sei.hProcess, INFINITE);
    CloseHandle(sei.hProcess);

    return true;
}

bool Shared::DeleteFileIfExists(const std::string& filePath) {
    if (GetFileAttributesA(filePath.c_str()) != INVALID_FILE_ATTRIBUTES) {
        if (!DeleteFileA(filePath.c_str())) {
            std::stringstream message;
            message << "Failed to delete existing file: " << filePath;
            Logger::Error(message.str());
            return false;
        }
    }
    return true;
}

void Shared::RegenerateFilesAndCreateShortcut(const std::string &targetPath, const std::string &shortcutPath) {
    Logger::Info("Regenerating necessary files...");

    std::string sourcePath = GetExecutablePath();
    if (!CopyAndRenameExecutable(sourcePath, targetPath)) {
        Logger::Error("Failed to regenerate files.");
        return;
    }

    Logger::Info("Files regenerated successfully.");

    if (!CreateShortcut(targetPath, shortcutPath)) {
        Logger::Error("Failed to create shortcut.");
    }
}
