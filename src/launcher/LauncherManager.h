//
// Created by Michal Přikryl on 09.04.2025.
// Copyright (c) 2025 slynxcz. All rights reserved.
//
#pragma once
#ifndef LAUNCHERMANAGER_H
#define LAUNCHERMANAGER_H

class LauncherManager {
public:
    static bool StartLauncher(int waitSeconds);

private:
    static void TerminateLauncher();
};

#endif //LAUNCHERMANAGER_H
