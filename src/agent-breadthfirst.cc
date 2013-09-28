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

void dump_path(const std::list<Action>& path) {
    for (Action a : path)
        printf("%c ", to_str(a));
}

auto action_list = { Action::PICK_GOLD, Action::MOVE_DOWN, Action::MOVE_RIGHT,
    Action::MOVE_LEFT, Action::MOVE_UP };

struct QueueItem {
    QueueItem(std::shared_ptr<const State>&& s, const std::map<Position, int>& best)
        : current_state(std::move(s)), scores(best) {}

    std::shared_ptr<const State> current_state;
    std::map<Position, int> scores;
};

bool is_better(const Perception& perception, const State& state, const std::map<Position, int>& best) {
    auto b = best.find(state.agent_position_);
    return (b == best.end() || b->second < perception.CalculateScore(state));
}

}

std::queue<Action> BreadthFirstStrategy(const Perception& perception, const std::shared_ptr<const State>& initial_state) {
    std::pair<std::shared_ptr<const State>, int> best_solution(initial_state, 0);

    std::queue<QueueItem> q;
    q.emplace(std::shared_ptr<const State>(initial_state), std::map<Position, int>());

    while (!q.empty()) {
        QueueItem qi = q.front();
        q.pop();

        std::shared_ptr<const State>& s = qi.current_state;
        int s_score = perception.CalculateScore(*s);

        // Is this a solution, and is it better than the best solution?
        if (s->agent_position_ == Position(0, 0) && s_score > best_solution.second) {
            best_solution.first = s;
            best_solution.second = s_score;
        }

        qi.scores[s->agent_position_] = s_score;

        // For every possible action
        for (Action a : action_list) {
            // If we can do this action
            if (perception.IsValidAction(a, *s)) {
                // Do it
                std::shared_ptr<const State> new_state = s->ExecuteAction(a);

                // Then check if we haven't reached this place with a better score.
                if (is_better(perception, *new_state, qi.scores))
                    q.emplace(std::move(new_state), qi.scores);
            }
        }
    }

    std::list<Action> best_path = best_solution.first->CreateActionList();

    puts("BEST SOLUTION:");
    dump_path(best_path);
    printf(" --> %d\n", best_solution.second);

    std::queue<Action> result;
    for (Action a : best_path)
        result.push(a);
    result.push(Action::DONE);
    return result;
}
