#include <SDL.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <Windows.h>
#include "color.h"
#include "framebuffer.h"
#include "objReader.h"
#include "uniforms.h"
#include "shaders.h"
#include "triangle.h"
#include "camera.h"
#include "primitiveAssembly.h"
#include "rasterize.h"
#include "matrixes.h"
#include "fragment.h"
#include "planet.h"

Planet planets[] = {
    {glm::radians(0.0f),
     glm::vec3(0.0f, 0.0f, 0.0f),
     glm::vec3(1.0f, 1.0f, 1.0f),
     false},
    {glm::radians(0.0f),
     glm::vec3(0.0f, 0.0f, 0.0f),
     glm::vec3(1.0f, 1.0f, 1.0f),
     true},
    {glm::radians(0.0f),
     glm::vec3(0.0f, 0.0f, 0.0f),
     glm::vec3(1.0f, 1.0f, 1.0f),
     false},
    {glm::radians(0.0f),
     glm::vec3(0.0f, 0.0f, 0.0f),
     glm::vec3(1.0f, 1.0f, 1.0f),
     false},
    {glm::radians(0.0f),
     glm::vec3(0.0f, 0.0f, 0.0f),
     glm::vec3(1.0f, 1.0f, 1.0f),
     false},
    {glm::radians(0.0f),
     glm::vec3(0.0f, 0.0f, 0.0f),
     glm::vec3(1.0f, 1.0f, 1.0f),
     false},
    {glm::radians(0.0f),
     glm::vec3(1.0f, 0.3f, 0.0f),
     glm::vec3(0.2f, 0.2f, 0.2f),
     false},
};

float rotationAngle = glm::radians(0.0f);
const float SCREEN_WIDTH = GetSystemMetrics(SM_CXSCREEN);
const float SCREEN_HEIGHT = GetSystemMetrics(SM_CYSCREEN);
int frame = 0;
Framebuffer framebuffer = Framebuffer(SCREEN_WIDTH, SCREEN_HEIGHT);
int celestialBody = 0;

Color clearColor = Color(0, 0, 0);
glm::vec3 light = glm::vec3(0.0f, 0.0f, 1.0f);
Color mainColor = Color(255, 255, 255);
const std::string modelPath = "../models/sphere.obj";

SDL_Window *window = nullptr;
SDL_Renderer *renderer = nullptr;
std::vector<glm::vec3> vertices;
std::vector<glm::vec3> normals;
std::vector<Face> faces;
std::vector<Vertex> verticesArray;

Uniforms uniforms;

glm::mat4 model = glm::mat4(1);
glm::mat4 view = glm::mat4(1);
glm::mat4 projection = glm::mat4(1);

