#ifndef GOLDMINE_ENVIRONMENT_H_
#define GOLDMINE_ENVIRONMENT_H_

#include "forward_declarations.h"
#include "actions.h"

#include <vector>
#include <memory>
#include <utility>
#include <set>

typedef std::vector<std::vector<bool>> MapMatrix;

struct Position {
    Position() : x(0), y(0) {}
    Position(int _x, int _y) : x(_x), y(_y) {}

    bool operator< (const Position& right) const {
        return (x < right.x) || (x == right.x && y < right.y);
    }

    int x, y;
};

struct Perception {
    MapMatrix matrix_;
    std::set<Position> gold_locations_;
    std::vector<Action> actions_;
    Position agent_position_;
};

class Environment {
  public:
    Environment();
    ~Environment();
    
    void Run();

    void set_agent(std::unique_ptr<Agent>&& agent);
    bool IsValidAction(Action) const;
    int CalculateScore() const;

             MapMatrix& matrix()         { return data_.matrix_; }
    std::set<Position>& gold_locations() { return data_.gold_locations_; }
    
  private:
    // Store the data in the perception object for simplicity.
    Perception data_;

    std::unique_ptr<Agent> agent_;
    

    void ExecuteAction(Action);
    
};

#endif // GOLDMINE_ENVIRONMENT_H_