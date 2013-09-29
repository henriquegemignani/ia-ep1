#ifndef GOLDMINE_AGENT_H_
#define GOLDMINE_AGENT_H_

#include "forward_declarations.h"
#include "actions.h"
#include <queue>
#include <functional>
#include <memory>

enum class SearchTarget {
    GOLD,
    ENTRANCE
};

typedef std::function<StatePtr(const Perception&, const StatePtr&)> TargetCheckFunction;
typedef std::function<StatePtr(const Perception&, const StatePtr&, SearchTarget)> Strategy;

StatePtr BreadthFirstStrategy(const Perception&, const StatePtr&, SearchTarget);
StatePtr LimitedDepthFirstStrategy(const Perception&, const StatePtr&, const SearchTarget);
StatePtr AStarStrategy(const Perception&, const StatePtr&, SearchTarget);

TargetCheckFunction GetCheckFunction(SearchTarget);

class Agent {
  public:
    Agent(const Strategy& strategy);
    
    Action CalculateNextAction(const Perception& perception, const std::shared_ptr<const State>& current_state);
    
  private:
    Strategy strategy_;
    std::queue<Action> next_actions_;

    StatePtr SearchForFixedGold(const Perception&, const StatePtr&, std::size_t num_gold);
    void Think(const Perception&, const StatePtr&);
};

#endif // GOLDMINE_AGENT_H_