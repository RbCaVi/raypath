#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include "shader.h"
#include <filesystem>

// (x, y) vertices for a square
float vertices[] = {
    -1.0f, -1.0f,
     1.0f, -1.0f,
     1.0f,  1.0f,
     1.0f,  1.0f,
    -1.0f,  1.0f,
    -1.0f, -1.0f,
};

int main()
{
    //Vertex Buffer Object (stores GPU memory for the vertex shader)
    unsigned int VBO;

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

    //sets the gl viewport (normalized for -1 to 1)
    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    //gets the max vertex attributes attributed to each shader
    int nrAttributes;
    glGetIntegerv(GL_MAX_VERTEX_ATTRIBS, &nrAttributes);
    std::cout << "max num of vertex attributes supported: " << nrAttributes << std::endl;

    //compiles the shader

    Shader ourShader(vertexPath.c_str(), fragPath.c_str());

    //Generates a vertex buffer, setting VBO as an id to it
    glGenBuffers(1, &VBO);

    //binds the array buffer to the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);


    //loads the vertices data into the buffer for the gpu to use
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    //sets the proper attributes for the vertex data
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 2 * sizeof(float), (void*)0);
    //enables vertex attributes
    glEnableVertexAttribArray(0);

    ourShader.use();

    //Enables the Z-BUFFER
    glEnable(GL_DEPTH_TEST);

    // if the window should close... NOW!
    bool closed = false;

    //the render loop
    while (!closed)
    {
        //swaps the rendered buffer with the next image render buffer
        SDL_GL_SwapWindow(window);
        //rendering commands
        //sets the back color of the toberendered buffer to the rgba values
        glClearColor(0.4f, 0.3f, 0.5f, 1.0f);
        //clears it to the the color buffer (i.e. the clear color setting) & uses the z-buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //uses the program
        ourShader.use();

        glDrawArrays(GL_TRIANGLES, 0, 6);

        SDL_Event event;

        //checks if any events were triggered (i.e. input from kb&m)
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
              case SDL_QUIT:
                closed = true;
                break;
              default:
                break;
            }
        }

        if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE] == 1) {
            closed = true;
        }
    }
    //delete the unused arrays
    glDeleteBuffers(1, &VBO);

    //ends the sdl library
    SDL_GL_DeleteContext(gl_context);
    SDL_Quit();
    return 0;
}
