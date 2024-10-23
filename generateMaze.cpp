#include <iostream>
#include <tuple>

#include "generateMaze.h"

// Create a maze object to store the maze information from user input
// Contract: The maze object will be used to store the maze information from user input
States mazeTerminal(States &curState) {
    int xCoords = 0, yCoords = 0, zCoords = 0;
    int xLen = 0, zLen = 0;

    setBasePoint(xCoords, yCoords, zCoords);
    setDimensions(xLen, zLen);
    Maze userMaze(mcpp::Coordinate(xCoords, yCoords, zCoords), xLen, zLen, 0);
    setMazeStructure(userMaze);
    userMaze.printInfo();

    //  Transfer mazeStructure and userMaze to menuUtils
    mazeInfo = userMaze;

    curState = ST_Main;

    std::cin.clear();
    std::cin.ignore(1000, '\n');
    return curState;
}

// Create a maze object to store the randomly generated information from user input
// Contract: The maze object will be used to store the randomly generated maze information from user input
States mazeRandom(States &curState) {
    srand(time(0));

    int xCoords = 0, yCoords = 0, zCoords = 0;
    int xLen = 0, zLen = 0;

    setBasePoint(xCoords, yCoords, zCoords);
    setDimensions(xLen, zLen);
    Maze randMaze(mcpp::Coordinate(xCoords, yCoords, zCoords), xLen, zLen, 0);
    std::vector<std::vector<char>> randomStructure(xLen, std::vector<char>(zLen, 'x'));

    // The starting point should be an empty cell adjacent to the outer wall. 
    int startX = chooseStartingPoint(xLen);
    int startZ = chooseStartingPoint(zLen);
    while (startX > 2 && (startX < xLen - 2)) {
        startX = chooseStartingPoint(xLen);
    }
    while (startZ > 2 && (startZ < zLen - 2)) {
        startZ = chooseStartingPoint(zLen);
    }

    std::cout << "startX: " << startX << ", startZ: " << startZ << '\n';

    std::vector<std::tuple<int, int>> neighbours;
    // Top edge
    if (startX < 2) {
        neighbours.push_back(std::tuple<int, int>(0, startZ));
    }
    // Bottom edge
    if (startX > xLen - 3) {
        neighbours.push_back(std::tuple<int, int>(startX + 1, startZ));
    }
    // Right edge
    if (startZ > zLen - 3) {
        neighbours.push_back(std::tuple<int, int>(startX, startZ + 1));
    }
    // Left edge
    if (startZ < 2) {
        neighbours.push_back(std::tuple<int, int>(startX, 0));
    }

    if (neighbours.size() == 0) {
        // Error
        randomStructure.at(0).at(0) = '!';
    }

    // Choose a random neighbour
    int randIndex = rand() % neighbours.size();
    std::tuple<int, int> chosenNeighbour = neighbours[randIndex];
    
    // Break
    randomStructure.at(std::get<0>(chosenNeighbour)).at(std::get<1>(chosenNeighbour)) = '.';

    bool** visited = createVisitMaze(xLen, zLen);

    if (visitNeighbours(randomStructure, startX, startZ, -1, -1, visited, randMaze.getMode())) {
        std::cout << "Maze generated successfully\n";
    }
    else {
        std::cout << "Maze Error: Bad random maze generation ....\n";
    }

    randMaze.setMazeStructure(randomStructure);
    randMaze.printInfo();

    deleteVisitMaze(visited, xLen);

    //   Transfer random maze to menuUtils
    mazeInfo = randMaze;

    curState = ST_Main;
    return curState;
}

