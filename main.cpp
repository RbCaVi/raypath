#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "shader.h"
#include <filesystem>
#include <string>


//functions used later in the program for, framebuffer & getting input
void framebuffer_size_callback(SDL_Window *window, int width, int height);
void processInput(SDL_Window *window);
//sets up the mouse
void mouse_callback(SDL_Window *window, double xpos, double ypos);
void scroll_callback(SDL_Window *window, double xoffset, double yoffset);

//icon image
SDL_Surface *iconImage;

//Path to all relevant files
std::filesystem::path currentPath = std::filesystem::current_path();
std::filesystem::path vertexPath;
std::filesystem::path fragPath;
std::filesystem::path iconPath;
std::filesystem::path tex1Path;
std::filesystem::path tex2Path;

//Vertex Array Object (i.e stores vertex attributes)
unsigned int VAO;

//Vertex Buffer Object (stores GPU memory for the vertex shader)
unsigned int VBO;

//Element Buffer Object (stores element array gpu memory)
unsigned int EBO;

//const default screen sizes
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 800;

//changes in the cameras speed
float changeInCameraSpeed = 0.0f;

//setting the inital mouse position
float lastX = SCR_WIDTH/2, lastY = SCR_HEIGHT/2;

//deltatime & lastframe variables
float deltaTime = 0.0f;
float lastFrame = 0.0f;

//direction for the rotation
float yaw = -90.0f;
float pitch = 0.0f;
//bool firstMouse = true; // not used?
float fov = 45.0f;


