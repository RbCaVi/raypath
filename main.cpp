#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "shader.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <exception>

// (x, y) vertices for a square
const float vertices[] = {
    -1.0f, -1.0f,
     1.0f, -1.0f,
     1.0f,  1.0f,
     1.0f,  1.0f,
    -1.0f,  1.0f,
    -1.0f, -1.0f,
};

const char *loadFile(std::filesystem::path &filepath) {
    // get the source code from filepath
    std::string content;
    std::ifstream file;
    //ensures ifstream object can throw exceptions
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        std::stringstream stream;

        file.open(filepath);
        stream << file.rdbuf();
        file.close();

        return strdup(stream.str().c_str());
    } catch (std::ifstream::failure *e) {
        std::cout << "ERROR::FILE::NOT_SUCCESFULLY_READ" << std::endl;
        throw e;
    }
}

int main()
{

    // Setting up the paths
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path vertexPath = currentPath / "shaders/shader.vs";
    std::filesystem::path fragPath = currentPath / "shaders/shader.fs";
    std::filesystem::path iconPath = currentPath / "assets/icon.png";

    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);

    unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 800;
    SDL_Window *window = SDL_CreateWindow(":3 UwU XD SillyWindow", 0, 0, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_OPENGL);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);

    //checks that the new window isnt null, if it is then it will terminate with an error
    if (window == NULL)
    {
        std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    //icon image
    SDL_Surface *iconImage = IMG_Load(iconPath.c_str());
    SDL_SetWindowIcon(window, iconImage);
    SDL_FreeSurface(iconImage);

    //loads in glad, sending an error if it doesnt
    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cout << "Failed to initalize GLAD" << std::endl;
        return -1;
    }

    // set the gl viewport (in gl, these are normalized to -1 to 1)
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // compile the shader
    unsigned int shader = compileShader(loadFile(vertexPath), loadFile(fragPath));

    // Vertex Buffer Object (stores GPU memory for the vertex shader)
    unsigned int VBO;
    // generate a vertex buffer and bind it to GL_ARRAY_BUFFER
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    // load the vertices data into the buffer for the gpu to use
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    // tell the gpu how the data is arranged
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    // enables vertex attributes (???)
    glEnableVertexAttribArray(0);

    glUseProgram(shader);

    // if the window should close... NOW!
    bool closed = false;

    //the render loop
    const long long startTick = SDL_GetPerformanceCounter();
    double lastFrame = 0;
    while (!closed)
    {
        double currentFrame = ((double)(SDL_GetPerformanceCounter() - startTick)) / SDL_GetPerformanceFrequency();
        double deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // swap the rendered buffer with the next image render buffer
        SDL_GL_SwapWindow(window);

        // rendering command
        // no clear or z buffer because it redraws the whole screen anyway
        setFloat(shader, "time", currentFrame);
        glDrawArrays(GL_TRIANGLES, 0, 6);

        SDL_Event event;

        // check if any events were triggered
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
              case SDL_QUIT: // x button
                closed = true;
                break;
              default:
                break;
            }
        }

        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE] == 1) { // if escape is pressed, quit
            closed = true;
        }
    }
    // delete VBO because we're done with it
    glDeleteBuffers(1, &VBO);

    // quit the sdl library
    SDL_GL_DeleteContext(gl_context);
    SDL_Quit();

    return 0;
}
