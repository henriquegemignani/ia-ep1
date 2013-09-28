#ifndef GOLDMINE_FORWARDDECLARATIONS_H_
#define GOLDMINE_FORWARDDECLARATIONS_H_

#include <memory>

enum class Action;
enum class SearchStrategyType;

class Agent;
class Environment;
class MapMatrix;
struct Perception;
struct Position;

struct State;
typedef std::shared_ptr<const State> StatePtr;


#endif // GOLDMINE_FORWARDDECLARATIONS_H_