//vertex data for a cube
//first 3 values are the (x,y,z), the last 2 values are (s,t) for textures
float vertices[] = {
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
     0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
     0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
     0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
     0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
    -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
    -0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
//EBA indices
unsigned int indices[] = {
    0,1,2,
    0,2,3
};

//translation for the cube positions using vec3
glm::vec3 cubePositions[] = {
    glm::vec3(0.0f,  0.0f,  0.0f),
    glm::vec3(2.0f,  5.0f, -15.0f),
    glm::vec3(-1.5f, -2.2f, -2.5f),
    glm::vec3(-3.8f, -2.0f, -12.3f),
    glm::vec3(2.4f, -0.4f, -3.5f),
    glm::vec3(-1.7f,  3.0f, -7.5f),
    glm::vec3(1.3f, -2.0f, -2.5f),
    glm::vec3(1.5f,  2.0f, -2.5f),
    glm::vec3(1.5f,  0.2f, -1.5f),
    glm::vec3(-1.3f,  1.0f, -1.5f)
};

glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
//--glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
//--glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
//--glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);

/* float texCoords[] = {
    0.0f, 0.0f,
    1.0f, 0.0f,
    0.5f, 1.0f
}; */

// time warp
float extraTime = 0.0f;

// if the window should close... NOW!
bool closed;

//Prepares the paths
void preparePath() {
    // shouldn't these paths be inlined into main?
    // (i changed them from string to std::filesystem::path and moved the additional paths to here)
    vertexPath = currentPath / "shaders/shader.vs";
    fragPath = currentPath / "shaders/shader.fs";
    tex1Path = currentPath / "assets/milly.png";
    tex2Path = currentPath / "assets/boba.png";
    iconPath = currentPath / "assets/icon.png";
    std::cout << currentPath << vertexPath.c_str() << tex2Path.c_str() << '\n';
}

int main()
{
    //Setting up the path
    preparePath();
  
    SDL_Init(SDL_INIT_TIMER | SDL_INIT_VIDEO);

    SDL_Window *window = SDL_CreateWindow(":3 UwU XD SillyWindow", 0, 0, SCR_WIDTH, SCR_HEIGHT, SDL_WINDOW_OPENGL);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    float startTime = SDL_GetPerformanceCounter() / SDL_GetPerformanceFrequency();

    //checks that the new window isnt null, if it is then it will terminate with an error
    if (window == NULL)
    {
        std::cout << "Failed to create window: " << SDL_GetError() << std::endl;
        SDL_Quit();
        return -1;
    }

    iconImage = IMG_Load(iconPath.c_str());
    SDL_SetWindowIcon(window, iconImage);
    SDL_FreeSurface(iconImage);

    //framebuffer variables // these are only used here, so i moved them
    int framebufferWidth;
    int framebufferHeight;

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

    
    //generates a vertex attribute array
    glGenVertexArrays(1, &VAO);
    //Generates a vertex buffer, setting VBO as an id to it
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    

    //binds the vertex array object
    glBindVertexArray(VAO);
    //binds the array buffer to the VBO
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    //binds ebo buffer to the EBO
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);


    //loads the vertices data into the buffer for the gpu to use
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    //loads indicies data into the ebo buffer for the gpu
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);
    
    //sets the proper attributes for the vertex data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
    //enables vertex attributes
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);

    //--glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    //--glEnableVertexAttribArray(2);


    //textures
    unsigned int texture1, texture2;
    //generates silly milly texture
    glGenTextures(1, &texture1);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_MIRRORED_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    int width, height, nrChannels;
    SDL_Surface *image1 = SDL_ConvertSurfaceFormat(IMG_Load(tex1Path.c_str()), SDL_PIXELFORMAT_RGBA8888, 0);
    if (image1) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image1->w, image1->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image1->pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture" << std::endl;
    }
    
    SDL_FreeSurface(image1);

    //generates a texture for boba tea
    glGenTextures(1, &texture2);
    glBindTexture(GL_TEXTURE_2D, texture2);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    SDL_Surface *image2 = SDL_ConvertSurfaceFormat(IMG_Load(tex2Path.c_str()), SDL_PIXELFORMAT_RGBA8888, 0);
    if (image2) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image2->w, image2->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, image2->pixels);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else {
        std::cout << "Failed to load texture 2" << std::endl;
    }
    
    SDL_FreeSurface(image2);

    //sets the texture uniforms
    ourShader.use();
    ourShader.setInt("texture1", 0);
    ourShader.setInt("texture2", 1);

    //Enables the Z-BUFFER
    glEnable(GL_DEPTH_TEST);
    
    // these look like one off kind of things (surely you don't have to reregister the callbacks every frame right?) (moved from processInput)
    //disables visible cursor capture
    SDL_ShowCursor(SDL_DISABLE);
    SDL_SetWindowGrab(window, SDL_TRUE);

    //sets & binds each of the textures
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, texture1);
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, texture2);
  
    closed=false;

    //the render loop
    while (!closed)
    {
        //swaps the rendered buffer with the next image render buffer
        SDL_GL_SwapWindow(window);
        //a function to handle input
        processInput(window);
        //rendering commands
        //sets the back color of the toberendered buffer to the rgba values
        glClearColor(0.4f, 0.3f, 0.5f, 1.0f);
        //clears it to the the color buffer (i.e. the clear color setting) & uses the z-buffer
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //gets the deltaTime using differing times & frames
        float currentFrame = SDL_GetPerformanceCounter() / SDL_GetPerformanceFrequency();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        //uses the program
        ourShader.use();


        //Base mat4 coordinate transformations
        glm::mat4 view;
        glm::mat4 projection;

        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        //sets the value for each mat4 transformation in coordinate spaces
        projection = glm::perspective(glm::radians(fov), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
        
        //model render loop
        for (unsigned int i = 0; i < 10; i++) {

            glm::mat4 model = glm::mat4(1.0f);

            float amountRotatedAngle = -10.0f * i;
            float deltaRotatedAngle = 50.0f + (i * 50);


            model = glm::translate(model, cubePositions[i]);
            model = glm::rotate(model, glm::radians(amountRotatedAngle), glm::vec3(1.0f, 0.3f, 0.5f));


            //rotates the local space by 50 rads over time
            model = glm::rotate(model, ((float)(SDL_GetPerformanceCounter() / SDL_GetPerformanceFrequency() - startTime) + extraTime) * glm::radians(deltaRotatedAngle), glm::vec3(0.5f, 1.0f, 0.0f));

            ourShader.setMat4("model", model);

            //draws vertexs from the VAO that pulls each vertex point to draw,
            //and draws each VAO as an element of a triangle
            glDrawArrays(GL_TRIANGLES, 0, 36);
        }
        //--glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    
        SDL_Event event;

        //checks if any events were triggered (i.e. input from kb&m)
        while (SDL_PollEvent(&event)) {
            switch (event.type) {
              case SDL_MOUSEWHEEL:
                scroll_callback(window, event.wheel.x, event.wheel.y);
                break;
              case SDL_KEYUP:
                mouse_callback(window, event.motion.x, event.motion.y);
                break;
              case SDL_QUIT:
                // handling of close button
                closed = true;
                break;
              default:
                break;
            }
        }
    }
    //delete the unused arrays
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO); // does this need to be freed?
    //glDeleteProgram(shaderProgram); // shaderProgram is never initialized

    //ends the glfw library
    SDL_Quit();
    return 0;
}

