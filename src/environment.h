#ifndef GOLDMINE_ENVIRONMENT_H_
#define GOLDMINE_ENVIRONMENT_H_

#include "forward_declarations.h"
#include "actions.h"
#include "mapmatrix.h"

#include <vector>
#include <list>
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
        return x == right.x && y == right.y;
    }

    int x, y;
};

template<class T, class Container>
inline bool is_in(const Container& s, const T& v) {
    return s.find(v) != s.end();
}

struct Perception {
    Perception() = default;
    ~Perception() = default;

    Perception(const Perception&) = delete;           
    Perception& operator=(const Perception&) = delete;

    Perception(Perception&& r)
        : matrix_(std::move(r.matrix_))
        , gold_locations_(std::move(r.gold_locations_)) {}
    Perception& operator=(Perception&&) = delete;

    MapMatrix matrix_;
    std::set<Position> gold_locations_;
    
    int CalculateScore(const State&) const;
    bool IsValidAction(Action, const State&) const;
};

class State : public std::enable_shared_from_this<State> {
public:
    State(const std::set<Position>& gold_locations) 
        : next_action_(Action::DONE)
        , size_(0)
        , available_gold_(new std::set<Position>(gold_locations)) {}
    ~State() = default;

    State(const State&) = default;
    State& operator=(const State&) = default;

    State(State&& r) throw()
        : previous_(std::move(r.previous_))
        , next_action_(r.next_action_)
        , available_gold_(std::move(r.available_gold_))
        , agent_position_(r.agent_position_) {}
    State& operator=(State&&) = delete;

    StatePtr ExecuteAction(Action) const;
    std::list<Action> CreateActionList() const;

    const StatePtr& previous() const { return previous_; }
    Action next_action() const { return next_action_; }
    int size() const { return size_; }
    const std::shared_ptr<const std::set<Position>>& available_gold() const { return available_gold_; }
    const Position& agent_position() const { return agent_position_; }

  private:
    StatePtr previous_;
    Action next_action_;
    int size_;
    std::shared_ptr<const std::set<Position>> available_gold_;
    Position agent_position_;

};

class Environment {
  public:
    struct Results {
      Results(std::list<Action>&& a, int s) : actions(a), score(s) {}
      Results(Results&& r) : actions(std::move(r.actions)), score(r.score) {}

      std::list<Action> actions;
      int score;
    };

    Environment();
    ~Environment();

    Environment(const Environment&) = delete;
    Environment& operator= (const Environment&) = delete;

    Environment(Environment&& r)
        : data_(std::move(r.data_))
        , current_state_(std::move(r.current_state_))
        , agent_(std::move(r.agent_)) {}
    Environment& operator= (Environment&&) = delete;
    
    // Sets the used agent.
    void set_agent(std::unique_ptr<Agent>&& agent);

    // Returns the data.
    Perception& data() { return data_; }
    
    // Execute the simulation.
    Results Run();
    
  private:
    // Store the data in the perception object for simplicity.
    Perception data_;

    StatePtr current_state_;

    std::unique_ptr<Agent> agent_;
};

#endif // GOLDMINE_ENVIRONMENT_H_