States enhancedMazeRandom(States &curState) {
    srand(time(0)); 

    int xCoords = 0, yCoords = 0, zCoords = 0;
    int xLen = 0, zLen = 0;

    setBasePoint(xCoords, yCoords, zCoords);
    setDimensions(xLen, zLen);
    Maze randMaze(mcpp::Coordinate(xCoords, yCoords, zCoords), xLen, zLen, 0);
    std::vector<std::vector<char>> randomStructure(xLen, std::vector<char>(zLen, 'x'));

    bool findingStartingPoint = true;
    int count = 0;
    const int countUntilFailure = 50;

    std::vector<std::tuple<int, int>> neighbours;
    int startX;
    int startZ;

    while (findingStartingPoint && count <= countUntilFailure) {
        // The starting point should be an empty cell adjacent to the outer wall. 
        startX = chooseStartingPoint(xLen);
        startZ = chooseStartingPoint(zLen);
        while (startX > 2 && (startX < xLen - 2)) {
            startX = chooseStartingPoint(xLen);
        }
        while (startZ > 2 && (startZ < zLen - 2)) {
            startZ = chooseStartingPoint(zLen);
        }

        // Top edge
        if (startX < 2) {
            neighbours.push_back(std::tuple<int, int>(0, startZ));
        }
        // Bottom edge
        if (startX > xLen - 3) {
            neighbours.push_back(std::tuple<int, int>(startX + 1, startZ));
        }
        // Right edge
        if (startZ > zLen - 3) {
            neighbours.push_back(std::tuple<int, int>(startX, startZ + 1));
        }
        // Left edge
        if (startZ < 2) {
            neighbours.push_back(std::tuple<int, int>(startX, 0));
        }

        if (neighbours.size() == 0) {
            // Error
            neighbours = std::vector<std::tuple<int, int>>();
            count++;
        }
        else {
            findingStartingPoint = false;
        }
    }

    // std::cout << "It took " << count + 1<< " attempts" << "\n";

    // Choose a random neighbour
    int randIndex = rand() % neighbours.size();
    std::tuple<int, int> chosenNeighbour = neighbours[randIndex];
    
    // Break
    randomStructure.at(std::get<0>(chosenNeighbour)).at(std::get<1>(chosenNeighbour)) = '.';

    bool** visited = createVisitMaze(xLen, zLen);
    
    if (enhancedVisitNeighbours(randMaze.getBasePoint(), randomStructure, startX, startZ, -1, -1, visited)) {
        std::cout << "Maze generated successfully\n";
    }
    else {
        std::cout << "Maze Error: Bad random maze generation ....\n";
    }

    randMaze.setMazeStructure(randomStructure);
    randMaze.printInfo();

    deleteVisitMaze(visited, xLen);

    // Transfer random maze to menuUtils
    mazeInfo = randMaze;

    curState = ST_Main;
    return curState;
}

States testRandom(States &curState) {
    int xCoords = 0, yCoords = 0, zCoords = 0;
    int xLen = 0, zLen = 0;

    setBasePoint(xCoords, yCoords, zCoords);
    setDimensions(xLen, zLen);

    Maze testMaze(mcpp::Coordinate(xCoords, yCoords, zCoords), xLen, zLen, 1);
    std::vector<std::vector<char>> testStructure(xLen, std::vector<char>(zLen, 'x'));

    int startX = 1;
    int startZ = 1;
    testStructure.at(1).at(0) = '.';

    bool** visited = createVisitMaze(xLen, zLen);

    if (visitNeighbours(testStructure, startX, startZ, -1, -1, visited, testMaze.getMode())) {
        std::cout << "Maze generated successfully\n";
    }
    else {
        std::cout << "Maze Error: Bad random maze generation ....\n";
    }

    testMaze.setMazeStructure(testStructure);
    testMaze.printInfo();

    deleteVisitMaze(visited, xLen);

    mazeInfo = testMaze;

    curState = ST_Main;
    return curState;
}

//  Set the basepoint of the maze
//  Contract: The basepoint of the maze will be set and a valid basepoint will be set on the passed by reference variables
void setBasePoint(int& x, int& y, int& z) {
    bool inputValid = false;
    
    //  User input for basepoint
    do {
        std::cout << "Enter the basePoint of the maze:\n";
        std::cin >> x >> y >> z;

        if (y != mc.getHeight(x, z) + 1) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');

            std::cout << std::endl;
            std::cout << "Input Error: Y-coordinate must be one unit above the ground, an ideal y-coordinate would be: " 
                            << mc.getHeight(x, z) + 1 << " ....\n";
        }
        else if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');

            std::cout << std::endl;
            std::cout << "Input Error: Basepoint coordinates must be integer values ....\n";
        }
        else {
            inputValid = true;
        }
    } while (!inputValid);
}

// Set the dimensions of the maze
// Contract: The dimensions of the maze will be set and valid dimensions will be set on the passed by reference variables
void setDimensions(int& xlen, int& zlen) {
    // User input for length and width
    do {
        std::cout << "Enter the length and width of maze:\n";
        std::cin >> xlen >> zlen;

        if (std::cin.fail() || xlen <= 0 || zlen <= 0 || xlen % 2 == 0 || zlen % 2 == 0) {
            std::cin.clear(); 
            std::cin.ignore(1000, '\n'); 

            std::cout << std::endl;
            std::cout << "Input error: Length and width must be positive odd integer values ....\n";
        }
    } while (xlen <= 0 || zlen <= 0 || xlen % 2 == 0 || zlen % 2 == 0);
}

