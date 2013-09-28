#include "agent.h"

#include "actions.h"
#include "errors.h"
#include "environment.h"

#include <map>
#include <list>    
#include <utility>

namespace {

static auto action_list = { Action::PICK_GOLD, Action::MOVE_DOWN, Action::MOVE_RIGHT,
    Action::MOVE_LEFT, Action::MOVE_UP };

}

std::queue<Action> LimitedDepthFirstStrategy(const Perception& perception, const std::shared_ptr<const State>& initial_state) {
    std::queue<Action> result;
    result.push(Action::DONE);
    return result;
}
