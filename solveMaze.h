#ifndef SOLVE_MAZE_H
#define SOLVE_MAZE_H

#include <queue>

#include "menuUtils.h"
#include "Agent.h"

// Teleports the player into a random location within the maze.
States manualSolve(States &curState);

bool insideMaze(const Agent &agent);

States escapeRoute(States &curState);

States shortestPath(States &curState);

// Linked list for BFS algorithm
struct MazeNode {
    // Default constructor for head node
    MazeNode() : location(mcpp::Coordinate(0, 0, 0)), adjacentEdges(0) {
        parentNode = nullptr;
    }

    MazeNode(mcpp::Coordinate tileLoc) : location(tileLoc), adjacentEdges(0) {
        parentNode = nullptr;
    }

    ~MazeNode() {
    }

    void insertAfter(MazeNode* node) {
        MazeNode* tempNext = this->nextNode;
        this->nextNode = node;
        node->nextNode = tempNext;
    }

    MazeNode* getNext() {
        return this->nextNode;
    }

    // Node data
    mcpp::Coordinate location;
    std::vector<mcpp::Coordinate> adjacentEdges;  // Stores valid adjacent tiles
    MazeNode* parentNode;  // Successive parent nodes from end to start create shortest path

    // Vanilla linked list stuff
    MazeNode* nextNode;
};

std::vector<mcpp::Coordinate> validTiles(mcpp::Coordinate currentTile, const std::vector<MazeNode*> exploredNodes);

bool explored(mcpp::Coordinate currentTile, const std::vector<MazeNode*> exploredNodes);

bool exitedMaze(MazeNode currentTile);

#endif