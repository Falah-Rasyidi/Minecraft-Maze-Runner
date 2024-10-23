#include "menuUtils.h"
#include <chrono>
#include <thread>

mcpp::MinecraftConnection mc;
Maze mazeInfo;

struct Block {
    mcpp::Coordinate coordinate;
    mcpp::BlockType ID;

    Block() : coordinate(mcpp::Coordinate(0, 0, 0)), ID(0) {
    }

    Block(mcpp::Coordinate _coordinate, mcpp::BlockType _id) : coordinate(_coordinate), ID(_id) {
    }

    Block& operator=(const Block& origBlock) {
        coordinate = origBlock.coordinate;
        ID = origBlock.ID;

        return *this;
    }
};

std::vector<Block> stateCollection;
std::vector<std::vector<int>> terrainHeights;
std::vector<mcpp::Coordinate> terraform;

void delay() {
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
}

void printStartText(void) {
    std::cout << std::endl;
    std::cout << "Welcome to MineCraft MazeRunner!\n";
    std::cout << "--------------------------------\n";
}

// *** Main Menu
States printMainMenu(States &curState) {
    std::cout << std::endl;
    std::cout << "------------- MAIN MENU -------------\n";
    std::cout << "1) Generate Maze\n";
    std::cout << "2) Build Maze in MineCraft\n";
    std::cout << "3) Solve Maze\n";
    std::cout << "4) Show Team Information\n";
    std::cout << "5) Exit\n";
    std::cout << "\nEnter Menu item to continue:\n";

    int userInput = 0;

    std::cin >> userInput;

    if (userInput == 1) {
        curState = ST_GenerateMaze;
    }
    else if (userInput == 2) {
        // TODO: BUILD THE MAZE
        // * Change state back to main after building maze
        curState = ST_BuildMaze;
    }
    else if (userInput == 3) {
        curState = ST_SolveMaze;
    }
    else if (userInput == 4) {
        curState = ST_Creators;
    }
    else if (userInput == 5) {
        curState = ST_Exit;
    }
    else {
        std::cout << std::endl;
        std::cout << "Input Error: Enter a number between 1 and 5 ....\n";
        curState = ST_Main;
    }

    std::cin.clear();
    std::cin.ignore(1000, '\n');
    return curState;
}


States printGenerateMazeMenu(States &curState) {
    std::cout << std::endl;
    std::cout << "------------- GENERATE MAZE -------------\n";
    std::cout << "1) Read Maze from terminal\n";
    std::cout << "2) Generate Random Maze\n";
    std::cout << "3) Back\n";
    std::cout << "\nEnter Menu item to continue:\n";

    int userInput = 0;
    std::cin >> userInput;

    if (userInput == 1) {
        curState = ST_MazeTerminal;
    }   
    // Generate random maze 
    else if (userInput == 2) {
        curState = ST_RandomMaze;
    }
    // Return back to main
    else if (userInput == 3) {         
        curState = ST_Main;
    }
    else {
        std::cout << std::endl;
        std::cout << "Input Error: Enter a number between 1 and 3 ....\n";
        curState = ST_GenerateMaze;
    }

    std::cin.clear();
    std::cin.ignore(1000, '\n');
    return curState;
}

