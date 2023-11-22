#include "color.h"
#include <SDL.h>

Color::Color() : r(0), g(0), b(0), a(0)
{
}

Color::Color(int red, int green, int blue, int alpha)
{
    r = static_cast<uint8_t>(std::min(std::max(red, 0), 255));
    g = static_cast<uint8_t>(std::min(std::max(green, 0), 255));
    b = static_cast<uint8_t>(std::min(std::max(blue, 0), 255));
    a = static_cast<uint8_t>(std::min(std::max(alpha, 0), 255));
}
Color::Color(float red, float green, float blue, float alpha)
{
    r = static_cast<uint8_t>(std::min(std::max(red * 255, 0.0f), 255.0f));
        g = static_cast<uint8_t>(std::min(std::max(green * 255, 0.0f), 255.0f));
        b = static_cast<uint8_t>(std::min(std::max(blue * 255, 0.0f), 255.0f));
        a = static_cast<uint8_t>(std::min(std::max(alpha * 255, 0.0f), 255.0f));
}

uint8_t Color::getBlue()
{
    return b;
}

uint8_t Color::getRed()
{
    return r;
}

uint8_t Color::getGreen()
{
    return g;
}

uint8_t Color::getAlpha()
{
    return a;
}

std::ostream &operator<<(std::ostream &os, const Color &color)
{
    os << "\033[1;31mR:" << static_cast<int>(color.r) << "\033[0m, ";
    os << "\033[1;32mG:" << static_cast<int>(color.g) << "\033[0m, ";
    os << "\033[1;34mB:" << static_cast<int>(color.b) << "\033[0m";
    os << "A:" << static_cast<int>(color.a);
    return os;
}

Color Color::operator+(const Color &color)
{
    uint8_t red = r + color.r;
    uint8_t green = g + color.g;
    uint8_t blue = b + color.b;
    uint8_t alpha = a + color.a;
    return Color(red, green, blue, alpha);
}

Color Color::operator*(float factor) const
{
    uint8_t red = static_cast<uint8_t>(r * factor);
    uint8_t green = static_cast<uint8_t>(g * factor);
    uint8_t blue = static_cast<uint8_t>(b * factor);
    uint8_t alpha = static_cast<uint8_t>(a * factor);
    return Color(red, green, blue, alpha);
}

bool Color::operator==(const Color &color)
{
    return r == color.r && b == color.b && g == color.g && a == color.a;
}

void Color::print()
{
    SDL_Log("R: %u, G: %u, B: %u, A: %u", r, g, b, a);
}