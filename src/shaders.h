#pragma once
#include <glm/glm.hpp>
#include "uniforms.h"
#include "fragment.h"
#include "FastNoiseLite.h"
#include <SDL.h>
#include <vector>

Vertex vertexShader(const Vertex &vertex, const Uniforms &uniforms)
{
    // Aplicar las transformaciones al vértice utilizando las matrices de uniforms
    glm::vec4 clipSpaceVertex = uniforms.projection * uniforms.view * uniforms.model * glm::vec4(vertex.position, 1.0f);

    // Perspectiva
    glm::vec3 ndcVertex = glm::vec3(clipSpaceVertex) / clipSpaceVertex.w;

    // Aplicar transformación del viewport
    glm::vec4 screenVertex = uniforms.viewport * glm::vec4(ndcVertex, 1.0f);

    // Transformar la normal
    glm::vec3 transformedNormal = glm::mat3(uniforms.model) * vertex.normal;
    transformedNormal = glm::normalize(transformedNormal);

    return Vertex{
        glm::vec3(screenVertex),
        transformedNormal,
        vertex.position};
}

Fragment whiteDwarfShader(Fragment &fragment, int frame)
{
    FastNoiseLite whiteDwarfNoise;
    float x = fragment.original.x;
    float y = fragment.original.y;
    float z = fragment.original.z;

    glm::vec3 darkColor = glm::vec3(49.0f / 255.0f, 151.0f / 255.0f, 213.0f / 255.0f);
    glm::vec3 brightColor = glm::vec3(168.0f / 254.0f, 248.0f / 255.0f, 255.0f / 255.0f);

    float noise;

    whiteDwarfNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    float animationSpeed = 0.02f;
    float time = frame * animationSpeed;
    whiteDwarfNoise.SetFrequency(0.04 + (sin(3 * time) + 1) * 0.01);
    int zoom = 600;
    noise = (1 + whiteDwarfNoise.GetNoise(x * zoom, y * zoom, z * zoom)) / 2.3f;

    glm::vec3 whiteDwarfNoiseColor = mix(brightColor, darkColor, noise * 2.0f);

    fragment.color = Color(whiteDwarfNoiseColor.x, whiteDwarfNoiseColor.y, whiteDwarfNoiseColor.z);

    return fragment;
}

Fragment redGiantShader(Fragment &fragment, int frame)
{
    FastNoiseLite redGiantNoise;
    float x = fragment.original.x;
    float y = fragment.original.y;
    float z = fragment.original.z;

    glm::vec3 darkColor = glm::vec3(44.0f / 255.0f, 1.0f / 255.0f, 1.0f / 255.0f);
    glm::vec3 brightColor = glm::vec3(199.0f / 254.0f, 81.0f / 255.0f, 21.0f / 255.0f);

    float noise;

    redGiantNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    float animationSpeed = 0.02f;
    float time = frame * animationSpeed;
    redGiantNoise.SetFrequency(0.04 + (sin(3 * time) + 1) * 0.01);
    int zoom = 200;
    noise = (1 + redGiantNoise.GetNoise(x * zoom, y * zoom, z * zoom)) / 2.3f;

    glm::vec3 whiteDwarfNoiseColor = mix(brightColor, darkColor, noise * 2.0f);

    fragment.color = Color(whiteDwarfNoiseColor.x, whiteDwarfNoiseColor.y, whiteDwarfNoiseColor.z) * fragment.intensity;

    return fragment;
}

