#ifndef GOLDMINE_INPUT_H_
#define GOLDMINE_INPUT_H_

#include "forward_declarations.h"

#include <string>

Environment BuildEnvironment(const std::string& mine_file);
std::unique_ptr<Agent> BuildAgent(const std::string& strategy_name);

#endif // GOLDMINE_INPUT_H_