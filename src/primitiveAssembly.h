#pragma once
#include <vector>
#include <glm/geometric.hpp>
#include "fragment.h"

enum class Primitive
{
    TRIANGLES,
};

std::vector<std::vector<Vertex>> primitiveAssembly(
    Primitive polygon,
    const std::vector<Vertex>& transformedVertices
) {
    std::vector<std::vector<Vertex>> assembledVertices;

    switch (polygon) {
        case Primitive::TRIANGLES: {
            assert(transformedVertices.size() % 3 == 0 && "El número de vértices debe ser un múltiplo de 3 para triángulos.");

            for (size_t i = 0; i < transformedVertices.size(); i += 3) {
                std::vector<Vertex> triangle = {
                    transformedVertices[i],
                    transformedVertices[i + 1],
                    transformedVertices[i + 2]
                };
                assembledVertices.push_back(triangle);
            }
            break;
        }
        default:
            std::cerr << "Error: No se reconoce el tipo primitivo." << std::endl;
            break;
    }

    return assembledVertices;
}