Fragment nabooShader(Fragment &fragment, int frame)
{
    Color fragmentColor;

    float x = fragment.original.x;
    float y = fragment.original.y;
    float z = fragment.original.z;

    glm::vec3 groundColor = {74.0f / 255, 140.0f / 255, 104.0f / 255};
    glm::vec3 dirtColor = {162.0f / 255, 180.0f / 255, 145.0f / 255};
    glm::vec3 plantColor = {70.0f / 255, 121.0f / 255, 89.0f / 255};
    glm::vec3 oceanColor = {90.0f / 255, 170.0f / 255, 226.0f / 255};
    glm::vec3 seaColor = {27.0f / 255, 109.0f / 255, 198.0f / 255};
    glm::vec3 cloudColor = {206.0f / 255, 216.0f / 255, 210.0f / 255};

    FastNoiseLite seaNoise;
    seaNoise.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);
    seaNoise.SetFrequency(0.04);
    seaNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
    seaNoise.SetFractalOctaves(5);

    FastNoiseLite groundNoise;
    groundNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    groundNoise.SetFrequency(0.005);
    groundNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
    groundNoise.SetFractalOctaves(5);

    FastNoiseLite cloudNoise;
    cloudNoise.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);
    cloudNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
    cloudNoise.SetFractalOctaves(5);
    float animationSpeed = 0.05f;

    float ox = 66.0f;
    float oy = 66.0f;
    float oz = 66.0f;
    float zoom = 800.0f;

    // Primera capa: Océano (dos tonos)
    float firstLayer = seaNoise.GetNoise((x + ox) * zoom, (y + oy) * zoom, (z + oz) * zoom);

    glm::vec3 tmpColor;
    tmpColor = glm::mix(seaColor, oceanColor, firstLayer);

    // Segunda capa: Continentes
    float secondLayer = groundNoise.GetNoise((x + ox) * zoom, (y + oy) * zoom, (z + oz) * zoom);

    if (secondLayer > 0.25)
    {
        tmpColor = plantColor;
    }
    else if (secondLayer > 0.1)
    {
        tmpColor = groundColor;
    }
    else if (secondLayer > 0.05)
    {
        tmpColor = dirtColor;
    }

    // Tercera capa: Nubes animadas
    zoom = 500.0f;
    float thirdLayer = cloudNoise.GetNoise((x)*zoom, (y + frame * animationSpeed) * zoom, z * zoom);
    if (thirdLayer > 0.05)
    {
        tmpColor = tmpColor + (cloudColor * thirdLayer);
    }

    fragmentColor = Color(tmpColor.x, tmpColor.y, tmpColor.z);

    fragment.color = fragmentColor * fragment.intensity;

    return fragment;
}

Fragment moonShader(Fragment &fragment, int frame)
{
    Color fragmentColor;

    float x = fragment.original.x;
    float y = fragment.original.y;
    float z = fragment.original.z;

    glm::vec3 mainColor = {213.0f / 255, 203.0f / 255, 190.0f / 255};
    glm::vec3 secondColor = {182.0f / 255, 159.0f / 255, 144.0f / 255};
    glm::vec3 craterColor = {33.0f / 255, 31.0f / 255, 27.0f / 255};

    FastNoiseLite mainNoise;
    mainNoise.SetNoiseType(FastNoiseLite::NoiseType_Cellular);
    mainNoise.SetFrequency(0.05);
    mainNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
    mainNoise.SetFractalOctaves(8);

    FastNoiseLite craterNoise;
    craterNoise.SetNoiseType(FastNoiseLite::NoiseType_Value);
    craterNoise.SetFrequency(0.01);

    float ox = 66.0f;
    float oy = 66.0f;
    float oz = 66.0f;
    float zoom = 800.0f;

    // Primera capa: Superficie (dos tonos)
    float firstLayer = mainNoise.GetNoise((x + ox) * zoom, (y + oy) * zoom, (z + oz) * zoom);

    glm::vec3 tmpColor;
    tmpColor = glm::mix(mainColor, secondColor, firstLayer);

    // Segunda capa: Continentes
    zoom = 500.0f;
    float secondLayer = craterNoise.GetNoise((x + ox) * zoom, (y + oy) * zoom, (z + oz) * zoom);

    if (secondLayer > 0.1)
    {
        tmpColor = glm::mix(tmpColor, craterColor * firstLayer, secondLayer);
    }

    fragmentColor = Color(tmpColor.x, tmpColor.y, tmpColor.z);

    fragment.color = fragmentColor * fragment.intensity;

    return fragment;
}

