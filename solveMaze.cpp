#include <iostream>
#include <chrono>
#include <thread>

#include "solveMaze.h"


// Solves te
States manualSolve(States &curState) {
    std::vector<mcpp::Coordinate> openCoords(0);

    const int xLen = mazeInfo.getXLen();
    const int zLen = mazeInfo.getZLen();

    bool invalidMaze = xLen <= 0 || zLen <= 0;

    if (mazeInfo.empty() || invalidMaze) {
        std::cout << std::endl;
        std::cout << "Maze Error: Could not find valid air block in current maze ....\n";
    }
    else {
        // Iterate through the maze.
        // Find all air blocks
        for (int i = 0; i < xLen; i++) {
            for (int j = 0; j < zLen; j++) {
                mcpp::Coordinate basePoint = mazeInfo.getBasePoint();

                mcpp::Coordinate currCoordinate(basePoint.x + i, basePoint.y, basePoint.z + j);

                mcpp::BlockType currBlock = mc.getBlock(currCoordinate);

                if (currBlock == mcpp::Blocks::AIR) {
                    openCoords.push_back(currCoordinate);
                }
            }
        }
        
        // TODO: Should we set the seed here?
        // Get a random element from the vector
        int randomIndex = rand() % openCoords.size();
        mcpp::Coordinate coordToTeleport = openCoords.at(randomIndex);

        // Teleport player
        mc.setPlayerPosition(coordToTeleport);
    }

    curState = ST_SolveMaze;
    return curState;
}

bool insideMaze(const Agent& agent) {
  bool insideMaze = false;
  
  // Get coordinates to check
  mcpp::Coordinate forward = agent.currentLoc + agent.forward();
  mcpp::Coordinate right = agent.currentLoc + agent.right();
  mcpp::Coordinate left = agent.currentLoc - agent.right();
  mcpp::Coordinate forwardRight = agent.currentLoc + agent.forward() + agent.right();
  mcpp::Coordinate forwardLeft = agent.currentLoc + agent.forward() - agent.right();

  // Check forward
  if (mc.getBlock(forward) == mcpp::Blocks::ACACIA_WOOD_PLANK) insideMaze = true;
  // Check right
  else if (mc.getBlock(right) == mcpp::Blocks::ACACIA_WOOD_PLANK) insideMaze = true;
  // Check left
  else if (mc.getBlock(left) == mcpp::Blocks::ACACIA_WOOD_PLANK) insideMaze = true;
  // Check top right (forward + right)
  else if (mc.getBlock(forwardRight) == mcpp::Blocks::ACACIA_WOOD_PLANK) insideMaze = true;
  //Check top left (forward + left)
  else if (mc.getBlock(forwardLeft) == mcpp::Blocks::ACACIA_WOOD_PLANK) insideMaze = true;

  return insideMaze;
}


States escapeRoute(States &curState) {
    mcpp::Coordinate playerPos = mc.getPlayerPosition();
    Agent mazeAgent = Agent(playerPos);

    int step = 1;

    if (!insideMaze(mazeAgent)) {
        std::cout << std::endl;
        std::cout << "Maze Error: Player doesn't appear to be in the maze ....\n";
    }

    while (insideMaze(mazeAgent)) {
        mcpp::BlockType rightBlock = mc.getBlock(mazeAgent.currentLoc + mazeAgent.right());
        mcpp::BlockType forwardBlock = mc.getBlock(mazeAgent.currentLoc + mazeAgent.forward());

        // Air block to the right
        if (rightBlock == mcpp::Blocks::AIR) {
            mazeAgent.rotateClockwise();
            // Move forwards
            mazeAgent.currentLoc = mazeAgent.currentLoc + mazeAgent.forward();

            std::cout << "Step[" << step << "]: (" << mazeAgent.currentLoc.x << ", " << mazeAgent.currentLoc.y << ", " << mazeAgent.currentLoc.z << ")\n";
            mc.setBlock(mazeAgent.currentLoc, mcpp::Blocks::LIME_CARPET);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            mc.setBlock(mazeAgent.currentLoc, mcpp::Blocks::AIR);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));

            step++;
        }
        // Non-air block forward
        else if (forwardBlock == mcpp::Blocks::ACACIA_WOOD_PLANK) {
            mazeAgent.rotateAnticlockwise();
            // Don't move forward! We don't know if we can in the first place
        }
        // Else we can't turn right and we can go forward, so just go forward
        else {
            mazeAgent.currentLoc = mazeAgent.currentLoc + mazeAgent.forward();

            std::cout << "Step[" << step << "]: (" << mazeAgent.currentLoc.x << ", " << mazeAgent.currentLoc.y << ", " << mazeAgent.currentLoc.z << ")\n";
            mc.setBlock(mazeAgent.currentLoc, mcpp::Blocks::LIME_CARPET);
            std::this_thread::sleep_for(std::chrono::milliseconds(1000));
            mc.setBlock(mazeAgent.currentLoc, mcpp::Blocks::AIR);
            std::this_thread::sleep_for(std::chrono::milliseconds(200));

            step++;
        }
    }

    mc.setBlock(mazeAgent.currentLoc, mcpp::Blocks::LIME_CARPET);
    std::this_thread::sleep_for(std::chrono::milliseconds(1000));
    mc.setBlock(mazeAgent.currentLoc, mcpp::Blocks::AIR);
    std::this_thread::sleep_for(std::chrono::milliseconds(200));

    curState = ST_SolveMaze;
    
    return curState;
}

