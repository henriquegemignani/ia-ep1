#ifndef GOLDMINE_AGENT_H_
#define GOLDMINE_AGENT_H_

enum class SearchStrategyType {
    INVALID,
    BREADTH_FIRST,
    LIMITED_DEPTH_FIRST,
    A_STAR
};

class Agent {
  public:
    Agent(SearchStrategyType type);
    
  private:
    
};

#endif // GOLDMINE_AGENT_H_