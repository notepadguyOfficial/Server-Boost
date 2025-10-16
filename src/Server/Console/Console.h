#pragma once

class Console
{
    bool debug;

public:
    explicit Console(bool debug) : debug(debug) {}
    void Show();
};