States buildMaze(States &curState, bool usingEnhancedBuild, bool testMode) {
    /**
     * Error handle user attempting to build maze without creating one first.
     * Also ensure that any existing maze is erased before building next maze. Only one maze to exist at a time.
    */
    if (!stateCollection.empty()) {
        while (!stateCollection.empty()) {
            Block currentBlock = stateCollection.back();
            int x = currentBlock.coordinate.x;
            int y = currentBlock.coordinate.y;
            int z = currentBlock.coordinate.z;
            mcpp::BlockType initialBlock = currentBlock.ID;

            mc.setBlock(mcpp::Coordinate(x, y, z), mcpp::BlockType(initialBlock));
            delay();

            stateCollection.pop_back();
        }
        while (!terraform.empty()) {
            mcpp::Coordinate currCoord = terraform.back();
            mc.setBlock(currCoord, mcpp::Blocks::AIR);
            terraform.pop_back();
        }
    }
    if (mazeInfo.empty()) {
        std::cout << std::endl;
        std::cout << "Maze Error: No maze loaded in ....\n";

        curState = ST_Main;
    }

    unsigned int index = 0;
    const int MAZE_HEIGHT = 3;
    const int zLen = mazeInfo.getZLen();
    const int xLen = mazeInfo.getXLen();
    const mcpp::Coordinate basePoint = mazeInfo.getBasePoint();

    int xCoords = basePoint.x;
    int yCoords = basePoint.y;
    int zCoords = basePoint.z;

    int width = zCoords + zLen;
    int length = xCoords + xLen;
    int height = yCoords + MAZE_HEIGHT;

    bool invalid = false;

    // If we are in normal build mode
    if (!usingEnhancedBuild && curState != ST_Main) {
        mc.setPlayerPosition(mcpp::Coordinate(xCoords, yCoords + 10, zCoords));
        stateCollection.resize(xLen * zLen * (MAZE_HEIGHT + 1));
        
        // Store initial state (including maze floor) and replace with air blocks
        for (int y = yCoords - 1; y < height; y++) {
            for (int x = xCoords; x < length; x++) {
                for (int z = zCoords; z < width && index < stateCollection.size(); z++, index++) {
                    mcpp::Coordinate currCoordinate = mcpp::Coordinate(x, y, z);
                    mcpp::BlockType block = mc.getBlock(currCoordinate);

                    stateCollection.at(index) = Block(currCoordinate, block);

                    mc.setBlock(mcpp::Coordinate(x, y, z), mcpp::Blocks::AIR);
                }
            }
        }

        // Terraform if maze has unsupported ground
        terrainHeights = mc.getHeights(mcpp::Coordinate(xCoords, yCoords, zCoords), mcpp::Coordinate(length, yCoords, width));
        unsigned int i = 0;
        for (int x = xCoords; x < length; x++, i++) {
            unsigned int j = 0;
            for (int z = zCoords; z < width; z++, j++) {
                if (terrainHeights.at(i).at(j) < yCoords - 1) {
                    int height = terrainHeights.at(i).at(j) + 1;
                    while (height < yCoords - 1) {
                        mc.setBlock(mcpp::Coordinate(x, height, z), mcpp::Blocks::DIRT);
                        terraform.push_back(mcpp::Coordinate(x, height, z));
                        height++;
                        delay();
                    }
                }
            }
        }

        // Add 'floor' of maze
        for (int x = xCoords; x < length; x++) {
            for (int z = zCoords; z < width; z++) {
                mc.setBlock(mcpp::Coordinate(x, yCoords - 1, z), mcpp::Blocks::GRASS);
                delay();
            }
        }

        // Build the maze
        for (int y = yCoords; y < height; y++) {
            int i = 0;
            for (int x = xCoords; x < length; x++, i++) {
                int j = 0;
                for (int z = zCoords; z < width; z++, j++) {
                    if (mazeInfo.getCell(i, j) == 'x') {
                        mc.setBlock(mcpp::Coordinate(x, y, z), mcpp::Blocks::ACACIA_WOOD_PLANK);
                        delay();
                    }
                }
            }
        }
    }

    // If we are in enhanced building mode
    else if (usingEnhancedBuild && curState != ST_Main) {
        mc.setPlayerPosition(mcpp::Coordinate(xCoords, yCoords + 10, zCoords));
        int i = 0;
        for (int x = xCoords; x < length; x++, i++) {
            int j = 0;
            for (int z = zCoords; z < width; z++, j++) {
                if (mazeInfo.getCell(i, j) == '.') {
                    int height = mc.getHeight(x, z) + 1;

                    const int dirX[] = { 0, 1, 0, 0 };
                    const int dirZ[] = { 0, 0, -1, 0 };

                    for (int k = 0; k < 4; k++) {
                        int diff = (mc.getHeight(x + dirX[k], z + dirZ[k]) + 1) - height;
                        // std::cout << "Height at (" << x << ", " << z << ") is " << height << "\n";
                        // std::cout << "Height at (" << x + dirX[k] << ", " << z + dirZ[k] << ") is " << mc.getHeight(x + dirX[k], z + dirZ[k]) << "\n";

                        // Consider it invalid if diff of >= 2 or <= -2 (-2 cause of drops)
                        if ((diff >= 2 || diff <= -2) 
                            && ((i + dirX[k] >= 0 && i + dirX[k] < xLen) && (j + dirZ[k] >= 0 && j + dirZ[k] < zLen)) 
                            && mazeInfo.getCell(i + dirX[k], j + dirZ[k]) == '.') {
                            invalid = true;
                        }
                    }
                }
            }
        }

        if (!invalid) {
            std::cout << "Able to build the maze!\n\n";
            int i = 0;
            for (int x = xCoords; x < length; x++, i++) {
                int j = 0;
                for (int z = zCoords; z < width; z++, j++) {
                    if (mazeInfo.getCell(i, j) == 'x') {
                        int height = mc.getHeight(x, z);

                        for (int y = 0; y < MAZE_HEIGHT; y++) {
                            mcpp::Coordinate currCoord = mcpp::Coordinate(x, height + y + 1, z);
                            mcpp::BlockType currBlock = mc.getBlock(currCoord);

                            stateCollection.push_back(Block(currCoord, currBlock));

                            mc.setBlock(currCoord, mcpp::Blocks::ACACIA_WOOD_PLANK);
                            delay();
                        }
                    }
                }
            }
        }
        else {
            std::cout << "Invalid maze!\n";
        }
    }
    
    curState = ST_Main;
    return curState;
}

