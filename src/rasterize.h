#pragma once
#include <vector>
#include "fragment.h"
#include "primitiveAssembly.h"
#include "triangle.h"
#include "color.h"

std::vector<Fragment> rasterize(
    Primitive primitive,
    const std::vector<std::vector<Vertex>> &assembledVertices,
    float SCREEN_WIDTH,
    float SCREEN_HEIGHT,
    glm::vec3 light,
    Color mainColor)
{
    std::vector<Fragment> fragments;

    switch (primitive)
    {
    case Primitive::TRIANGLES:
    {
        for (const std::vector<Vertex> &triangleVertices : assembledVertices)
        {
            assert(triangleVertices.size() == 3 && "Triangle vertices must contain exactly 3 vertices.");
            std::vector<Fragment> triangleFragments = triangle(
                triangleVertices[0],
                triangleVertices[1],
                triangleVertices[2],
                SCREEN_WIDTH,
                SCREEN_HEIGHT,
                light,
                mainColor);
            fragments.insert(fragments.end(), triangleFragments.begin(), triangleFragments.end());
        }
        break;
    }
    default:
        std::cerr << "Error: No se reconoce el tipo primitivo para rasterización." << std::endl;
        break;
    }

    return fragments;
}