Fragment jestefadShader(Fragment &fragment, int frame)
{
    Color fragmentColor;

    float x = fragment.original.x;
    float y = fragment.original.y;
    float z = fragment.original.z;

    glm::vec3 mainColor = {126.0f / 255, 7.0f / 255, 22.0f / 255};
    glm::vec3 darkColor = {61.0f / 255, 11.0f / 255, 15.0f / 255};
    glm::vec3 fireColor = {223.0f / 255, 75.0f / 255, 40.0f / 255};
    glm::vec3 cloudColor = {206.0f / 255, 216.0f / 255, 210.0f / 255};

    FastNoiseLite mainNoise;
    mainNoise.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);
    mainNoise.SetFrequency(0.01);
    mainNoise.SetFractalType(FastNoiseLite::FractalType_PingPong);
    mainNoise.SetFractalOctaves(5);

    FastNoiseLite fireNoise;
    fireNoise.SetNoiseType(FastNoiseLite::NoiseType_Value);
    fireNoise.SetFrequency(0.035);
    fireNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
    fireNoise.SetFractalOctaves(3);
    float fireSpeed = 0.1;

    FastNoiseLite cloudNoise;
    cloudNoise.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);
    cloudNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
    cloudNoise.SetFractalOctaves(5);
    float animationSpeed = 0.05f;

    float ox = 0.05f;
    float oy = 66.0f;
    float oz = 66.0f;
    float zoom = 800.0f;

    // Primera capa: Gas caliente (dos tonos)
    float firstLayer = mainNoise.GetNoise((x + ox * frame) * zoom, (y + oy) * zoom, (z + oz) * zoom);

    glm::vec3 tmpColor;
    tmpColor = glm::mix(mainColor, darkColor, firstLayer);

    // Segunda capa: Fuego y destellos
    zoom = 200.0f;
    float secondLayer = fireNoise.GetNoise((x + frame * fireSpeed) * zoom, (y + oy) * zoom, (z + oz) * zoom);

    if (secondLayer > 0.7)
    {
        tmpColor = fireColor * secondLayer;
    }

    // Tercera capa: Nubes animadas
    zoom = 500.0f;
    float thirdLayer = cloudNoise.GetNoise((x)*zoom, (y + frame * animationSpeed) * zoom, z * zoom);
    if (thirdLayer > 0.01)
    {
        tmpColor = tmpColor + (cloudColor * thirdLayer);
    }

    fragmentColor = Color(tmpColor.x, tmpColor.y, tmpColor.z);

    fragment.color = fragmentColor * fragment.intensity;

    return fragment;
}

Fragment tethShader(Fragment &fragment, int frame)
{
    Color fragmentColor;

    float x = fragment.original.x;
    float y = fragment.original.y;
    float z = fragment.original.z;

    glm::vec3 mainColor = {102.0f / 255, 67.0f / 255, 102.0f / 255};
    glm::vec3 darkColor = {65.0f / 255, 40.0f / 255, 66.0f / 255};
    glm::vec3 stormMainColor = {234.0f / 255, 210.0f / 255, 243.0f / 255};
    glm::vec3 stormDarkColor = {162.0f / 255, 95.0f / 255, 170.0f / 255};
    glm::vec3 cloudMainColor = {167.0f / 255, 121.0f / 255, 147.0f / 255};
    glm::vec3 cloudDarkColor = {78.0f / 255, 32.0f / 255, 47.0f / 255};

    FastNoiseLite mainNoise;
    mainNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    mainNoise.SetFrequency(0.01);

    FastNoiseLite stormNoise;
    stormNoise.SetNoiseType(FastNoiseLite::NoiseType_Perlin);
    stormNoise.SetFrequency(0.005);
    stormNoise.SetFractalType(FastNoiseLite::FractalType_PingPong);
    stormNoise.SetFractalOctaves(3);
    stormNoise.SetFractalLacunarity(1.5);
    float stormSpeed = 0.05f;

    float ox = 0.05f;
    float oy = 66.0f;
    float oz = 66.0f;
    float zoom = 800.0f;

    // Primera capa: Colores principales (dos tonos)
    float firstLayer = mainNoise.GetNoise((x + ox) * zoom, (y + oy) * zoom, (z + oz) * zoom);

    glm::vec3 tmpColor;
    tmpColor = glm::mix(mainColor, darkColor, firstLayer);

    // Segunda capa: Tormenta
    zoom = 800.0f;
    float secondLayer = stormNoise.GetNoise((x + frame * stormSpeed) * zoom, (y + oy) * zoom, (z + oz) * zoom);
    tmpColor = glm::mix(stormMainColor, tmpColor, secondLayer);

    fragmentColor = Color(tmpColor.x, tmpColor.y, tmpColor.z);

    fragment.color = fragmentColor * fragment.intensity;

    return fragment;
}

