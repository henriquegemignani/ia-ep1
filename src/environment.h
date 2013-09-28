#ifndef GOLDMINE_ENVIRONMENT_H_
#define GOLDMINE_ENVIRONMENT_H_

#include "forward_declarations.h"
#include "actions.h"

#include <vector>
#include <memory>
#include <utility>
#include <set>

class MapMatrix {
  public:
    MapMatrix();

    int size() const;
    void clear();
    void resize(int n);

    std::vector<bool>& operator[](int n);

    bool operator() (int x, int y) const;
    bool operator() (const Position&) const;

    bool IsInside(const Position&) const;

  private:
    std::vector<std::vector<bool>> data_;
};

struct Position {
    Position() : x(0), y(0) {}
    Position(int _x, int _y) : x(_x), y(_y) {}

    bool operator< (const Position& right) const {
        return (x < right.x) || (x == right.x && y < right.y);
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
};

struct State {
    std::vector<Action> actions_;
    std::set<Position> picked_gold_;
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

    State current_state_;

    std::unique_ptr<Agent> agent_;
    

    void ExecuteAction(Action);
    
};

#endif // GOLDMINE_ENVIRONMENT_H_