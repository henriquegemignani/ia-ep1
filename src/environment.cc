#include "environment.h"

#include "agent.h"

Environment::Environment() {}

Environment::~Environment() {}

void Environment::AddAgent(std::unique_ptr<Agent>&& agent) {
    agents_.push_back(std::move(agent));
}