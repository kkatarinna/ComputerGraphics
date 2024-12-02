#include "Grid.h"

//Grid::Grid(unsigned int rows, unsigned int cols, float size)
//    : rows(rows), cols(cols) {
//    // Kreiranje grida
//    for (unsigned int i = 0; i < rows; ++i) {
//        std::vector<Tile*> row;
//        for (unsigned int j = 0; j < cols; ++j) {
//            // Inicijalizacija svakog tile-a sa početnim pozicijama            
//            row.push_back(new Tile(j * size * 0.5625, i * size)); //Dodat je scale factor da zna na kojoj koordinati x da ga iscrtava
//        }
//        grid.push_back(row);
//    }
//}

Grid::Grid(unsigned int rows, unsigned int cols, float size)
    : rows(rows), cols(cols) {
    // Izračunaj polovine širine i visine grida
    float halfWidth = (cols * size * 0.5625) / 2.0f; // Širina grida podeljena sa 2
    float halfHeight = (rows * size) / 2.0f;         // Visina grida podeljena sa 2

    // Kreiranje grida
    for (unsigned int i = 0; i < rows; ++i) {
        std::vector<Tile*> row;
        for (unsigned int j = 0; j < cols; ++j) {
            // Inicijalizacija svakog tile-a sa centriranim pozicijama            
            row.push_back(new Tile(j * size * 0.5625 - halfWidth, i * size - halfHeight));
        }
        grid.push_back(row);
    }
}


Tile* Grid::getTile(unsigned int row, unsigned int col) {
    //return grid[row][col];
    if (row < rows && col < cols) {
        return grid[row][col];  // Vraća pokazivač na odgovarajući Tile
    }
    return nullptr;
}

Tile* Grid::getTileOnClick(GLFWwindow* window) {
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    // Pretvaranje miša u NDC koordinate
    mouseY = 1080 - mouseY;  // Pretpostavljam da je vertikalna veličina okvira 600

    // Prolazimo kroz grid i proveravamo koji je tile kliknut
    for (auto row = grid.begin(); row != grid.end(); ++row) {
        for (auto tile = row->begin(); tile != row->end(); ++tile) {

            if ((*tile)->onClick(window)) {  // Pozivanje isClicked() za svaki Tile objekat
                return *tile;
            }
        }
    }
    return nullptr;
}

void Grid::setTextureOnTile1x1(unsigned int texture, float rotation, double mouseX, double mouseY) {
    // Prolazimo kroz grid i proveravamo koji je tile kliknut
    for (auto row = grid.begin(); row != grid.end(); ++row) {
        for (auto tile = row->begin(); tile != row->end(); ++tile) {

            if ((*tile)->isClicked(mouseX, mouseY)) {  // Pozivanje isClicked() za svaki Tile objekat
                (*tile)->setRotationAngle(rotation);
                (*tile)->setUandV(0.0f, 1.0f);
                (*tile)->setTexture(texture);
            }
        }
    }
}

void Grid::setTextureOnTile1x2(unsigned int texture, float rotation, double mouseX, double mouseY) {
    // Prolazimo kroz grid i proveravamo koji je tile kliknut
    for (auto row = grid.begin(); row != grid.end(); ++row) {
        for (auto tile = row->begin(); tile != row->end(); ++tile) {

            if ((*tile)->isClicked(mouseX, mouseY)) {  // Pozivanje isClicked() za svaki Tile objekat
                // Proveravamo da li postoji sledeći tile unutar trenutnog reda
                auto nextTile = std::next(tile);
                if (nextTile == row->end()) {
                    // Ako sledeći tile ne postoji, ne radimo ništa
                    //std::cout << "Ne moze se iscrtati zbog neodgovarajucih dimenzija" << std::endl;
                    return;
                }

                // Primenjujemo rotaciju i teksturu na trenutni i sledeći tile
                (*tile)->setRotationAngle(rotation);
                (*nextTile)->setRotationAngle(rotation);
                if (rotation == 180) {
                    (*tile)->setUandV(0.5f, 1.0f);
                    (*nextTile)->setUandV(0.0f, 0.5f);
                }
                else {
                    (*tile)->setUandV(0.0f, 0.5f);
                    (*nextTile)->setUandV(0.5f, 1.0f);
                }
                (*tile)->setTexture(texture);
                (*nextTile)->setTexture(texture);
            }
        }
    }
}

void Grid::setTextureOnTile2x1(unsigned int texture, float rotation, double mouseX, double mouseY) {
    // Prolazimo kroz grid i proveravamo koji je tile kliknut
    for (auto row = grid.begin(); row != grid.end(); ++row) {
        for (auto tile = row->begin(); tile != row->end(); ++tile) {

            if ((*tile)->isClicked(mouseX, mouseY)) {  // Pozivanje isClicked() za svaki Tile objekat
                // Prvo proveravamo da li postoji sledeći red
                auto nextRow = std::next(row);
                if (nextRow == grid.end()) {
                    // Ako sledeći red ne postoji, ne radimo ništa
                    //std::cout << "Ne moze se iscrtati zbog neodgovarajucih dimenzija" << std::endl;
                    return;
                }

                // Proveravamo da li u sledećem redu postoji tile na istoj poziciji
                size_t tileIndex = std::distance(row->begin(), tile);
                if (tileIndex >= nextRow->size()) {
                    // Ako nema tile-a ispod, ne radimo ništa
                    return;
                }

                // Primenjujemo rotaciju i teksturu na trenutni i donji tile
                auto belowTile = std::next(nextRow->begin(), tileIndex);
                (*tile)->setRotationAngle(rotation);
                (*belowTile)->setRotationAngle(rotation);

                if (rotation == 270) {
                    (*tile)->setUandV(0.5f, 1.0f);
                    (*belowTile)->setUandV(0.0f, 0.5f);
                }
                else if (rotation == 90){
                    (*tile)->setUandV(0.0f, 0.5f);
                    (*belowTile)->setUandV(0.5f, 1.0f);
                }

                (*tile)->setTexture(texture);
                (*belowTile)->setTexture(texture);
            }
        }
    }
}



void Grid::draw() {
    // Iteracija kroz sve redove
    for (auto row = grid.begin(); row != grid.end(); ++row) {
        // Iteracija kroz sve tile-ove u svakom redu
        for (auto tile = row->begin(); tile != row->end(); ++tile) {
            float u = (*tile)->getU();
            float v = (*tile)->getV();
            (*tile)->draw(u, v);  // Pozivanje draw() za svaki Tile objekat
        }
    }
}


bool Grid::onClick(GLFWwindow* window) {
    double mouseX, mouseY;
    glfwGetCursorPos(window, &mouseX, &mouseY);

    // Pretvaranje miša u NDC koordinate
    mouseY = 1080 - mouseY;  // Pretpostavljam da je vertikalna veličina okvira 1080

    // Prolazimo kroz grid i proveravamo koji je tile kliknut
    for (auto row = grid.begin(); row != grid.end(); ++row) {
        for (auto tile = row->begin(); tile != row->end(); ++tile) {

            if ((*tile)->onClick(window)) {  // Pozivanje isClicked() za svaki Tile objekat
                return true;
            }
        }
    }
    return false;
}


void Grid::setTextureToGrid(unsigned int textureID) {
    for (auto row = grid.begin(); row != grid.end(); ++row) {
        // Iteracija kroz sve tile-ove u svakom redu
        for (auto tile = row->begin(); tile != row->end(); ++tile) {
            (*tile)->loadTexture(textureID);  // Pozivanje draw() za svaki Tile objekat
        }
    }
}