//takes in the input while window is active
void processInput(SDL_Window *window) {
    //camera movement speed
    float cameraSpeed = 2.5f * deltaTime + changeInCameraSpeed;
    //right normalized vector
    glm::vec3 rightDirectionVector = glm::normalize(glm::cross(cameraFront, cameraUp));
    //if esc is pressed then close the window
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_ESCAPE] == 1)
        closed = true;
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_1] == 1) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_2] == 1) {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_W] == 1) {
        cameraPos += cameraSpeed * cameraFront;
    }
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_S] == 1) {
        cameraPos -= cameraSpeed * cameraFront;
    }
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_A] == 1) {
        cameraPos -= rightDirectionVector * cameraSpeed;
    }
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_D] == 1) {
        cameraPos += rightDirectionVector * cameraSpeed;
    }
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_PAGEUP] == 1) {
        changeInCameraSpeed += 0.01f;
    }
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_PAGEDOWN] == 1) {
        changeInCameraSpeed -= 0.01f;
    }
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_SPACE] == 1) {
        extraTime += 4 * deltaTime;
    }
    if (SDL_GetKeyboardState(NULL)[SDL_SCANCODE_BACKSPACE] == 1) {
        extraTime -= 6 * deltaTime;
    }
}
//handles mouse input functionality
void mouse_callback(SDL_Window *window, double xpos, double ypos) {
    //captures the offset of the mouse's position
    float xoffset = xpos - lastX;
    float yoffset = ypos - lastY;
        lastX = xpos;
        lastY = ypos;

    //the amount of power that the capture has
    const float sensitivity = 0.1f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    //updates the yaw and pitch according to the offset
    yaw += -xoffset;
    pitch += yoffset;

    //pitch parameters
    if (pitch > 89.0f) {
        pitch = 89.0f;
    } if (pitch < -89.0f) {
        pitch = -89.0f;
    }
    //using the input variables to make the new direction & normalizing it
    glm::vec3 direction = glm::vec3(cos(glm::radians(yaw)) * cos(glm::radians(pitch)),
        sin(glm::radians(pitch)),
        sin(glm::radians(yaw)) * cos(glm::radians(pitch)));
    cameraFront = glm::normalize(direction);
}
void scroll_callback(SDL_Window *window, double xoffset, double yoffset) {
    //takes in the scroll wheel's offset and sets parameters
    fov -= (float)yoffset;
    if (fov < 1.0f) {
        fov = 1.0f;
    } if (fov > 45.0f) {
        fov = 45.0f;
    }
}
//sets the framebuffersize to change so the viewport adjusts
void framebuffer_size_callback(SDL_Window *window, int width, int height) {
    glViewport(0, 0, width, height);
}
