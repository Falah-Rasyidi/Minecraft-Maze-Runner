#include "Agent.h"

Agent::Agent(mcpp::Coordinate startLoc) { currentLoc = startLoc; _orientation = X_PLUS; }

Agent::~Agent()
{
}

AgentOrientation Agent::rotateClockwise() {
    _orientation = static_cast<AgentOrientation>((static_cast<int>(_orientation) + 1) % 4);
    return _orientation;
}

AgentOrientation Agent::rotateAnticlockwise() {
    _orientation = static_cast<AgentOrientation>((static_cast<int>(_orientation) + 3) % 4);
    return _orientation;
}

mcpp::Coordinate Agent::forward() const {
    mcpp::Coordinate result;
    if (_orientation == X_PLUS) {
        result = mcpp::Coordinate(1, 0, 0);
    }
    else if (_orientation == Z_PLUS) {
        result = mcpp::Coordinate(0, 0, 1);
    }
    else if (_orientation == X_MINUS) {
        result = mcpp::Coordinate(-1, 0, 0);
    }
    else if (_orientation == Z_MINUS) {
        result = mcpp::Coordinate(0, 0, -1);
    }
    else {
      result = mcpp::Coordinate(0, 0, 0);
    }

    return result;
}

mcpp::Coordinate Agent::right() const {
    mcpp::Coordinate currDir = forward();
    return mcpp::Coordinate(-currDir.z, 0, currDir.x);
}