#include "agent.h"

#include "actions.h"
#include "errors.h"
#include "environment.h"

#include <map>
#include <list>    
#include <utility>

namespace {

auto action_list = { Action::MOVE_DOWN, Action::MOVE_RIGHT, Action::MOVE_LEFT, Action::MOVE_UP };

StatePtr GoldPicker(const Perception& perception, const StatePtr& state) {
    if (perception.IsValidAction(Action::PICK_GOLD, *state))
        return state->ExecuteAction(Action::PICK_GOLD);
    return StatePtr();
}

StatePtr ExitSearcher(const Perception& perception, const StatePtr& state) {
    if (state->agent_position_ == Position(0, 0))
        return state;
    return StatePtr();
}

template<class ResultCheck>
StatePtr BreadthSearch(const Perception& perception, const StatePtr& initial_state, const ResultCheck& result_checker) {
    std::queue<StatePtr> q;
    q.emplace(initial_state);

    std::set<Position> visited;

    while (!q.empty()) {
        StatePtr& s = q.front();

        if (auto result = result_checker(perception, s))
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
    throw input_error("Couldn't find route to any gold, but only picked " + std::to_string(initial_state->picked_gold_.size()));
    return StatePtr();
}

std::shared_ptr<const State> BreadthFetchFixedGold(const Perception& perception, const StatePtr& initial_state, size_t num_gold) {
    StatePtr result = initial_state;
    for (; num_gold > 0; --num_gold)
        result = BreadthSearch(perception, result, GoldPicker);
    return BreadthSearch(perception, result, ExitSearcher);
}

}

std::queue<Action> BreadthFirstStrategy(const Perception& perception, const std::shared_ptr<const State>& initial_state) {
    std::list < std::shared_ptr<const State> > gold_routes;

    // Doing nothing is a valid strategy and may be the optimal one.
    gold_routes.emplace_back(initial_state);

    // Try to pick each gold quantity;
    for (size_t num_gold_fetched = 1; num_gold_fetched <= perception.gold_locations_.size(); ++num_gold_fetched) {
        gold_routes.emplace_back(BreadthFetchFixedGold(perception, initial_state, num_gold_fetched));
    }

    auto best = gold_routes.begin();
    for (auto sol = gold_routes.begin(); sol != gold_routes.end(); ++sol)
        if (perception.CalculateScore(**sol) > perception.CalculateScore(**best))
            best = sol;

    std::queue<Action> result;
    for (Action a : (*best)->CreateActionList())
        result.push(a);
    result.push(Action::DONE);
    return result;
}