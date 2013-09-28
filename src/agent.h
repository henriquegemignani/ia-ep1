#ifndef GOLDMINE_AGENT_H_
#define GOLDMINE_AGENT_H_

#include "forward_declarations.h"
#include "actions.h"
#include <queue>
#include <functional>
#include <memory>

std::queue<Action> BreadthFirstStrategy(const Perception&, const std::shared_ptr<const State>&);
std::queue<Action> LimitedDepthFirstStrategy(const Perception&, const std::shared_ptr<const State>&);

typedef std::function<std::queue<Action>(const Perception&, const std::shared_ptr<const State>&)> Strategy;

class Agent {
  public:
    Agent(const Strategy& strategy);
    
    Action CalculateNextAction(const Perception& perception, const std::shared_ptr<const State>& current_state);
    
  private:
    Strategy strategy_;
    std::queue<Action> next_actions_;
};

#endif // GOLDMINE_AGENT_H_