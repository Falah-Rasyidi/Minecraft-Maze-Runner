#include "Maze.h"
#include <random>

Maze::Maze() {
    this->basePoint = mcpp::Coordinate(0, 0, 0);
    this->xLen = 0;
    this->zLen = 0;
    this->mode = 0;
}

Maze::Maze(mcpp::Coordinate basePoint, unsigned int xLen, unsigned int zLen, bool mode) {
    this->basePoint = basePoint;
    this->xLen = xLen;
    this->zLen = zLen;
    this->mode = mode;

    this->mazeStructure.resize(xLen, std::vector<char>(zLen, '.'));
}
                              
Maze::~Maze() {
    // std::cout << std::endl;
    // std::cout << "Maze has been deleted\n";
}

// Setters
void Maze::setMazeStructure(std::vector<std::vector<char>> mazeStructure) {
    this->mazeStructure = mazeStructure;
}

// Getters
std::vector<std::vector<char>> Maze::getMazeStructure() const {
    return this->mazeStructure;
}

char Maze::getCell(const int x, const int z) const{
    return this->mazeStructure.at(x).at(z);
}

bool Maze::empty() {
    return this->mazeStructure.empty();
}

mcpp::Coordinate Maze::getBasePoint() const { 
    return this->basePoint;
}

int Maze::getXLen() const {
    return this->xLen;
}

int Maze::getZLen() const {
    return this->zLen;
}

bool Maze::getMode() const {
    return this->mode;
}

// Printing
void Maze::printInfo() const {
    std::cout << "**Printing Maze**\n";
    std::cout << "BasePoint: (" << this->basePoint.x << ", " << this->basePoint.y << ", " << this->basePoint.z << ")\n";

    std::cout << "Structure:\n";
    for (int i = 0; i < this->xLen; i++) {
        for (int j = 0; j < this->zLen; j++) {
            std::cout << this->mazeStructure[i][j];
        }
        std::cout << "\n";
    }

    std::cout << "**End Printing Maze**\n";
}