//Autor: Nedeljko Tesanovic
//Opis: Primjer upotrebe tekstura

#define _CRT_SECURE_NO_WARNINGS

#include <iostream>
#include <fstream>
#include <sstream>

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "src/button/Button.h"
#include "src/grid/Tile.h"
#include "src/grid/Grid.h"
#include <vector>


//stb_image.h je header-only biblioteka za ucitavanje tekstura.
//Potrebno je definisati STB_IMAGE_IMPLEMENTATION prije njenog ukljucivanja
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

unsigned int compileShader(GLenum type, const char* source);
unsigned int createShader(const char* vsSource, const char* fsSource);
static unsigned loadImageToTexture(const char* filePath); //Ucitavanje teksture, izdvojeno u funkciju

double mouseX = 0.0, mouseY = 0.0;
bool rotatePressed = false;
bool newGridPressed = false;
bool leftArrowPressed = false;
bool rightArrowPressed = false;
Button* selectedButton = nullptr;

const double TARGET_FPS = 60.0;
const double FRAME_DURATION = 1.0 / TARGET_FPS;

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS) {
        glfwGetCursorPos(window, &mouseX, &mouseY);
        std::cout << "Pozicija kursora inace: " << mouseX << ", " << mouseY << std::endl;
    }
}

int main(void)
{
    if (!glfwInit())
    {
        std::cout<<"GLFW Biblioteka se nije ucitala! :(\n";
        return 1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window;
    unsigned int wWidth = 1920;
    unsigned int wHeight = 1080;
    float normFactorX = 2.0f / wWidth;  // normFactorX = x piksel
    float normFactorY = 2.0f / wHeight; // normFactorY = y piksel
    const char wTitle[] = "Pokusaj 1";
    window = glfwCreateWindow(wWidth, wHeight, wTitle, NULL, NULL);

    double xpos, ypos;
    double previousTime = glfwGetTime();
    
    if (window == NULL)
    {
        std::cout << "Prozor nije napravljen! :(\n";
        glfwTerminate();
        return 2;
    }

    glfwMakeContextCurrent(window);


    if (glewInit() != GLEW_OK)
    {
        std::cout << "GLEW nije mogao da se ucita! :'(\n";
        return 3;
    }

    unsigned int buttonShader = createShader("shaders/Button/basic.vert", "shaders/Button/basic.frag");
    unsigned int gridShader = createShader("shaders/HalfTexture/basic.vert", "shaders/HalfTexture/basic.frag");

    //Tekstura
    unsigned defaultTexture = loadImageToTexture("res/default.png"); //Ucitavamo teksturu
    unsigned bedTexture = loadImageToTexture("res/Bed.png");
    unsigned chairTexture = loadImageToTexture("res/Chair.png");
    unsigned cDoorTexture = loadImageToTexture("res/Closed_door.png");
    unsigned cornerTexture = loadImageToTexture("res/Corner.png");
    unsigned doucheTexture = loadImageToTexture("res/douche.png"); //Ucitavamo teksturu
    unsigned doorTexture = loadImageToTexture("res/Open_door.png");
    unsigned sofaTexture = loadImageToTexture("res/sofa.png");
    unsigned tableTexture = loadImageToTexture("res/Table.png");
    unsigned wallTexture = loadImageToTexture("res/Wall.png");
    unsigned windowTexture = loadImageToTexture("res/Window.png");
    unsigned wcTexture = loadImageToTexture("res/wc.png");

    double mouseX, mouseY;

    Button::initializeButtonBuffers(100.0f, 100.0f, normFactorX, normFactorY);
    Button::loadShader(buttonShader);

    Button defaultButton = Button(0.9, 0.9);
    defaultButton.loadTexture(defaultTexture);
    Button bedButton = Button(0.9, 0.7, 1, 2);
    bedButton.loadTexture(bedTexture);
    Button chairButton = Button(0.9, 0.5);
    chairButton.loadTexture(chairTexture);
    Button cornerButton = Button(0.9, 0.3);
    cornerButton.loadTexture(cornerTexture);
    Button doucheButton = Button(0.9, 0.1);
    doucheButton.loadTexture(doucheTexture);
    Button doorButton = Button(0.9, -0.1);
    doorButton.loadTexture(doorTexture);
    Button sofaButton = Button(0.9, -0.3);
    sofaButton.loadTexture(sofaTexture);
    Button tableButton = Button(0.9, -0.5, 1, 2);
    tableButton.loadTexture(tableTexture);
    Button wallButton = Button(0.9, -0.7);
    wallButton.loadTexture(wallTexture);
    Button windowButton = Button(0.9, -0.9);
    windowButton.loadTexture(windowTexture);
    Button wcButton = Button(0.7, 0.9);
    wcButton.loadTexture(wcTexture);
    Button closeDoor = Button(0.7, 0.7);
    closeDoor.loadTexture(cDoorTexture);

    Tile::initializeBuffers(0.2f * 0.5625, 0.2f); // Kvadrat veličine 0.2 (NDC)
    Tile::loadShader(gridShader);

    std::vector<Grid> floors; // Vektor za čuvanje više gridova
    int currentGridIndex = -1; // Indeks trenutnog grida (-1 znači da nema gridova)

    Grid grid = Grid(8, 10, 0.2f);
    grid.setTextureToGrid(defaultTexture);

    glEnable(GL_BLEND); // nesto za alfa kanal
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); // nesto za alfa kanal
    glfwSetMouseButtonCallback(window, mouse_button_callback);

    while (!glfwWindowShouldClose(window))
    {
        double currentTime = glfwGetTime();
        double elapsedTime = currentTime - previousTime;

        if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        {
            glfwSetWindowShouldClose(window, GL_TRUE);
        }

        if (elapsedTime >= FRAME_DURATION) {


            if (selectedButton != nullptr) {
                if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) {
                    rotatePressed = false;
                }
                if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
                    if (rotatePressed == false) {
                        selectedButton->rotate(90.0f);
                        std::cout << "rotiran" << std::endl;
                        std::cout << selectedButton->getDimX() << "x" << selectedButton->getDimY() << std::endl;
                    }
                    rotatePressed = true;
                }
            }

            if (glfwGetKey(window, GLFW_KEY_N) == GLFW_RELEASE) {
                newGridPressed = false;
            }
            if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS) {
                if (newGridPressed == false) {
                    // Dodaj novi grid
                    floors.push_back(Grid(8, 10, 0.2f));
                    floors.back().setTextureToGrid(defaultTexture);
                    currentGridIndex = floors.size() - 1; // Postavi trenutni grid na novi
                    std::cout << "Novi sprat (grid) dodan! Trenutni broj spratova: " << floors.size() << std::endl;
                }
                newGridPressed = true;
            }

            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_RELEASE) {
                rightArrowPressed = false;
            }
            if (glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS) {
                if (currentGridIndex < static_cast<int>(floors.size()) - 1 && rightArrowPressed == false) {
                    currentGridIndex++;
                    std::cout << "Prešao na sledeći grid: " << currentGridIndex << std::endl;
                }
                rightArrowPressed = true;
            }

            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_RELEASE) {
                leftArrowPressed = false;
            }
            if (glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS) {
                if (currentGridIndex > 0 && leftArrowPressed == false) {
                    currentGridIndex--;
                    std::cout << "Prešao na prethodni grid: " << currentGridIndex << std::endl;
                }
                leftArrowPressed = true;
            }

            glfwGetCursorPos(window, &mouseX, &mouseY);

            // Normalizuj mouseX i mouseY na opseg od -1 do 1
            float normalizedMouseX = (2.0f * static_cast<float>(mouseX) / wWidth) - 1.0f;
            float normalizedMouseY = 1.0f - (2.0f * static_cast<float>(mouseY) / wHeight);

            if (defaultButton.onClick(window)) {
                selectedButton = &defaultButton;
            }
            else if (bedButton.onClick(window)) {
                selectedButton = &bedButton;
            }
            else if (chairButton.onClick(window)) {
                selectedButton = &chairButton;
            }
            else if (cornerButton.onClick(window)) {
                selectedButton = &cornerButton;
            }
            else if (doucheButton.onClick(window)) {
                selectedButton = &doucheButton;
            }
            else if (doorButton.onClick(window)) {
                selectedButton = &doorButton;
            }
            else if (sofaButton.onClick(window)) {
                selectedButton = &sofaButton;
            }
            else if (tableButton.onClick(window)) {
                selectedButton = &tableButton;
            }
            else if (wallButton.onClick(window)) {
                selectedButton = &wallButton;
            }
            else if (windowButton.onClick(window)) {
                selectedButton = &windowButton;
            }
            else if (wcButton.onClick(window)) {
                selectedButton = &wcButton;
            }
            else if (closeDoor.onClick(window)) {
                selectedButton = &wcButton;
            }



            int state = glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT);
            if (state == GLFW_PRESS && selectedButton != nullptr)
            {

                float dimensionX = selectedButton->getDimX();
                float dimensionY = selectedButton->getDimY();

                if (selectedButton->getDimX() == 1 && selectedButton->getDimY() == 2) {
                    //std::cout << "1x2" << std::endl;
                    floors[currentGridIndex].setTextureOnTile1x2(selectedButton->getTexture(), selectedButton->getRotationAngle(), mouseX, mouseY);
                }
                else if (selectedButton->getDimX() == 2 && selectedButton->getDimY() == 1) {
                    //std::cout << "2x1" << std::endl;
                    floors[currentGridIndex].setTextureOnTile2x1(selectedButton->getTexture(), selectedButton->getRotationAngle(), mouseX, mouseY);
                }
                else if (dimensionX == 1 && dimensionY == 1) {
                    //std::cout << "1x1" << std::endl;
                    floors[currentGridIndex].setTextureOnTile1x1(selectedButton->getTexture(), selectedButton->getRotationAngle(), mouseX, mouseY);
                }

                //grid.setTextureOnTile2x1(selectedButton->getTexture(), selectedButton->getRotationAngle(), mouseX, mouseY);

            }
        }
        
        glClearColor(0.5, 0.5, 0.5, 1.0);
        glClear(GL_COLOR_BUFFER_BIT);

        defaultButton.draw();
        bedButton.draw();
        chairButton.draw();
        cornerButton.draw();
        doucheButton.draw();
        doorButton.draw();
        sofaButton.draw();
        tableButton.draw();
        wallButton.draw();
        windowButton.draw();
        wcButton.draw();
        closeDoor.draw();

        if (currentGridIndex >= 0 && currentGridIndex < static_cast<int>(floors.size())) {
            floors[currentGridIndex].draw();
        }

        //grid.draw();

        /*GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cout << "OpenGL Error: " << err << std::endl;
        }*/
        

        glfwSwapBuffers(window);
        glfwPollEvents();
    }


    glDeleteTextures(1, &defaultTexture);
    glDeleteTextures(1, &bedTexture);
    glDeleteTextures(1, &chairTexture);
    glDeleteTextures(1, &cDoorTexture);
    glDeleteTextures(1, &cornerTexture);
    glDeleteTextures(1, &doucheTexture);
    glDeleteTextures(1, &doorTexture);
    glDeleteTextures(1, &sofaTexture);
    glDeleteTextures(1, &tableTexture);
    glDeleteTextures(1, &wallTexture);
    glDeleteTextures(1, &windowTexture);


    glDeleteProgram(buttonShader);
    glDeleteProgram(gridShader);

    glfwTerminate();
    return 0;
}

