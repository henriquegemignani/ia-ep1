#include "agent.h"

#include "actions.h"
#include "errors.h"
#include "environment.h"

#include <map>
#include <list>    
#include <utility>

namespace {
auto action_list = { Action::MOVE_DOWN, Action::MOVE_RIGHT, Action::MOVE_LEFT, Action::MOVE_UP };
typedef std::set<Position> VisitSet;

StatePtr RecursiveLimitedDepthSearch(const Perception& perception, const StatePtr& s, const ResultCheck& checker, const VisitSet& visit, size_t max_depth) {

    VisitSet my_visit(visit);
    my_visit.insert(s->agent_position_);

    if (my_visit.size() > max_depth)
        return StatePtr();

    if (StatePtr result = checker(perception, s))
        return result;

    for (Action a : action_list) {
        // If we can do this action
        if (perception.IsValidAction(a, *s)) {
            // Do it
            StatePtr new_state = s->ExecuteAction(a);

            // If we haven't visited this place, queue it.
            if (my_visit.find(new_state->agent_position_) == my_visit.end()) {
                StatePtr recursive = RecursiveLimitedDepthSearch(perception, new_state, checker, my_visit, max_depth);
                if (recursive)
                    return recursive;
            }
        }
    }
    return StatePtr();
}

}

StatePtr LimitedDepthFirstStrategy(const Perception& perception, const StatePtr& initial_state, const ResultCheck& checker) {
    for (size_t max_depth = 1; true; ++max_depth) {
        VisitSet visit;
        StatePtr search = RecursiveLimitedDepthSearch(perception, initial_state, checker, visit, max_depth);
        if (search)
            return search;
    }
    return initial_state;
}
