#include "agent.h"

#include "actions.h"

Agent::Agent(const Strategy& strategy) : strategy_(strategy) {}

Action Agent::CalculateNextAction(const Perception& perception, const State& current_state) {
    if(next_actions_.empty())
        next_actions_ = strategy_(perception, current_state);

    Action result = next_actions_.front();
    next_actions_.pop();
    return result;
}