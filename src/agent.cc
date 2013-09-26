#include "agent.h"

#include "actions.h"

Agent::Agent(SearchStrategyType type) {
}

Action Agent::CalculateNextAction(const Perception& perception) {
    return Action::MOVE_RIGHT;
}