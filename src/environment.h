#ifndef GOLDMINE_ENVIRONMENT_H_
#define GOLDMINE_ENVIRONMENT_H_

#include "forward_declarations.h"
#include "actions.h"

#include <vector>
#include <memory>
#include <utility>

enum class MatrixItem {
    INVALID,
    EMPTY,
    OBSTACLE,
    GOLD
};

typedef std::vector<std::vector<MatrixItem>> MapMatrix;

struct Position {
    Position() : x(0), y(0) {}
    int x, y;
};

struct Perception {
    Perception(const MapMatrix& m, const std::vector<Action>& a, const Position& p)
        : matrix(m), actions(a), agent_position(p) {}

    const MapMatrix& matrix;
    const std::vector<Action>& actions;
    const Position& agent_position;
};

class Environment {
  public:
    Environment();
    ~Environment();
    
    void Run();

    void set_agent(std::unique_ptr<Agent>&& agent);
    bool IsValidAction(Action) const;
    int CalculateScore() const;

    const MapMatrix& matrix() const { return matrix_; }
          MapMatrix& matrix()       { return matrix_; }
    
  private:
    MapMatrix matrix_;
    std::vector<Action> actions_;
    std::unique_ptr<Agent> agent_;
    Position agent_position_;

    void ExecuteAction(Action);
    
};

#endif // GOLDMINE_ENVIRONMENT_H_