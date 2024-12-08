#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "shader.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <exception>
#include <glm/glm.hpp>

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
    // ensures ifstream object can throw exceptions
    file.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    try {
        std::stringstream stream;

        file.open(filepath);
        stream << file.rdbuf();
        file.close();

        // the strdup is to stop the string from disappearing when the stream is destroyed
        return strdup(stream.str().c_str());
    } catch (std::ifstream::failure *e) {
        std::cout << "ERROR::FILE::NOT_SUCCESFULLY_READ" << std::endl;
        throw e;
    }
}

const float pi = 3.14159; // i'm too lazy to copy paste std::numbers::pi

int main()
{

    // Setting up the paths
    std::filesystem::path currentPath = std::filesystem::current_path();
    std::filesystem::path vertexPath = currentPath / "shaders/shader.vs";
    std::filesystem::path fragPath = currentPath / "shaders/shader.fs";
    std::filesystem::path iconPath = currentPath / "assets/icon.png";

    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);

    const unsigned int SCR_WIDTH = 800, SCR_HEIGHT = 800;
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
    // generate a vertex buffer handle and bind it to GL_ARRAY_BUFFER
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

    setFloat(shader, "width", SCR_WIDTH);
    setFloat(shader, "height", SCR_HEIGHT);

    glm::vec3 front = glm::vec3(0.0, 0.0, 1.0);
    glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
    glm::vec3 right = glm::vec3(1.0, 0.0, 0.0);
    glm::mat3 defaultcamera = glm::mat3(right, up, front);
    
    const int dtlen = 20;
    double dts[dtlen];
    
    for (int i = 0; i < dtlen; i++) {
        dts[i] = 0.0;
    }
    
    int dti = 0;

    glm::vec3 campos = glm::vec3(-1.0, 0.0, 0.0);
    glm::mat3 camtrans = glm::mat3(
        1.0, 0.0, 0.0,
        0.0, 1.0, 0.0,
        0.0, 0.0, 1.0
    );

    //the render loop
    const long long startTick = SDL_GetPerformanceCounter();
    double lastFrame = 0;
    while (!closed)
    {
        double currentFrame = ((double)(SDL_GetPerformanceCounter() - startTick)) / SDL_GetPerformanceFrequency();
        double deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        
        dts[dti] = deltaTime;
        dti += 1;
        dti %= dtlen;
        
        double totdt = 0.0;
        
        for (int i = 0; i < dtlen; i++) {
            totdt += dts[i];
        }

        // swap the rendered buffer with the next image render buffer
        SDL_GL_SwapWindow(window);

        // rendering command
        // no clear or z buffer because it redraws the whole screen anyway
        // set the camera location and transformation matrix
        setMat3(shader, "camera", defaultcamera * camtrans);
        setVec3(shader, "campos", campos);
        // seconds since start
        setFloat(shader, "time", currentFrame);
        // fps meter
        setFloat(shader, "fps", 1.0 / (totdt / dtlen));
        // draw over the whole screen
        glDrawArrays(GL_TRIANGLES, 0, 6);

        SDL_Event event;

        // check if any events were triggered
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
              case SDL_QUIT: // x button
                closed = true;
                break;
              case SDL_MOUSEMOTION:
                if (SDL_GetMouseState(NULL, NULL)) {
                    float angle = atan2(event.motion.yrel, event.motion.xrel);
                    float mag = 2 * sqrt(((float)event.motion.xrel) / SCR_WIDTH * ((float)event.motion.xrel) / SCR_WIDTH + ((float)event.motion.yrel) / SCR_HEIGHT * ((float)event.motion.yrel) / SCR_HEIGHT);
                    camtrans = glm::mat3(
                        cos(angle), -sin(angle), 0.0,
                        sin(angle), cos(angle), 0.0,
                        0.0, 0.0, 1.0
                    ) * glm::mat3(
                        cos(mag), 0.0, -sin(mag),
                        0.0, 1.0, 0.0,
                        sin(mag), 0.0, cos(mag)
                    ) * glm::mat3(
                        cos(angle), sin(angle), 0.0,
                        -sin(angle), cos(angle), 0.0,
                        0.0, 0.0, 1.0
                    ) * camtrans;
                    event.motion.x;
                    event.motion.y;
                }
              default:
                break;
            }
        }

        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE] == 1) { // if escape is pressed, quit
            closed = true;
        }
        
        // strafe
        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_UP] == 1) {
            campos += glm::vec3(0.0, 0.1, 0.0) * camtrans;
        }
        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_DOWN] == 1) {
            campos += glm::vec3(0.0, -0.1, 0.0) * camtrans;
        }
        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_LEFT] == 1) {
            campos += glm::vec3(-0.1, 0.0, 0.0) * camtrans;
        }
        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_RIGHT] == 1) {
            campos += glm::vec3(0.1, 0.0, 0.0) * camtrans;
        }

        // fly
        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_SPACE] == 1) {
            campos += glm::vec3(0.0, 0.0, 0.1) * camtrans;
        }
        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_BACKSPACE] == 1) {
            campos += glm::vec3(0.0, 0.0, -0.1) * camtrans;
        }
        
        // look
        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_W] == 1) {
            camtrans = glm::mat3(
                1.0, 0.0, 0.0,
                0.0, cos(0.1), sin(0.1),
                0.0, -sin(0.1), cos(0.1)
            ) * camtrans;
        }
        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_S] == 1) {
            camtrans = glm::mat3(
                1.0, 0.0, 0.0,
                0.0, cos(0.1), -sin(0.1),
                0.0, sin(0.1), cos(0.1)
            ) * camtrans;
        }
        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_A] == 1) {
            camtrans = glm::mat3(
                cos(0.1), 0.0, -sin(0.1),
                0.0, 1.0, 0.0,
                sin(0.1), 0.0, cos(0.1)
            ) * camtrans;
        }
        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_D] == 1) {
            camtrans = glm::mat3(
                cos(0.1), 0.0, sin(0.1),
                0.0, 1.0, 0.0,
                -sin(0.1), 0.0, cos(0.1)
            ) * camtrans;
        }

        // twist
        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_Q] == 1) {
            camtrans = glm::mat3(
                cos(0.1), -sin(0.1), 0.0,
                sin(0.1), cos(0.1), 0.0,
                0.0, 0.0, 1.0
            ) * camtrans;
        }
        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_E] == 1) {
            camtrans = glm::mat3(
                cos(0.1), sin(0.1), 0.0,
                -sin(0.1), cos(0.1), 0.0,
                0.0, 0.0, 1.0
            ) * camtrans;
        }

        // weird :)
        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_Y] == 1) {
            camtrans = glm::mat3(
                1.0, 0.1, 0.0,
                0.0, 1.0, 0.0,
                0.0, 0.0, 1.0
            ) * camtrans;
        }
        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_H] == 1) {
            camtrans = glm::mat3(
                1.0, -0.1, 0.0,
                0.0, 1.0, 0.0,
                0.0, 0.0, 1.0
            ) * camtrans;
        }
        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_U] == 1) {
            camtrans = glm::mat3(
                1.0, 0.0, 0.0,
                0.1, 1.0, 0.0,
                0.0, 0.0, 1.0
            ) * camtrans;
        }
        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_J] == 1) {
            camtrans = glm::mat3(
                1.0, 0.0, 0.0,
                -0.1, 1.0, 0.0,
                0.0, 0.0, 1.0
            ) * camtrans;
        }
        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_I] == 1) {
            camtrans = glm::mat3(
                1.0, 0.0, 0.0,
                0.0, 1.0, 0.0,
                0.1, 0.0, 1.0
            ) * camtrans;
        }
        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_K] == 1) {
            camtrans = glm::mat3(
                1.0, 0.0, 0.0,
                0.0, 1.0, 0.0,
                -0.1, 0.0, 1.0
            ) * camtrans;
        }
        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_O] == 1) {
            camtrans = glm::mat3(
                1.0, 0.0, 0.0,
                0.0, 1.0, 0.0,
                0.0, 0.1, 1.0
            ) * camtrans;
        }
        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_L] == 1) {
            camtrans = glm::mat3(
                1.0, 0.0, 0.0,
                0.0, 1.0, 0.0,
                0.0, -0.1, 1.0
            ) * camtrans;
        }
    }
    // delete VBO because we're done with it
    glDeleteBuffers(1, &VBO);

    // quit the sdl library
    SDL_GL_DeleteContext(gl_context);
    SDL_Quit();

    return 0;
}
