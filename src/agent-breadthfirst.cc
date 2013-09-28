#include "agent.h"

#include "actions.h"
#include "errors.h"
#include "environment.h"

#include <map>
#include <list>    
#include <utility>

namespace {

char to_str(Action a) {
    switch (a) {
        case Action::MOVE_LEFT: return 'E';
        case Action::MOVE_UP: return 'C';
        case Action::MOVE_RIGHT: return 'D';
        case Action::MOVE_DOWN: return 'B';
        case Action::PICK_GOLD: return 'P';
        default:
            throw input_error("imprimindo action invalido");
    }

}

void dump_path(const std::vector<Action>& path) {
    for (Action a : path)
        printf("%c ", to_str(a));
}

auto action_list = { Action::PICK_GOLD, Action::MOVE_DOWN, Action::MOVE_RIGHT,
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

}

std::queue<Action> BreadthFirstStrategy(const Perception& perception, const State& initial_state) {
    std::pair<State, int> best_solution(initial_state, 0);

    std::queue<QueueItem> q;
    q.emplace(State(initial_state), std::map<Position, int>());

    while (!q.empty()) {
        QueueItem qi = q.front();
        q.pop();

        State& s = qi.current_state;
        int s_score = perception.CalculateScore(s);

        if (s.agent_position_ == Position(0, 0) && s_score > best_solution.second) {
            best_solution.first = s;
            best_solution.second = s_score;
        }

        qi.scores[s.agent_position_] = perception.CalculateScore(s);

        for (Action a : action_list) {
            if (perception.IsValidAction(a, s)) {
                State new_state = s.ExecuteAction(a);
                if (is_worse(perception, new_state, qi.scores))
                    q.emplace(std::move(new_state), qi.scores);
            }
        }
    }

    puts("BEST SOLUTION:");
    dump_path(best_solution.first.actions_);
    printf(" --> %d\n", best_solution.second);

    std::queue<Action> result;
    for (Action a : best_solution.first.actions_)
        result.push(a);
    result.push(Action::DONE);
    return result;
}
