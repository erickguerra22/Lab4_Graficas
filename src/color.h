#pragma once
#include <iostream>
#include <cstdint>

class Color
{
private:
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

public:
    Color();
    Color(int red, int green, int blue, int alpha = 255);
    Color(float red, float green, float blue, float alpha = 1.0f);
    uint8_t getBlue();
    uint8_t getRed();
    uint8_t getGreen();
    uint8_t getAlpha();

    friend std::ostream &operator<<(std::ostream &os, const Color &color);
    Color operator+(const Color &color);
    Color operator*(float factor) const;
    bool operator==(const Color &color);
    void print();
};
