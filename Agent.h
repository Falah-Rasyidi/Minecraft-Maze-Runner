#ifndef ASSIGN3_AGENT_H
#define ASSIGN3_AGENT_H

#include <iostream>
#include <mcpp/mcpp.h>

#define MOVE_XPLUS mcpp::Coordinate(1,0,0)
#define MOVE_XMINUS mcpp::Coordinate(-1,0,0)
#define MOVE_ZPLUS mcpp::Coordinate(0,0,1)
#define MOVE_ZMINUS mcpp::Coordinate(0,0,-1)

enum solveAlgorithm{
        RIGHT_HAND_FOLLOW,
        BREATH_FIRST_SEARCH,
};

enum AgentOrientation{
    X_PLUS,   // North
    Z_PLUS,   // East
    X_MINUS,  // South
    Z_MINUS   // West
};  

class Agent
{

public:
    Agent(mcpp::Coordinate startLoc);
    ~Agent();
    mcpp::Coordinate currentLoc;

    AgentOrientation rotateClockwise();
    AgentOrientation rotateAnticlockwise();

    mcpp::Coordinate forward() const;

    mcpp::Coordinate right() const;

private:
    /* data */
    mcpp::MinecraftConnection mc;
    AgentOrientation _orientation;
};



#endif //ASSIGN3_AGENT_H