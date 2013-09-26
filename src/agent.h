#ifndef GOLDMINE_AGENT_H_
#define GOLDMINE_AGENT_H_

#include "forward_declarations.h"

enum class SearchStrategyType {
    INVALID,
    BREADTH_FIRST,
    LIMITED_DEPTH_FIRST,
    A_STAR
};

class Agent {
  public:
    Agent(SearchStrategyType type);
    
    Action CalculateNextAction(const Perception& perception);
    
  private:
    SearchStrategyType type_;
};

#endif // GOLDMINE_AGENT_H_