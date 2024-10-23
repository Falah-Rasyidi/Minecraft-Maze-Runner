#ifndef MENU_UTILS_H
#define MENU_UTILS_H

#include <iostream>

#include <mcpp/mcpp.h>
#include "Maze.h"

enum States {
    // Main states
    ST_Main,
    ST_BuildMaze,
    ST_GenerateMaze,
    ST_SolveMaze,
    ST_Creators,
    ST_Exit,

    // States for GenerateMazeMenu
    ST_MazeTerminal,
    ST_RandomMaze,

    // States for SolveMazeMenu
    ST_ManualSolve,
    ST_EscapeRoute
};

void printStartText(void);

// *** Main Menu
States printMainMenu(States &curState);
States printGenerateMazeMenu(States &curState);
States buildMaze(States &curState, bool usingEnhancedBuild, bool testMode);
States printSolveMazeMenu(States &curState);
States printTeamInfo(States &curState);
void printExitMassage(void);

// Variables to interact with generateMaze()
extern mcpp::MinecraftConnection mc;
extern Maze mazeInfo;

#endif