Fragment criamShader(Fragment &fragment, int frame)
{
    Color fragmentColor;

    float x = fragment.original.x;
    float y = fragment.original.y;
    float z = fragment.original.z;

    glm::vec3 mainColor = {120.0f / 255, 175.0f / 255, 93.0f / 255};
    glm::vec3 darkColor = {154.0f / 255, 145.0f / 255, 103.0f / 255};
    glm::vec3 continentColor = {175.0f / 255, 180.0f / 255, 107.0f / 255};
    glm::vec3 cloudColor = {1.0f, 1.0f, 1.0f};

    FastNoiseLite mainNoise;
    mainNoise.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);
    mainNoise.SetFrequency(0.04);
    mainNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
    mainNoise.SetFractalOctaves(3);

    FastNoiseLite continentNoise;
    continentNoise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2S);
    continentNoise.SetFrequency(0.005);
    continentNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
    continentNoise.SetFractalOctaves(3);

    FastNoiseLite cloudNoise;
    cloudNoise.SetNoiseType(FastNoiseLite::NoiseType_ValueCubic);
    cloudNoise.SetFractalType(FastNoiseLite::FractalType_FBm);
    cloudNoise.SetFractalOctaves(5);
    float animationSpeed = 0.05f;

    float ox = 66.0f;
    float oy = 66.0f;
    float oz = 66.0f;
    float zoom = 700.0f;

    // Primera capa: Colores principales (dos tonos)
    float firstLayer = mainNoise.GetNoise((x + ox) * zoom, (y + oy) * zoom, (z + oz) * zoom);

    glm::vec3 tmpColor;
    tmpColor = glm::mix(mainColor, darkColor, firstLayer);

    // Segunda capa: Continentes
    float secondLayer = continentNoise.GetNoise((x + ox) * zoom, (y + oy) * zoom, (z + oz) * zoom);

    if (secondLayer > 0)
    {
        tmpColor = glm::mix(darkColor, continentColor, secondLayer);
    }

    // Tercera capa: Nubes animadas
    zoom = 500.0f;
    float thirdLayer = cloudNoise.GetNoise((x)*zoom, (y + frame * animationSpeed) * zoom, z * zoom);
    if (thirdLayer > 0.05)
    {
        tmpColor = tmpColor + (cloudColor * thirdLayer);
    }

    fragmentColor = Color(tmpColor.x, tmpColor.y, tmpColor.z);

    fragment.color = fragmentColor * fragment.intensity;

    return fragment;
}

std::vector<Fragment> fragmentShader(Fragment &fragment, int frame, int planetCase)
{
    std::vector<Fragment> planets;
    switch (planetCase)
    {
    case 0:
        planets.push_back(whiteDwarfShader(fragment, frame));
        break;
    case 1:
        planets.push_back(nabooShader(fragment, frame));
        break;
    case 2:
        planets.push_back(jestefadShader(fragment, frame));
        break;
    case 3:
        planets.push_back(tethShader(fragment, frame));
        break;
    case 4:
        planets.push_back(criamShader(fragment, frame));
        break;
    case 5:
        planets.push_back(redGiantShader(fragment, frame));
        break;
    case 6:
        planets.push_back(moonShader(fragment, frame));
        break;
    }
    return planets;
}