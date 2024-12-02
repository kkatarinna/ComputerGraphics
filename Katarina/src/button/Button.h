#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>


class Button
{
    private:
        float x, y;                         // Pozicija dugmeta
        float rotationAngle;
        bool isRReleased;                   //Da li je R dugme otpusteno
        static float width, height;        // Dimenzije dugmeta
        static float normFactorX, normFactorY; // Normalizacioni faktori za veli?inu
        unsigned int textureID;     // ID teksture
        static unsigned int VAO, VBO;      // OpenGL objekti za crtanje
        static unsigned int shaderProgram; // Shader program
        int dimensionX, dimensionY;

        // Metoda za ažuriranje vertiksa u OpenGL VBO

    public:
        // Constructor
        Button(float startX = 0, float startY = 0, int dimX = 1, int dimY = 1);
        void loadTexture(unsigned int texture);
        bool isHovered(float mouseX, float mouseY);
        bool onClick(GLFWwindow* window);
        void rotate(float angle);
        void onRRelease(GLFWwindow* window);
        unsigned int getTexture() const;
        static void initializeButtonBuffers(float w, float h, float nfX, float nfY);
        static void loadShader(unsigned int shader);
        // Destruktor
        ~Button();

        // Metode za crtanje
        void draw();
        float getRotationAngle();
        float getDimX();
        float getDimY();
};

