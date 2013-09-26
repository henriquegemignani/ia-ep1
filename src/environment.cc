#include "environment.h"

#include "agent.h"

Environment::Environment() {}

Environment::~Environment() {}

void Environment::set_agent(std::unique_ptr<Agent>&& agent) {
    agent_ = std::move(agent);
}

bool Environment::IsValidAction(Action a) {
    return false;
}