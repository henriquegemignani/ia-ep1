#ifndef GOLDMINE_ENVIRONMENT_H_
#define GOLDMINE_ENVIRONMENT_H_

#include <vector>
#include <memory>

enum class MatrixItem {
    INVALID,
    EMPTY,
    OBSTACLE,
    GOLD
};

typedef std::vector<std::vector<MatrixItem>> MapMatrix;

class Agent;
class Environment {
  public:
    Environment();
    ~Environment();

    void AddAgent(std::unique_ptr<Agent>&& agent);

    const MapMatrix& matrix() const { return matrix_; }
          MapMatrix& matrix()       { return matrix_; }
    
  private:
    MapMatrix matrix_;
    std::vector<std::unique_ptr<Agent>> agents_;
    
};

#endif // GOLDMINE_ENVIRONMENT_H_