#ifndef GOLDMINE_ENVIRONMENT_H_
#define GOLDMINE_ENVIRONMENT_H_

#include "forward_declarations.h"

#include <vector>
#include <memory>

enum class MatrixItem {
    INVALID,
    EMPTY,
    OBSTACLE,
    GOLD
};

typedef std::vector<std::vector<MatrixItem>> MapMatrix;

class Environment {
  public:
    Environment();
    ~Environment();

    void set_agent(std::unique_ptr<Agent>&& agent);
    bool IsValidAction(Action);

    const MapMatrix& matrix() const { return matrix_; }
          MapMatrix& matrix()       { return matrix_; }
    
  private:
    MapMatrix matrix_;
    std::unique_ptr<Agent> agent_;
    
};

#endif // GOLDMINE_ENVIRONMENT_H_