// Set the maze structure
// Contract: The maze structure will be set and valid maze structure will be set 
void setMazeStructure(Maze& mazeIn) {
    bool invalidCell = false;
    bool checkExits = false;
    bool checkCycle = false;
    bool checkPath = false;

    const int xLen = mazeIn.getXLen();
    const int zLen = mazeIn.getZLen();
    
    std::vector<std::vector<char>> tmpMaze(xLen,std::vector<char>(zLen));

    do {
        invalidCell = false;
        // Enter maze structure
        std::cout << "Enter the maze structure:\n";
        for (int i = 0; i < xLen; i++) {
            for (int j = 0; j < zLen; j++) {
                std::cin >> tmpMaze.at(i).at(j);

                if (tmpMaze.at(i).at(j) != 'x' && tmpMaze.at(i).at(j) != '.') {
                    invalidCell = true;
                }
            }
        }

        mazeIn.setMazeStructure(tmpMaze);

        // Check for exits, cycles, path, entrances while at it
        checkExits = validExits(mazeIn);
        checkCycle = hasCycle(mazeIn);
        checkPath = hasPath(mazeIn);
        // std::cout << "checkPath: " << checkPath << "\n";
        if (!checkExits) 
            std::cout << "Input Error: Maze must have an exit, no corner exits and no adjacent exits ....\n";

        if (checkCycle)
            std::cout << "Input Error: Maze must not have a cycle ....\n";

        if (!checkPath) 
            std::cout << "\nInput Error: Maze must have a path ....\n";

        if (invalidCell) 
            std::cout << "\nInput Error: Maze must only contain 'x' or '.' ....\n";

        if (!checkExits || checkCycle || !checkPath || invalidCell) {
            std::cin.clear();
            std::cin.ignore(1000, '\n');
        }

    } while (!checkExits || checkCycle || !checkPath || invalidCell);

    std::cout << "Maze read successfully\n";
 }

//   Return true if there is a VALID AMOUNT of exits (1), no adjacent exits, no corner exits
bool validExits(const Maze& mazeIn) {
    int exitCount = 0;
    bool result = true;

    const int xLen = mazeIn.getXLen();
    const int zLen = mazeIn.getZLen();

    // Check for corner exits
    if (mazeIn.getCell(0, 0) == '.'
        || mazeIn.getCell(0, zLen - 1) == '.'
            || mazeIn.getCell(xLen - 1, 0) == '.'
                || mazeIn.getCell(xLen - 1, zLen - 1) == '.') {
                    std::cout << std::endl;
                    std::cout << "Maze Exit Error: There should be no corner exits ....\n";
        result = false;
    }

    // Count exits
    // Check top && bottom, including corners/edge
    for (int i = 0; i < zLen; i++) {
        if (mazeIn.getCell(0, i) == '.') {
            ++exitCount;

            if (checkAdjacentExit(mazeIn, 0, i - 1) || checkAdjacentExit(mazeIn, 0, i + 1)) {
                result = false;
            }
        }
        if (mazeIn.getCell(xLen - 1, i) == '.') {
            ++exitCount;
            
            if (checkAdjacentExit(mazeIn, xLen - 1, i - 1) || checkAdjacentExit(mazeIn, xLen - 1, i + 1)) {
                result = false;
            }
        }
    }
    
    //   Check right && left, besides corners
    for (int i = 1; i < xLen - 1; i++) {
        if (mazeIn.getCell(i, 0) == '.') {
            ++exitCount;
            
            if (checkAdjacentExit(mazeIn, i - 1, 0) || checkAdjacentExit(mazeIn, xLen + 1, 0)) {
                result = false;
            }
        }
        if (mazeIn.getCell(i, zLen - 1) == '.') {
            ++exitCount;

            if (checkAdjacentExit(mazeIn, i - 1, zLen - 1) || checkAdjacentExit(mazeIn, xLen + 1, zLen - 1)) {
                result = false;
            }
        }
    }

    if (exitCount != 1) {
        std::cout << std::endl;
        std::cout << "Maze Exit Error: There should be 1 exit, there are: " << exitCount << " exits ....\n";

        result = false;
    }

    return result;
}

