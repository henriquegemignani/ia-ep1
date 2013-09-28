#include "agent.h"

#include "actions.h"
#include "errors.h"

void BreadthFirstStrategy(const Perception&, std::queue<Action>& actions) {
    actions.push(Action::DONE);
}

Strategy findStrategy(SearchStrategyType type) {
    switch(type) {
    case SearchStrategyType::BREADTH_FIRST: return BreadthFirstStrategy;
    default: throw input_error("Strategy not yet implemented.");
    }
}

Agent::Agent(SearchStrategyType type) : strategy_(findStrategy(type)) {}

Action Agent::CalculateNextAction(const Perception& perception) {
    if(next_actions_.empty())
        strategy_(perception, next_actions_);

    Action result = next_actions_.front();
    next_actions_.pop();
    return result;
}