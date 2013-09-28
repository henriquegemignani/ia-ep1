#include "agent.h"

#include "actions.h"
#include "errors.h"
#include "environment.h"

#include <map>
#include <list>    
#include <utility>

namespace {
auto action_list = { Action::MOVE_DOWN, Action::MOVE_RIGHT, Action::MOVE_LEFT, Action::MOVE_UP };
}

StatePtr LimitedDepthFirstStrategy(const Perception&, const StatePtr& initial_state, const ResultCheck&) {
    return initial_state;
}