bool init()
{
    if (SDL_Init(SDL_INIT_VIDEO) != 0)
    {
        std::cerr << "Error: No se puedo inicializar SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    window = SDL_CreateWindow("SR 2: Flat Shading", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    if (!window)
    {
        std::cerr << "Error: No se pudo crear una ventana SDL: " << SDL_GetError() << std::endl;
        return false;
    }

    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    if (!renderer)
    {
        std::cerr << "Error: No se pudo crear SDL_Renderer: " << SDL_GetError() << std::endl;
        return false;
    }

    return true;
}

void render(Primitive polygon, int planetIndex)
{

    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
    SDL_RenderClear(renderer);

    // 1. Vertex Shader
    std::vector<Vertex> transformedVertices;
    for (const Vertex &vertex : verticesArray)
    {
        transformedVertices.push_back(vertexShader(vertex, uniforms));
    }

    // 2. Primitive Assembly
    std::vector<std::vector<Vertex>> assembledVertices = primitiveAssembly(polygon, transformedVertices);

    // 3. Rasterization
    std::vector<Fragment> fragments = rasterize(polygon, assembledVertices, SCREEN_WIDTH, SCREEN_HEIGHT, framebuffer.getLight(), framebuffer.getMainColor());

    // 4. Fragment Shader
    for (Fragment &fragment : fragments)
    {
        // Apply the fragment shader to compute the final color
        std::vector<Fragment> fragments;
        fragments = fragmentShader(fragment, frame, planetIndex);
        if (fragments.size() > 0)
            for (Fragment &f : fragments)
            {
                framebuffer.point(f);
            }
    }
}

int main(int argv, char **args)
{
    framebuffer.setClearColor(clearColor);
    framebuffer.setMainColor(mainColor);
    framebuffer.setLight(light);

    if (!init())
    {
        return 1;
    }

    framebuffer.clear();

    loadOBJ(modelPath, vertices, normals, faces);
    verticesArray = setupVertexArray(vertices, normals, faces);

    glm::vec3 rotationAxis(0.0f, 1.0f, 0.0f); // Rotar alrededor del eje Y

    // Inicializar cámara
    Camera camera;
    camera.cameraPosition = glm::vec3(0.0f, 0.0f, 7.0f);
    camera.targetPosition = glm::vec3(0.0f, 0.0f, 0.0f);
    camera.upVector = glm::vec3(0.0f, 1.0f, 0.0f);

    // Matriz de proyección
    uniforms.projection = createProjectionMatrix(SCREEN_WIDTH, SCREEN_HEIGHT);

    // Matriz de viewport
    uniforms.viewport = createViewportMatrix(SCREEN_WIDTH, SCREEN_HEIGHT);

    // Matriz de vista

    int speed = 2;
    int counter = 0;

    bool isRunning = true;
    while (isRunning)
    {
        frame += 1;
        SDL_Event event;
        while (SDL_PollEvent(&event) != 0)
        {
            if (event.type == SDL_QUIT)
            {
                isRunning = false;
            }
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_UP:
                    if (counter < 2)
                    {
                        camera.cameraPosition.z += -speed;
                        counter += 1;
                    }
                    break;
                case SDLK_DOWN:
                    camera.cameraPosition.z += speed;
                    counter -= 1;
                    break;
                case SDLK_LEFT:
                    celestialBody = celestialBody == 0 ? 5 : celestialBody - 1;
                    break;
                case SDLK_RIGHT:
                    celestialBody = celestialBody == 5 ? 0 : celestialBody + 1;
                    break;
                }
            }
        }

        SDL_Delay(1);

        framebuffer.clear();

        // Crear la matriz de vista usando el objeto cámara
        uniforms.view = glm::lookAt(
            camera.cameraPosition, // The position of the camera
            camera.targetPosition, // The point the camera is looking at
            camera.upVector        // The up vector defining the camera's orientation
        );

        int planetsSize = sizeof(planets) / sizeof(planets[0]);

        Planet model = planets[celestialBody];

        if (model.hasMoon)
        {
            Planet moon = planets[planetsSize - 1];
            moon.translation.x = 1.0f * cos(glm::radians(rotationAngle-1));
            moon.translation.z = 1 - (1.0f * sin(glm::radians(rotationAngle+1)));
            rotationAngle += 10.0f;
            glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), rotationAxis);

            glm::mat4 translation = glm::translate(glm::mat4(1.0f), moon.translation);
            glm::mat4 scale = glm::scale(glm::mat4(1.0f), moon.scale);

            // Calcular la matriz de modelo
            uniforms.model = translation * rotation * scale;
            render(Primitive::TRIANGLES, planetsSize - 1);

            framebuffer.renderBuffer(renderer);
        }

        rotationAngle += 1.0f;
        glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(rotationAngle), rotationAxis);

        glm::mat4 translation = glm::translate(glm::mat4(1.0f), model.translation);
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), model.scale);

        // Calcular la matriz de modelo
        uniforms.model = translation * rotation * scale;
        render(Primitive::TRIANGLES, celestialBody);

        framebuffer.renderBuffer(renderer);
        SDL_RenderPresent(renderer);
    }

    return 0;
}