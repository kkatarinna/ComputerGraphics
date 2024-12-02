#pragma once

#include "Tile.h"
#include <vector>

class Grid {
private:
    std::vector<std::vector<Tile*>> grid; // Matrica tile-ova
    unsigned int rows, cols;             // Broj redova i kolona

public:
    // Konstruktor
    Grid(unsigned int rows, unsigned int cols, float size);

    // Metoda za pristup pojedinačnom tile-u
    Tile* getTile(unsigned int row, unsigned int col);

    // Metoda za crtanje celog grida
    void draw();

    // Metoda za detektovanje klika na neki tile
    bool onClick(GLFWwindow* window);

    // Metoda za postavljanje teksture za ceo grid
    void setTextureToGrid(unsigned int textureID);

    Tile* getTileOnClick(GLFWwindow* window);

    void setTextureOnTile1x1(unsigned int texture, float rotation, double mouseX, double mouseY);

    void setTextureOnTile1x2(unsigned int texture, float rotation, double mouseX, double mouseY);

    void setTextureOnTile2x1(unsigned int texture, float rotation, double mouseX, double mouseY);

};
