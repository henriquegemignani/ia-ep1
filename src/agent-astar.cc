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

    int entrance_h_func(const StatePtr& state) {
        return distance(state->agent_position(), Position(0, 0));
    }

    int gold_h_func(const StatePtr& state) {
        int dist = std::numeric_limits<int>::max();
        for (const Position& gold_pos : *state->available_gold()) {
            dist = std::min(dist, distance(gold_pos, state->agent_position()));
        }
        return dist;
    }

    struct QueueItem {
        QueueItem(const StatePtr& s, int _f)
            : state(std::move(s)), f_value(_f) {}

        StatePtr state;
        int f_value;

        bool operator<(const QueueItem& r) const {
            // priority_queue picks the highest values first, so we must invert the order
            return this->f_value > r.f_value;
        }
    };
}

StatePtr AStarStrategy(const Perception& perception, const StatePtr& initial_state, SearchTarget target) {
    std::map<Position, int> g_score;
    auto g = [&g_score](const StatePtr& s) -> int& { return g_score[s->agent_position()]; };
    auto h = (target == SearchTarget::GOLD)
        ? gold_h_func
        : entrance_h_func;

    g(initial_state) = 0;

    std::priority_queue<QueueItem> q;
    q.emplace(initial_state, g(initial_state) + h(initial_state));

    while (!q.empty()) {
        QueueItem s = q.top();

        if (auto result = GetCheckFunction(target)(perception, s.state))
            return result;

        q.pop();

        // For every possible action
        for (Action a : action_list) {
            // If we can do this action
            if (!perception.IsValidAction(a, *s.state))
                continue;
            StatePtr new_state = s.state->ExecuteAction(a);

            int tentative_g = g(s.state) + 1;

            if (is_in(g_score, new_state->agent_position()) && tentative_g >= g(new_state))
                continue;

            g(new_state) = tentative_g;
            q.emplace(new_state, g(new_state) + h(new_state));
        }
    }
    return StatePtr();
}
