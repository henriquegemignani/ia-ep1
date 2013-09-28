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

struct QueueItem {
    QueueItem(State&& s, const std::map<Position, int>& best)
        : current_state(std::move(s)), scores(best) {}

    State current_state;
    std::map<Position, int> scores;
};

bool is_worse(const Perception& perception, const State& state, const std::map<Position, int>& best) {
    auto b = best.find(state.agent_position_);
    return (b == best.end() || b->second < perception.CalculateScore(state));
}

std::list<Action> RecursiveDepthFirst(const Perception& perception, const State& state, std::map<Position, int> best) {
    if (state.actions_.size() > 30) return std::list<Action>();
    best[state.agent_position_] = perception.CalculateScore(state);
    for (Action a : action_list) {
        if (perception.IsValidAction(a, state)) {
            State new_state = state.ExecuteAction(a);
            if (!is_worse(perception, new_state, best)) {
                auto r = RecursiveDepthFirst(perception, new_state, best);
                if (!r.empty()) {
                    r.push_front(a);
                    return r;
                }
            }
        }
    }
    return std::list<Action>();
}

}

std::queue<Action> LimitedDepthFirstStrategy(const Perception& perception, const State& initial_state) {
    std::map<Position, int> best;
    auto result_list = RecursiveDepthFirst(perception, initial_state, best);
    std::queue<Action> result;
    for (Action a : result_list)
        result.push(a);
    result.push(Action::DONE);
    return result;
}
