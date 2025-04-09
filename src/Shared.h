//
// Created by Michal Přikryl on 09.04.2025.
// Copyright (c) 2025 slynxcz. All rights reserved.
//
#pragma once

#ifndef SHARED_H
#define SHARED_H

#include <string>

class Shared {
public:
    static std::string GetUserPath(const std::string& relativePath);
};

#endif //SHARED_H