// Return true if there is an adjacent exit
bool checkAdjacentExit(const Maze mazeIn, const int currX, const int currZ) {
    bool retValue = false;

    if (isWithinBounds(mazeIn, currX, currZ)) {
        if (mazeIn.getCell(currX, currZ) == '.') {
            std::cout <<  "Maze Error: There should be no adjacent exits ....\n";
            retValue = true;
        }
    }

    if (isWithinBounds(mazeIn, currX, currZ)) {
        if (mazeIn.getCell(currX, currZ) == '.') {
            std::cout <<  "Maze Error: There should be no adjacent exits ....\n";
            retValue = true;
        }
    }

    return retValue;
}

//   Returns true if the given maze contains a cycle
bool hasCycle(const Maze& mazeIn) {
    bool retValue = false;

    int xLen = mazeIn.getXLen();
    int zLen = mazeIn.getZLen();

    //   This will be a set of things we have visited
    bool** visited = createVisitMaze(xLen, zLen);

    for (int i = 0; i < xLen; i++) {
        for (int j = 0; j < zLen; j++) {
            if (!retValue) {
                if (!visited[i][j] && mazeIn.getCell(i, j) == '.') {
                    //   Do dfs if not visited and is air
                    if (dfs(mazeIn, i, j, -1, -1, visited)) {
                        deleteVisitMaze(visited, xLen);
                        retValue = true;
                    }
                }
            }
        }
    }    

    if (!retValue) {
        deleteVisitMaze(visited, xLen);
    }
    
    return retValue;
 }

// Return true if there is a cycle in the maze, else false, in which its indicated from there being a cell that has been visited twice
bool dfs(const Maze mazeIn, const int currX, const int currZ, const int prevX, const int prevZ, bool** visited) {
    bool retValue = false;

    // Mark current "node" as visited
    visited[currX][currZ] = true;

    int directionsX[] = {-1, 1, 0, 0};
    int directionsZ[] = {0, 0, -1, 1};

    for (int i = 0; i < 4; i++) {
        int nextX = currX + directionsX[i];
        int nextZ = currZ + directionsZ[i];

        if (isWithinBounds(mazeIn, nextX, nextZ) && isNotParent(nextX, prevX, nextZ, prevZ)) {
            if (!retValue && mazeIn.getCell(nextX, nextZ) == '.') {
                if (visited[nextX][nextZ]) {
                    retValue = true;
                }
                else if (dfs(mazeIn, nextX, nextZ, currX, currZ, visited)) {
                    retValue = true;
                }
            }
        }
    }

    return retValue;
}

//  Returns true if the given maze contains a path from the entrance to the exit
bool hasPath(const Maze& mazeIn) {
    bool retValue = true;

    int xLen = mazeIn.getXLen();
    int zLen = mazeIn.getZLen();

    for (int i = 0; i < xLen; i++) {
        for (int j = 0; j < zLen; j++) {
            // Make a visited maze for this the ij node
            bool** visited = createVisitMaze(xLen, zLen);

            // Visit the node only if it has not been visited and is NOT an exit
            if (retValue && !visited[i][j] && !isExit(mazeIn, i, j) && mazeIn.getCell(i, j) == '.') {
                if (visitNode(mazeIn, i, j, -1, -1, visited)) {
                    retValue = true;
                }
                else {
                    retValue = false;
                }
            }

            deleteVisitMaze(visited, xLen);
        }
    }    

    return retValue;
}

//  Returns true if the given maze contains a path from the entrance to the exit, else false if there is no path from the origin cell to the exit
bool visitNode(const Maze mazeIn, const int currX, const int currZ, const int prevX, const int prevZ, bool** visited) {
    bool retValue = false;

    visited[currX][currZ] = true;

    if (prevX != -1 && prevZ != -1 && isExit(mazeIn, currX, currZ)) {
        retValue = true;
    }

    const int directionsX[] = {-1, 1, 0, 0};
    const int directionsZ[] = {0, 0, -1, 1};

    if (!retValue) {
        for (int i = 0; i < 4; i++) {
            int nextX = currX + directionsX[i];
            int nextZ = currZ + directionsZ[i];

            if (!retValue && isWithinBounds(mazeIn, nextX, nextZ) && isNotParent(currX, nextX, currZ, nextZ)) {
                if (mazeIn.getCell(nextX, nextZ) == '.' && !visited[nextX][nextZ]) {
                    if (visitNode(mazeIn, nextX, nextZ, currX, currZ, visited)) {
                        retValue = true;
                    }
                }
            }
        }
    }

    return retValue;
}

