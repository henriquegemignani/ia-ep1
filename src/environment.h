#ifndef GOLDMINE_ENVIRONMENT_H_
#define GOLDMINE_ENVIRONMENT_H_

#include "forward_declarations.h"
#include "actions.h"
#include "mapmatrix.h"

#include <vector>
#include <memory>
#include <utility>
#include <set>
#include <map>

struct Position {
    Position() : x(0), y(0) {}
    Position(int _x, int _y) : x(_x), y(_y) {}

    bool operator< (const Position& right) const {
        return (x < right.x) || (x == right.x && y < right.y);
    }
    bool operator== (const Position& right) const {
        return !((*this < right) && (right < *this));
    }

    int x, y;
};

template<class T>
inline bool is_in(const std::set<T>& s, const T& v) {
    return s.find(v) != s.end();
}

struct Perception {
    MapMatrix matrix_;
    std::set<Position> gold_locations_;
    
    int CalculateScore(const State&) const;
    bool IsValidAction(Action, const State&) const;
};

struct State {
    State() = default;
    State(const State&) = default;
    State& operator=(const State&) = default;
    State(State&& r)
        : actions_(std::move(r.actions_))
        , picked_gold_(std::move(r.picked_gold_))
        , agent_position_(r.agent_position_) {}


    std::vector<Action> actions_;
    std::set<Position> picked_gold_;
    Position agent_position_;

    State ExecuteAction(Action) const;
};

class Environment {
  public:
    Environment();
    ~Environment();
    
    State Run();

    void set_agent(std::unique_ptr<Agent>&& agent);

    Perception& data() { return data_; }
    
  private:
    // Store the data in the perception object for simplicity.
    Perception data_;

    State current_state_;

    std::unique_ptr<Agent> agent_;
};

#endif // GOLDMINE_ENVIRONMENT_H_