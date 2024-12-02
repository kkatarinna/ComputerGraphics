#include "Tile.h"

// Statičke promenljive
float Tile::with = 0;
float Tile::height = 0;
unsigned int Tile::VAO = -1;
unsigned int Tile::VBO = -1;
unsigned int Tile::shaderProgram = -1;

// Konstruktor
Tile::Tile(float startX, float startY) : x(startX), y(startY), textureID(0), rotationAngle(0), u(0.0f), v(1.0f) {}

// Inicijalizacija OpenGL bafera
void Tile::initializeBuffers(float w, float h) {

    with = w;
    height = h;

    float halfWidth = with / 2.0f;
    float halfHeight = height / 2.0f;

    // Koordinate vertiksa (pozicija i tekstura)
    float vertices[] = {
        -halfWidth, -halfHeight, 0.0f, 0.0f,  // Donji levi ugao
         halfWidth, -halfHeight, 1.0f, 0.0f,  // Donji desni ugao
        -halfWidth,  halfHeight, 0.0f, 1.0f,  // Gornji levi ugao
         halfWidth,  halfHeight, 1.0f, 1.0f   // Gornji desni ugao
    };

    unsigned int stride = (2 + 2) * sizeof(float);

    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, stride, (void*)0);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, stride, (void*)(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

// Učitavanje teksture
void Tile::loadTexture(unsigned int texture) {
    textureID = texture;

    glBindTexture(GL_TEXTURE_2D, texture);

    glGenerateMipmap(GL_TEXTURE_2D);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    glBindTexture(GL_TEXTURE_2D, 0);
}

// Učitavanje shadera
void Tile::loadShader(unsigned int shader) {
    shaderProgram = shader;
    glUseProgram(shaderProgram);
    unsigned int uTexLoc = glGetUniformLocation(shaderProgram, "uTex");
    glUniform1i(uTexLoc, 0);
    glUseProgram(0);
}

// Destruktor
Tile::~Tile() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}

// Crtanje kvadrata
void Tile::draw(float u, float v) {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textureID);
    // Postavljanje uniform vrednosti za poziciju
    glUniform2f(glGetUniformLocation(shaderProgram, "uPos"), x, y); //Ovo je scale factor 0.5625 = visina/sirina ekrana
    glUniform1f(glGetUniformLocation(shaderProgram, "uRotation"), rotationAngle);
    glUniform2f(glGetUniformLocation(shaderProgram, "uTexOffset"), u, v);


    // Crtanje kvadrata
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    //std::cout << "Drawing Tile at position (" << x << ", " << y << ") with texture ID " << textureID << std::endl;

    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}

void Tile::setRotationAngle(float ra)
{
    rotationAngle = ra;
}

float Tile::getRotationAngle()
{
    return rotationAngle;
}

bool Tile::isClicked(float mouseX, float mouseY) {
    unsigned int wWidth = 1920;
    unsigned int wHeight = 1080;
    float normalizedMouseX = ((2.0f * static_cast<float>(mouseX) / wWidth) - 1.0f);
    float normalizedMouseY = 1.0f - (2.0f * static_cast<float>(mouseY) / wHeight);
    float halfWidth = with / 2.0f;
    float halfHeight = height / 2.0f;
    return normalizedMouseX >= x - halfWidth && normalizedMouseX <= x + halfWidth &&
        normalizedMouseY >= y - halfHeight && normalizedMouseY <= y + halfHeight;
}

bool Tile::onClick(GLFWwindow* window) {
    unsigned int wWidth = 1920;
    unsigned int wHeight = 1080;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        // Dohvatanje trenutne pozicije miša
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);
        std::cout << "Mouse X: " << mouseX << ", Mouse Y: " << mouseY << std::endl;

        // Pretvaranje iz piksel koordinata u OpenGL koordinate
        // Normalizuj mouseX i mouseY na opseg od -1 do 1
        float normalizedMouseX = ((2.0f * static_cast<float>(mouseX) / wWidth) - 1.0f);
        float normalizedMouseY = 1.0f - (2.0f * static_cast<float>(mouseY) / wHeight);
        std::cout << "Normalized Mouse X: " << normalizedMouseX << ", Normalized Mouse Y: " << normalizedMouseY << std::endl;


        // Provera da li su koordinate unutar granica dugmeta
        float halfWidth = with / 2.0f;
        float halfHeight = height / 2.0f;
        return normalizedMouseX >= x - halfWidth && normalizedMouseX <= x + halfWidth &&
            normalizedMouseY >= y - halfHeight && normalizedMouseY <= y + halfHeight;
    }

    return false;
}

void Tile::setTexture(unsigned int newTexture) {
    //std::cout << "promenjena tekstura: " << newTexture << std::endl;
    textureID = newTexture; // Postavi novu teksturu
}

Tile::Tile(const Tile& other)
    : x(other.x), y(other.y), textureID(other.textureID), rotationAngle(other.rotationAngle) {
    // VAO, VBO i shaderProgram su statički, pa ih nije potrebno kopirati
}

Tile& Tile::operator=(const Tile& other) {
    if (this != &other) {
        x = other.x;
        y = other.y;
        textureID = other.textureID;
        rotationAngle = other.rotationAngle;
        // VAO, VBO i shaderProgram ostaju isti jer su statički
    }
    return *this;
}

void Tile::setUandV(float uCoor, float vCoor) {
    u = uCoor;
    v = vCoor;
}

float Tile::getU() {
    return u;
}

float Tile::getV() {
    return v;
}