//  Returns true if the current cell is an exit, else false
bool isExit(const Maze maze, const int currX, const int currZ) {
    int xLen = maze.getXLen();
    int zLen = maze.getZLen();
    
    // If on the outer ring of the maze
    return (currX >= 0 && (currZ == 0 || currZ == zLen - 1)) 
        || (currZ >= 0 && (currX == 0 || currX == xLen - 1));
}

//  Returns a integer value that will always be a positive odd value
int chooseStartingPoint(const int length) {
    int coord = 0;

    do {
        coord = (rand() % (length - 1)) + 1;
    } 
    while(coord % 2 == 0);

    return coord; 
}

// Returns true if a randomly generated maze was built successfully, else false
bool visitNeighbours(std::vector<std::vector<char>>& randMaze, const int currX, const int currZ, const int prevX, const int prevZ, bool** visited, bool testMode) {
    bool retValue = false;

    visited[currX][currZ] = true;
    randMaze.at(currX).at(currZ) = '.';

    if (!isNeighbouringNodes(randMaze, currX, currZ, visited)) {
    }
    else {
        if (testMode) {
            int directionsX[] = {-2, 0, 2, 0};
            int directionsZ[] = {0, 2, 0, -2};

            for (int i = 0; i < 4; i++) {
                // if that node is within the bounds, check if its visitied
                if (isWithinBounds(randMaze, currX + directionsX[i], currZ + directionsZ[i])) {                
                    // if not visited, visit
                    if (!visited[currX + directionsX[i]][currZ + directionsZ[i]]) {
                        makeWay(randMaze, currX + directionsX[i], currZ + directionsZ[i], directionsX[i],  directionsZ[i]);
                        visitNeighbours(randMaze, currX + directionsX[i], currZ + directionsZ[i], currX, currZ, visited, testMode);
                    }
                }
            }
        }
        else {
            int directionsX[] = {-2, 2, 0, 0};
            int directionsZ[] = {0, 0, -2, 2};
            shuffleDirections(directionsX, directionsZ);

            for (int i = 0; i < 4; i++) {
                // if that node is within the bounds, check if its visitied
                if (isWithinBounds(randMaze, currX + directionsX[i], currZ + directionsZ[i])) {                
                    // if not visited, visit
                    if (!visited[currX + directionsX[i]][currZ + directionsZ[i]]) {
                        makeWay(randMaze, currX + directionsX[i], currZ + directionsZ[i], directionsX[i],  directionsZ[i]);
                        visitNeighbours(randMaze, currX + directionsX[i], currZ + directionsZ[i], currX, currZ, visited, testMode);
                    }
                }
            }
        }

        retValue = true;
    }

    return retValue;
}

bool enhancedVisitNeighbours(const mcpp::Coordinate& basePoint, std::vector<std::vector<char>>& randMaze, const int currX, const int currZ, const int prevX, const int prevZ, bool** visited) {
    bool retValue = false;

    visited[currX][currZ] = true;
    randMaze.at(currX).at(currZ) = '.';

    if (!isNeighbouringNodes(randMaze, currX, currZ, visited)) {
    }
    else {
        int directionsX[] = {-2, 2, 0, 0};
        int directionsZ[] = {0, 0, -2, 2};
        shuffleDirections(directionsX, directionsZ);

        // std::cout << "Originating from " << "(" << basePoint.x + currX << ", \'y\', " << basePoint.z + currZ << ")\n";

        for (int i = 0; i < 4; i++) {
            // if that node is within the bounds, check if its visitied
            if (isWithinBounds(randMaze, currX + directionsX[i], currZ + directionsZ[i])) {                
                // Check steepness  
                // Curr block
                int currHeight = mc.getHeight(basePoint.x + currX, basePoint.z + currZ);
                // Wall we are trying ot break
                int wallHeight = mc.getHeight(basePoint.x + currX + directionsX[i] / 2, basePoint.z + currZ + directionsZ[i] / 2);
                // Block we want to visit
                int nextHeight = mc.getHeight(basePoint.x + currX + directionsX[i], basePoint.z + currZ + directionsZ[i]);

                // std::cout << "Currently visiting " << "(" << basePoint.x + currX << ", " << currHeight << ", " << basePoint.z + currZ << ")\n";
                // std::cout << "Currently looking at " << "(" << basePoint.x + currX + directionsX[i] << ", " << currHeight << ", " << basePoint.z + currZ + directionsZ[i] << ")\n";

                bool canBreakWall = (abs(currHeight - wallHeight) <= 1 && abs(wallHeight - nextHeight) <= 1);

                // std::cout << "Can we break wall? " << canBreakWall << "\n";
                // std::cout << "First: " << abs(currHeight - wallHeight) << "\n";
                // std::cout << "Second: " << abs(wallHeight - nextHeight) << "\n\n";
                
                // if not visited, visit
                if (canBreakWall && !visited[currX + directionsX[i]][currZ + directionsZ[i]]) {
                    makeWay(randMaze, currX + directionsX[i], currZ + directionsZ[i], directionsX[i],  directionsZ[i]);
                    enhancedVisitNeighbours(basePoint, randMaze, currX + directionsX[i], currZ + directionsZ[i], currX, currZ, visited);
                }
            }
        }

        retValue = true;
    }

    return retValue;
}

