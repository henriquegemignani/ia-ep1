#ifndef GOLDMINE_AGENT_H_
#define GOLDMINE_AGENT_H_

#include "forward_declarations.h"
#include "actions.h"
#include <queue>
#include <functional>

enum class SearchStrategyType {
    INVALID,
    BREADTH_FIRST,
    LIMITED_DEPTH_FIRST,
    A_STAR
};

typedef std::function<std::queue<Action> (const Perception&, const State&)> Strategy;

class Agent {
  public:
    Agent(SearchStrategyType type);
    
    Action CalculateNextAction(const Perception& perception, const State& current_state);
    
  private:
    Strategy strategy_;
    std::queue<Action> next_actions_;
};

#endif // GOLDMINE_AGENT_H_