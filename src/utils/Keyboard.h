#pragma once

#include <windows.h>

enum class Keyboard
{
    W,
    A,
    S,
    D,
    M, // middle
    E, // extra information

    ARROW_UP,
    ARROW_DOWN,
    ARROW_LEFT,
    ARROW_RIGHT,

    NOTHING,
};

bool isKeyPressed(int virtualKeyCode);
Keyboard pressed();