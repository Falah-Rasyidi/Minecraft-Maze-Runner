#ifndef GENERATE_MAZE_H
#define GENERATE_MAZE_H

#include <cstring>

#include "menuUtils.h"
#include "Maze.h"

// Reads a maze from the user via the terminal. Stores the new state and maze into curState and outputMaze respectively.
States mazeTerminal(States &curState);

// Generates a random maze
States mazeRandom(States &curState);
States enhancedMazeRandom(States &curState);
States testRandom(States &curState);

// Helper functions for both
void printMazeInfo(const Maze& mazeIn);
void setBasePoint(int& x, int& y, int& z);
void setDimensions(int& xLen, int& zLen);
bool isWithinBounds(const Maze& mazeIn, const int offsetX, const int offsetZ);
bool isWithinBounds(const std::vector<std::vector<char>>& randMaze, const int offsetX, const int offsetZ);
bool** createVisitMaze(const int xLen, const int zLen);
void deleteVisitMaze(bool** visited, const int xLen);

// Helper functions for userMaze
void setMazeStructure(Maze& mazeIn);
bool validExits(const Maze& mazeIn);
bool hasCycle(const Maze& mazeIn);
bool dfs(const Maze maze, const int currX, const int currZ, const int prevX, const int prevZ, bool** visited);
bool hasPath(const Maze& mazeIn);
bool visitNode(const Maze maze, const int currX, const int currZ, const int prevX, const int prevZ, bool** visited);
bool isExit(const Maze maze, const int currX, const int currZ);
bool isNotParent(const int childX, const int parentX, const int childZ, const int parentZ);
bool checkAdjacentExit(const Maze maze, const int currX, const int currZ);

// Helper functions for randomMaze
int chooseStartingPoint(const int length);
bool visitNeighbours(std::vector<std::vector<char>>& randMaze, const int x, const int z, const int prevX, const int prevZ, bool** visited, bool testMode);
bool enhancedVisitNeighbours(const mcpp::Coordinate& basePoint, std::vector<std::vector<char>>& randMaze, const int x, const int z, const int prevX, const int prevZ, bool** visited);
void shuffleDirections(int directionsX[], int directionsZ[]);
bool isNeighbouringNodes(const std::vector<std::vector<char>>& randMaze, const int x, const int z, bool** visited);
void makeWay(std::vector<std::vector<char>>& randMaze, const int offsetX, const int offsetZ, const int dirX, const int dirZ);
bool tryMakeWay(const mcpp::Coordinate& basePoint, std::vector<std::vector<char>>& randMaze, const int offsetX, const int offsetZ, const int dirX, const int dirZ);

#endif