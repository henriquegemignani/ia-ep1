#include "agent.h"

#include "actions.h"
#include "errors.h"
#include "environment.h"

#include <map>
#include <list>    
#include <utility>
#include <queue>
#include <limits>

namespace {
    auto action_list = { Action::MOVE_DOWN, Action::MOVE_RIGHT, Action::MOVE_LEFT, Action::MOVE_UP };

    int distance(const Position& l, const Position& r) {
        return std::abs(l.x - r.x) + std::abs(l.y - r.y);
    }

    int entrance_h_func(const Perception& perception, const StatePtr& state) {
        return distance(state->agent_position_, Position(0, 0));
    }

    int gold_h_func(const Perception& perception, const StatePtr& state) {
        int dist = std::numeric_limits<int>::max();
        for (const Position& gold_pos : perception.gold_locations_) {
            if (!is_in(state->picked_gold_, gold_pos)) {
                dist = std::min(dist, distance(gold_pos, state->agent_position_));
            }
        }
        return dist;
    }

    struct QueueItem {
        QueueItem(const StatePtr& s, int _f)
            : state(std::move(s)), f_value(_f) {}

        StatePtr state;
        int f_value;

        bool operator<(const QueueItem& r) const {
            return this->f_value < r.f_value;
        }
    };
}

StatePtr AStarStrategy(const Perception& perception, const StatePtr& initial_state, SearchTarget target) {
    std::map<const State*, int> g_score;
    auto g = [&g_score](const StatePtr& s) -> int& { return g_score[s.get()]; };
    std::function<int(const StatePtr&)> h;
    if (target == SearchTarget::GOLD)
        h = [&perception](const StatePtr& s) -> int { return gold_h_func(perception, s); };
    else
        h = [&perception](const StatePtr& s) -> int { return entrance_h_func(perception, s); };

    g(initial_state) = 0;

    std::priority_queue<QueueItem> q;
    q.emplace(initial_state, g(initial_state) + h(initial_state));

    std::set<Position> visited;

    while (!q.empty()) {
        QueueItem s = q.top();

        if (auto result = GetCheckFunction(target)(perception, s.state))
            return result;

        q.pop();
        visited.insert(s.state->agent_position_);

        // For every possible action
        for (Action a : action_list) {
            // If we can do this action
            if (!perception.IsValidAction(a, *s.state))
                continue;
            StatePtr new_state = s.state->ExecuteAction(a);

            int tentative_g = g(s.state) + 1; // 1 == distance between s.state and new_state

            // If we haven't visited this place, queue it.
            if (is_in(visited, new_state->agent_position_) && tentative_g >= g(s.state))
                continue;

            g(new_state) = tentative_g;
            q.emplace(new_state, g(new_state) + h(new_state));
        }
    }
    return StatePtr();
}