States printSolveMazeMenu(States &curState) {
    std::cout << std::endl;
    std::cout << "------------- SOLVE MAZE -------------\n";
    std::cout << "1) Solve Manually\n";
    std::cout << "2) Show Escape Route\n";
    std::cout << "3) Back\n";
    std::cout << "\nEnter Menu item to continue:\n";

    int userInput = 0;

    std::cin >> userInput;

    if (userInput == 1) {
        // TODO: IMPLEMENT THIS
        curState = ST_ManualSolve;
    }
    else if (userInput == 2) {
        // TODO: IMPLEMENT THIS
        curState = ST_EscapeRoute;
    }
    else if (userInput == 3) {
        curState = ST_Main;
    }
    else {
        std::cout << std::endl;
        std::cout << "Input Error: Enter a number between 1 and 3 ....\n";
        curState = ST_SolveMaze;
    }

    std::cin.clear();
    std::cin.ignore(1000, '\n');
    return curState;
}

States printTeamInfo(States &curState) {
    std::cout << std::endl;
    std::cout << "Team members\n";
    std::cout << '\t' << "[1] Falah Rasyidi (s4007133@student.rmit.edu.au)\n";
    std::cout << '\t' << "[2] Mark Castillo (s4007180@student.rmit.edu.au)\n";
    std::cout << '\t' << "[3] Benjamin Nguyen (s4008016@student.rmit.edu.au)\n";
    std::cout << '\t' << "[4] Frandom Leo Inovejas (s3923193@student.rmit.edu.au)\n";

    curState = ST_Main;
    
    return curState;
}

void printExitMassage(void) {
    // Restore any changes made
    while (!stateCollection.empty()) {
        // Restore changes backwards
          Block currentBlock = stateCollection.back();
          int x = currentBlock.coordinate.x;
          int y = currentBlock.coordinate.y;
          int z = currentBlock.coordinate.z;
          mcpp::BlockType initialBlock = currentBlock.ID;

          mc.setBlock(mcpp::Coordinate(x, y, z), mcpp::BlockType(initialBlock));
          delay();

          stateCollection.pop_back();
    }
    while (!terraform.empty()) {
        mcpp::Coordinate currCoord = terraform.back();
        mc.setBlock(currCoord, mcpp::Blocks::AIR);
        terraform.pop_back();
    }

    std::cout << std::endl;
    std::cout << "The End!\n";
}