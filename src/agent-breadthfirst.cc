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

StatePtr BreadthFirstStrategy(const Perception& perception, const StatePtr& initial_state, SearchTarget target) {
    std::queue<StatePtr> q;
    q.emplace(initial_state);

    std::set<Position> visited;

    while (!q.empty()) {
        StatePtr& s = q.front();

        if (auto result = GetCheckFunction(target)(perception, s))
            return result;

        visited.insert(s->agent_position_);

        // For every possible action
        for (Action a : action_list) {
            // If we can do this action
            if (perception.IsValidAction(a, *s)) {
                // Do it
                StatePtr new_state = s->ExecuteAction(a);

                // If we haven't visited this place, queue it.
                if (visited.find(new_state->agent_position_) == visited.end())
                    q.emplace(new_state);
            }
        }

        q.pop();
    }
    return StatePtr();
}
