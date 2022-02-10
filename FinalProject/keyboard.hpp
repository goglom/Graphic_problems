#pragma once
#include <unordered_map>
#include <QKeyEvent>

class Keyboard
{   
public:
    Keyboard(std::initializer_list<int> trackedKeys);
    bool getKeyState(int key);
    bool operator[](int key);
    void setKeyState(int key, bool state);

private:
    std::unordered_map<int, bool> keysState;
};

