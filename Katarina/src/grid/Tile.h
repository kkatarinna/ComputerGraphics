#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>

class Tile {
private:
    float x, y;                          // Pozicija kvadrata (NDC koordinate)
    static float with;                   // Dimenzija kvadrata (NDC)
    static float height;
    unsigned int textureID;              // ID teksture
    static unsigned int VAO, VBO;        // OpenGL objekti za crtanje
    static unsigned int shaderProgram;   // Shader program
    float rotationAngle;
    float u, v;                          // U i V koordinate teksture

public:
    // Konstruktor
    Tile(float startX = 0, float startY = 0);
    Tile(const Tile& other);

    Tile& operator=(const Tile& other);

    // Statičke metode za inicijalizaciju i postavljanje
    static void initializeBuffers(float w, float h);
    static void loadShader(unsigned int shader);
    void loadTexture(unsigned int texture);
    bool isClicked(float mouseX, float mouseY);
    void setTexture(unsigned int newTexture);
    bool onClick(GLFWwindow* window);

    // Destruktor
    ~Tile();

    // Metoda za crtanje
    void draw(float u, float v);

    void setRotationAngle(float rotationAngle);
    float getRotationAngle();

    void setUandV(float u, float v);
    float getU();
    float getV();
};
