#include "Keyboard.h"

bool isKeyPressed(int virtualKeyCode)
{
    return GetAsyncKeyState(virtualKeyCode) & 0x8000;
}

Keyboard pressed()
{
    if (isKeyPressed('W'))
        return Keyboard::W;
    else if (isKeyPressed('A'))
        return Keyboard::A;
    else if (isKeyPressed('S'))
        return Keyboard::S;
    else if (isKeyPressed('D'))
        return Keyboard::D;
    else if (isKeyPressed('M'))
        return Keyboard::M;
    else if (isKeyPressed('E'))
        return Keyboard::E;
    else if (isKeyPressed(VK_UP))
        return Keyboard::ARROW_UP;
    else if (isKeyPressed(VK_DOWN))
        return Keyboard::ARROW_DOWN;
    else if (isKeyPressed(VK_LEFT))
        return Keyboard::ARROW_LEFT;
    else if (isKeyPressed(VK_RIGHT))
        return Keyboard::ARROW_RIGHT;

    return Keyboard::NOTHING;
}