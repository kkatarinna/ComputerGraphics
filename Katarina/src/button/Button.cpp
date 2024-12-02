#include "Button.h"

float Button::width = 0;
float Button::height = 0;
float Button::normFactorX = 0;
float Button::normFactorY = 0;
unsigned int Button::VAO = -1;
unsigned int Button::VBO = -1;
unsigned int Button::shaderProgram = -1;

Button::Button(float startX, float startY, int dimX, int dimY) : rotationAngle(0.0f), isRReleased(false)
{
    x = startX;
    y = startY;
    dimensionX = dimX;
    dimensionY = dimY;
}

void Button::initializeButtonBuffers(float w, float h, float nfX, float nfY)
{
    normFactorX = nfX;
    normFactorY = nfY;
    width = w * normFactorX;
    height = h * normFactorY;
    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;

    // Računanje koordinata na osnovu veličine projektila
    float vertices[] = {
        -halfWidth, -halfHeight, 0.0f, 0.0f,   // Donji levi ugao
        halfWidth, -halfHeight, 1.0f, 0.0f,   // Donji desni ugao
        -halfWidth,  halfHeight, 0.0f, 1.0f,   // Gornji levi ugao
        halfWidth,  halfHeight, 1.0f, 1.0f    // Gornji desni ugao
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

void Button::loadTexture(unsigned int texture)
{
    textureID = texture;

    // Poveži teksturu sa OpenGL-om
    glBindTexture(GL_TEXTURE_2D, texture);

    // Generiši mip-mape
    glGenerateMipmap(GL_TEXTURE_2D);

    // Podesi parametre teksture
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    // Očisti bind
    glBindTexture(GL_TEXTURE_2D, 0);

}

void Button::loadShader(unsigned int shader)
{
    shaderProgram = shader;
    glUseProgram(shaderProgram);
    unsigned int uTexLoc = glGetUniformLocation(shaderProgram, "uTex");
    glUniform1i(uTexLoc, 0);  // Učitavanje teksture u shader
    glUseProgram(0);
}

// Destruktor
Button::~Button() {
    glDeleteBuffers(1, &VBO);
    glDeleteVertexArrays(1, &VAO);
}


// Crta igrača na ekranu
void Button::draw() {
    glUseProgram(shaderProgram);   // Aktiviraj shader
    glBindVertexArray(VAO);        // Binduj VAO

    glActiveTexture(GL_TEXTURE0);  // Aktiviraj teksturnu jedinicu
    glBindTexture(GL_TEXTURE_2D, textureID); // Binduj teksturu Koristi teksturu specifičnu za instancu

    // Postavljanje uniform vrednosti
    glUniform2f(glGetUniformLocation(shaderProgram, "uPos"), x, y);

    // Postavljanje uniform vrednosti za rotaciju
    glUniform1f(glGetUniformLocation(shaderProgram, "uRotation"), rotationAngle);

    // Crtanje pravougaonika
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

    // Resetuj stanje
    glBindTexture(GL_TEXTURE_2D, 0);
    glBindVertexArray(0);
    glUseProgram(0);
}
float Button::getRotationAngle()
{
    return rotationAngle;
}


bool Button::isHovered(float mouseX, float mouseY) 
{
    float halfWidth = width / 2.0f;
    float halfHeight = height / 2.0f;
    return mouseX >= x - halfWidth && mouseX <= x + halfWidth &&
        mouseY >= y - halfHeight && mouseY <= y + halfHeight;
}

bool Button::onClick(GLFWwindow* window) 
{
    unsigned int wWidth = 1920;
    unsigned int wHeight = 1080;
    if (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS) {
        // Dohvatanje trenutne pozicije miša
        double mouseX, mouseY;
        glfwGetCursorPos(window, &mouseX, &mouseY);

        // Pretvaranje iz piksel koordinata u OpenGL koordinate
        // Normalizuj mouseX i mouseY na opseg od -1 do 1
        float normalizedMouseX = (2.0f * static_cast<float>(mouseX) / wWidth) - 1.0f;
        float normalizedMouseY = 1.0f - (2.0f * static_cast<float>(mouseY) / wHeight);

        // Provera da li su koordinate unutar granica dugmeta
        float halfWidth = width / 2.0f;
        float halfHeight = height / 2.0f;

        return normalizedMouseX >= x - halfWidth && normalizedMouseX <= x + halfWidth &&
            normalizedMouseY >= y - halfHeight && normalizedMouseY <= y + halfHeight;
    }

    return false;
}

unsigned int Button::getTexture() const 
{
    return textureID;
}

void Button::rotate(float angle) 
{
    rotationAngle += angle; // Povećajte ugao za zadatu vrednost
    if (rotationAngle >= 360.0f) {
        rotationAngle -= 360.0f; // Održavajte ugao u opsegu [0, 360)
    }  
    std::swap(dimensionX, dimensionY);
}

void Button::onRRelease(GLFWwindow* window) {

    bool oldVal = isRReleased;
    // Ako je taster R pritisnut i sada je otpušten, izvrši rotaciju
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE) {
        //rotate(90.0f);  // Rotiraj za 90 stepeni
        isRReleased = false;  // Obeleži da je taster otpušten
    }
    // Ako je taster ponovo pritisnut, obeležavamo da je R opet pritisnut
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) {
        isRReleased = true;  // Spremni smo za sledeće otpuštanje
    }

    if (isRReleased != oldVal) {
        rotate(90.0f); 
    }
}

float Button::getDimX() {
    return dimensionX;
}

float Button::getDimY() {
    return dimensionY;
}