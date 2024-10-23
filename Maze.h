#ifndef ASSIGN_MAZE_H
#define ASSIGN_MAZE_H

#include <vector>
#include <mcpp/mcpp.h>

// This records the data of the maze
class Maze
{
public:
    Maze();
    Maze(mcpp::Coordinate basePoint, unsigned int xLen, unsigned int zLen, bool mode);
    ~Maze();

    // setters
    void setMazeStructure(std::vector<std::vector<char>> mazeStructure);

    // getters
    std::vector<std::vector<char>> getMazeStructure() const;
    char getCell(const int x, const int z) const;
    bool empty();
    mcpp::Coordinate getBasePoint() const;
    int getXLen() const;
    int getZLen() const;
    bool getMode() const;

    // Print info
    void printInfo() const;
    
private:
    std::vector<std::vector<char>> mazeStructure;
    mcpp::Coordinate basePoint;
    int xLen;
    int zLen;
    bool mode;
};

#endif //ASSIGN_MAZE_H