/**
 * Pseudocode from https://en.wikipedia.org/wiki/Breadth-first_search
 * G = graph (maze)
 * procedure BFS(G, root) is
 *   let Q be a queue
 *   label root as explored
 *   Q.enqueue(root)
 *   while Q is not empty do
 *     v := Q.dequeue()
 *     if v is the goal then
 *       return v
 *     for all edges from v to w in G.adjacentEdges(v) do
 *       if w is not labeled as explored then
 *         label w as explored
 *         w.parent := v
 *         Q.enqueue(w)
*/

States shortestPath(States &curState) {
    std::vector<MazeNode*> exploredTiles;
    std::vector<MazeNode*> shortestPath;
     
    std::queue<MazeNode*> MazeQueue;

    mcpp::Coordinate startLocation = mc.getPlayerPosition();

    MazeNode* startingTile = new MazeNode(startLocation);
    exploredTiles.push_back(startingTile);

    if (exitedMaze(*startingTile)) {
        std::cout << std::endl;
        std::cout << "Maze Error: Player doesn't appear to be in the maze ....\n";
    }
    else {
        MazeQueue.push(startingTile);
        
        std::cout << "Solving the maze.\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        std::cout << "Solving the maze..\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(200));
        std::cout << "Solving the maze...\n";

        while (!MazeQueue.empty()) {
            MazeNode* currentTile = MazeQueue.front();
            MazeQueue.pop();

            if (exitedMaze(*currentTile)) {
                // Empty the maze queue, we found the exit!
                MazeQueue = std::queue<MazeNode*>();
                // Go back up, calling successive parent nodes until starting node is reached
                // Don't include the "final" node, since the second last node is also "outside" the maze
                currentTile = currentTile->parentNode;
                while (currentTile->parentNode != nullptr) {
                    shortestPath.push_back(currentTile);
                    currentTile = currentTile->parentNode;
                }

                shortestPath.push_back(startingTile);

                for (MazeNode* nodes : shortestPath) {
                    mc.setBlock(nodes->location, mcpp::Blocks::LIME_CARPET);
                }
                std::cout << "Maze solved!\n";
            }
            else {
                // Vector of adjacentEdges includes tiles that are air blocks and haven't been explored yet.
                currentTile->adjacentEdges = validTiles(currentTile->location, exploredTiles);
                for (mcpp::Coordinate direction : currentTile->adjacentEdges) {
                    MazeNode* adjacentTile = new MazeNode(direction);

                    adjacentTile->parentNode = currentTile;
                    exploredTiles.push_back(adjacentTile);
                    MazeQueue.push(adjacentTile);
                }
            }
        }
    }

    curState = ST_SolveMaze;
    return curState;
}

// Add tile only if it's an air block and it hasn't been visited before
std::vector<mcpp::Coordinate> validTiles(mcpp::Coordinate currentTile, const std::vector<MazeNode*> exploredTiles) {
    std::vector<mcpp::Coordinate> validTiles;
    mcpp::Coordinate directions[4] = { MOVE_XPLUS, MOVE_ZPLUS, MOVE_XMINUS, MOVE_ZMINUS };
    
    for (mcpp::Coordinate direction : directions) {
        if ((mc.getBlock(currentTile + direction) == mcpp::Blocks::AIR) && !explored(currentTile + direction, exploredTiles)) {
            validTiles.push_back(currentTile + direction);
        }
    }

    return validTiles;
}

// If tile has been visited before, explored = true
bool explored(mcpp::Coordinate nextTile, const std::vector<MazeNode*> exploredTiles) {
    bool explored = false;

    for (MazeNode* node : exploredTiles) {
        if (nextTile == node->location) {
            explored = true;
        }
    }

    return explored;
}

bool exitedMaze(MazeNode currentTile) {
    return mc.getBlock(currentTile.location + MOVE_XPLUS) == mcpp::Blocks::AIR &&
           mc.getBlock(currentTile.location + MOVE_XMINUS) == mcpp::Blocks::AIR &&
           mc.getBlock(currentTile.location + MOVE_ZPLUS) == mcpp::Blocks::AIR &&
           mc.getBlock(currentTile.location + MOVE_ZMINUS) == mcpp::Blocks::AIR &&
           mc.getBlock(currentTile.location + MOVE_XMINUS + MOVE_ZMINUS) == mcpp::Blocks::AIR && 
           mc.getBlock(currentTile.location + MOVE_XPLUS + MOVE_ZMINUS) == mcpp::Blocks::AIR && 
           mc.getBlock(currentTile.location + MOVE_XMINUS + MOVE_ZPLUS) == mcpp::Blocks::AIR && 
           mc.getBlock(currentTile.location + MOVE_XPLUS + MOVE_ZPLUS) == mcpp::Blocks::AIR ? true : false;
}