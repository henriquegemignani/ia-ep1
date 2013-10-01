#include "agent.h"

#include "actions.h"
#include "environment.h"

namespace {
    StatePtr GoldPicker(const Perception& perception, const StatePtr& state) {
        if (perception.IsValidAction(Action::PICK_GOLD, *state))
            return state->ExecuteAction(Action::PICK_GOLD);
        return StatePtr();
    }

    StatePtr ExitSearcher(const Perception& /*perception*/, const StatePtr& state) {
        if (state->agent_position() == Position(0, 0))
            return state;
        return StatePtr();
    }
}

TargetCheckFunction GetCheckFunction(SearchTarget target) {
    switch (target) {
        case SearchTarget::GOLD: return GoldPicker;
        case SearchTarget::ENTRANCE: return ExitSearcher;
        default:
            return TargetCheckFunction();
    }
}

Agent::Agent(const Strategy& strategy) : strategy_(strategy) {}

Action Agent::CalculateNextAction(const Perception& perception, const std::shared_ptr<const State>& current_state) {
    if(next_actions_.empty())
        Think(perception, current_state);

    Action result = next_actions_.front();
    next_actions_.pop();
    return result;
}

StatePtr Agent::SearchForFixedGold(const Perception& perception, const StatePtr& initial_state, size_t num_gold) {
    StatePtr result = initial_state;
    for (; num_gold > 0; --num_gold)
        result = strategy_(perception, result, SearchTarget::GOLD);
    if (!result)
        return result;
    return strategy_(perception, result, SearchTarget::ENTRANCE);
}

void Agent::Think(const Perception& perception, const StatePtr& initial_state) {
    std::list<StatePtr> gold_routes;

    // Doing nothing is a valid strategy and may be the optimal one.
    gold_routes.emplace_back(initial_state);

    // Try to pick each gold quantity;
    for (size_t num_gold_fetched = 1; num_gold_fetched <= perception.gold_locations_.size(); ++num_gold_fetched) {
        StatePtr route = SearchForFixedGold(perception, initial_state, num_gold_fetched);
        if (route)
            gold_routes.emplace_back(route);
        else
            break;
    }

    auto best = gold_routes.begin();
    for (auto sol = gold_routes.begin(); sol != gold_routes.end(); ++sol)
    if (perception.CalculateScore(**sol) > perception.CalculateScore(**best))
        best = sol;

    for (Action a : (*best)->CreateActionList())
        next_actions_.push(a);
    next_actions_.push(Action::DONE);
}
