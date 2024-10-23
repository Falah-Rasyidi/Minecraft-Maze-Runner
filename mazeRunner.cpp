#include "mazeRunner.h"

#define NORMAL_MODE 0
#define TESTING_MODE 1

// Remark:
// To enable each of the enhancements, use the following flags:
// Test Mode:   -testmode
// BFS:         -e-solve
// Better Maze: -e-generation

// Valid ways to use CLA (in any order):
// ./mazeRunner -{testmode/-e-{solve/generation}}
// ./mazeRunner -testmode -e-{solve/generation}
// ./mazeRunner -testmode -e-{solve/generation}
// ./mazeRunner -testmode -e-solve -e-generation

int main(int argc, char **argv) {
    bool mode = NORMAL_MODE;
    bool useBFS = false;
    bool useBetterMazes = false;

    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "-testmode") == 0) {
            mode = TESTING_MODE;
        }
        else if (strcmp(argv[i], "-e-solve") == 0) {
            useBFS = true;
        }
        else if (strcmp(argv[i], "-e-generation") == 0) {
            useBetterMazes = true;
        }
    }

    if (mode == TESTING_MODE) {
        std::cout << "Running in test mode...\n";
    }
    if (useBFS) {
        std::cout << "Using enhanced maze solving (BFS)...\n";
    }
    if (useBetterMazes) {
        std::cout << "Using enhanced maze generation...\n";
    }

    // read Mode
    // Maze currentMaze;

    mcpp::MinecraftConnection mc; 
    mc.doCommand("time set day");

    States curState = ST_Main;

    printStartText(); 

    // State machine for menu
    while (curState != ST_Exit)
    {
        // Main menu options
        if (curState == ST_Main) {
            printMainMenu(curState);
        }
        else if (curState == ST_GenerateMaze) {
            printGenerateMazeMenu(curState);
        }
        else if (curState == ST_BuildMaze) {
            buildMaze(curState, useBetterMazes, mode);
        }
        else if (curState == ST_SolveMaze) {
            printSolveMazeMenu(curState);
        }
        else if (curState == ST_Creators) {
            printTeamInfo(curState);
        }
        
        // Generate maze
        else if (curState == ST_MazeTerminal) {
            mazeTerminal(curState);
        }
        else if (curState == ST_RandomMaze) {
            if (useBetterMazes) {
                // Use the better mazes method
                enhancedMazeRandom(curState);
            }
            else if (mode) {
                // Testing mode
                testRandom(curState);
            }
            else mazeRandom(curState);
        }
        
        // Solve maze
        else if (curState == ST_ManualSolve) {
            manualSolve(curState);
        }
        else if (curState == ST_EscapeRoute) {
            if (useBFS) {
                shortestPath(curState);
            }
            else {
                escapeRoute(curState);
            }
        }
        
        else {
            curState = ST_Main;
        }
    }

    printExitMassage();

    return EXIT_SUCCESS;
}

// for commits
// Co-authored-by: leoinovejas <leoinovejas@users.noreply.github.com>
// Co-authored-by: Falah-Rasyidi <Falah-Rasyidi@users.noreply.github.com>
// Co-authored-by: nebbyJammin <nebbyJammin@users.noreply.github.com>
// Co-authored-by: 2trill2code <2trill2code@users.noreply.github.com>

// TODO: 6/10/2023
// - Implement mazeTerminal()
// - Link mazeTerminal() with buildMaze()
// - Write input/expout files

// TODO: 9/10/2023 or Weekend
// - Continue implementing mazeTerminal()
// - Continue implementing buildMaze()
// - Write more input/expout files
//   > buildMaze() with empty maze
//   > Inputting more characters than specified into maze
//   > Inputting characters other than 'x' or '.' into maze
//   > Testing all menus with wrong inputs
//   > Invalid coordinates in generateMaze() (above build limit, in the void, etc.)
//   > Unnecessarily large numbers for length/width in generateMaze()?

// TODO: Finish by the end of the week
// - Wall follower algorithm
// - Check for loops
// - Supporting blocks beneath maze