// Shuffle the directions array to ensure that the maze is more random
void shuffleDirections(int directionsX[], int directionsZ[]) {
    int tempX = 0, tempZ = 0, randI = 0;

    for (int i = 0; i < 4; i++) {
        randI = rand() % 4;
        
        tempX = directionsX[i];
        tempZ = directionsZ[i];

        directionsX[i] = directionsX[randI];
        directionsZ[i] = directionsZ[randI];

        directionsX[randI] = tempX;
        directionsZ[randI] = tempZ;
    }
}

// Returns true if within bounds of maze, else false
bool isWithinBounds(const Maze& mazeIn, const int offsetX, const int offsetZ) {
    int xLen = mazeIn.getXLen();
    int zLen = mazeIn.getZLen();

    return (offsetX >= 0 && offsetX < xLen) && (offsetZ >= 0 && offsetZ < zLen);
}

// Returns true if within bounds of maze structure, else false
bool isWithinBounds(const std::vector<std::vector<char>>& randMaze, const int offsetX, const int offsetZ) {
    int xLen = randMaze.size();
    int zLen = randMaze.at(0).size();

    return (offsetX >= 0 && offsetX < xLen) && (offsetZ >= 0 && offsetZ < zLen);
}

// Returns true if there is a neighbouring node, else false
bool isNeighbouringNodes(const std::vector<std::vector<char>>& randMaze, const int x, const int z, bool** visited) {
    bool retValue = false;

    int directionsX[] = {-2, 2, 0, 0};
    int directionsZ[] = {0, 0, -2, 2};

    for (int i = 0; i < 4; i++) {
        if (!retValue) {
            // If within the bounds and has not been visited then return, there is a neighbouring node
            if (isWithinBounds(randMaze, x + directionsX[i], z + directionsZ[i])) {
                if (!visited[x + directionsX[i]][z + directionsZ[i]]) {
                    retValue = true;
                }
            }
        }
    }

    return retValue;
}

// Make a way in the maze based on the direction
void makeWay(std::vector<std::vector<char>>& randMaze, const int offsetX, const int offsetZ, const int dirX, const int dirZ) {
    randMaze.at(offsetX).at(offsetZ) = '.';

    if (dirX == 2) {
        randMaze.at(offsetX - 1).at(offsetZ) = '.';
    }
    else if (dirX == -2) {
        randMaze.at(offsetX + 1).at(offsetZ) = '.';
    }
    else if (dirZ == 2) {
        randMaze.at(offsetX).at(offsetZ - 1) = '.';
    }
    else if (dirZ == -2) {
        randMaze.at(offsetX).at(offsetZ + 1) = '.';
    }
}

// Creates a 2D array of booleans to keep track of visited nodes
bool** createVisitMaze(const int xLen, const int zLen) {
    bool** visited = new bool *[xLen];

    for (int i = 0; i < xLen; i++) {
        visited[i] = new bool[zLen];

        for (int j = 0; j < zLen; j++) {
            visited[i][j] = false;
        }
    }

    return visited;
}

// Deletes the 2D array of booleans to keep track of visited nodes
void deleteVisitMaze(bool** visited, const int xLen) {
    for (int i = 0; i < xLen; i++) {
        delete[] visited[i];
        visited[i] = nullptr;
    } 

    delete[] visited;
    visited = nullptr;
}

// Returns true if the child node is not the parent node, else false
bool isNotParent(const int childX, const int parentX, const int childZ, const int parentZ) {
    return (childX != parentX) || (childZ != parentZ);
}