unsigned int compileShader(GLenum type, const char* source)
{
    std::string content = "";
    std::ifstream file(source);
    std::stringstream ss;
    if (file.is_open())
    {
        ss << file.rdbuf();
        file.close();
        std::cout << "Uspjesno procitao fajl sa putanje \"" << source << "\"!" << std::endl;
    }
    else {
        ss << "";
        std::cout << "Greska pri citanju fajla sa putanje \"" << source << "\"!" << std::endl;
    }
     std::string temp = ss.str();
     const char* sourceCode = temp.c_str();

    int shader = glCreateShader(type);
    
    int success;
    char infoLog[512];
    glShaderSource(shader, 1, &sourceCode, NULL);
    glCompileShader(shader);

    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        if (type == GL_VERTEX_SHADER)
            printf("VERTEX");
        else if (type == GL_FRAGMENT_SHADER)
            printf("FRAGMENT");
        printf(" sejder ima gresku! Greska: \n");
        printf(infoLog);
    }
    return shader;
}
unsigned int createShader(const char* vsSource, const char* fsSource)
{
    
    unsigned int program;
    unsigned int vertexShader;
    unsigned int fragmentShader;

    program = glCreateProgram();

    vertexShader = compileShader(GL_VERTEX_SHADER, vsSource);
    fragmentShader = compileShader(GL_FRAGMENT_SHADER, fsSource);

    
    glAttachShader(program, vertexShader);
    glAttachShader(program, fragmentShader);

    glLinkProgram(program);
    glValidateProgram(program);

    int success;
    char infoLog[512];
    glGetProgramiv(program, GL_VALIDATE_STATUS, &success);
    if (success == GL_FALSE)
    {
        glGetShaderInfoLog(program, 512, NULL, infoLog);
        std::cout << "Objedinjeni sejder ima gresku! Greska: \n";
        std::cout << infoLog << std::endl;
    }

    glDetachShader(program, vertexShader);
    glDeleteShader(vertexShader);
    glDetachShader(program, fragmentShader);
    glDeleteShader(fragmentShader);

    return program;
}
static unsigned loadImageToTexture(const char* filePath) {
    int TextureWidth;
    int TextureHeight;
    int TextureChannels;
    unsigned char* ImageData = stbi_load(filePath, &TextureWidth, &TextureHeight, &TextureChannels, 0);
    if (ImageData != NULL)
    {
        //Slike se osnovno ucitavaju naopako pa se moraju ispraviti da budu uspravne
        stbi__vertical_flip(ImageData, TextureWidth, TextureHeight, TextureChannels);

        // Provjerava koji je format boja ucitane slike
        GLint InternalFormat = -1;
        switch (TextureChannels) {
        case 1: InternalFormat = GL_RED; break;
        case 2: InternalFormat = GL_RG; break;
        case 3: InternalFormat = GL_RGB; break;
        case 4: InternalFormat = GL_RGBA; break;
        default: InternalFormat = GL_RGB; break;
        }

        unsigned int Texture;
        glGenTextures(1, &Texture);
        glBindTexture(GL_TEXTURE_2D, Texture);
        glTexImage2D(GL_TEXTURE_2D, 0, InternalFormat, TextureWidth, TextureHeight, 0, InternalFormat, GL_UNSIGNED_BYTE, ImageData);
        glBindTexture(GL_TEXTURE_2D, 0);
        // oslobadjanje memorije zauzete sa stbi_load posto vise nije potrebna
        stbi_image_free(ImageData);
        return Texture;
    }
    else
    {
        std::cout << "Textura nije ucitana! Putanja texture: " << filePath << std::endl;
        stbi_image_free(ImageData);
        return 0;
    }
}