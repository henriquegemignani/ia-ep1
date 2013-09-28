#include "agent.h"

#include "actions.h"
#include "errors.h"
#include "environment.h"

#include <map>
    
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


static auto action_list = { Action::PICK_GOLD, Action::MOVE_DOWN, Action::MOVE_RIGHT,
        Action::MOVE_LEFT, Action::MOVE_UP };

std::queue<Action> BreadthFirstStrategy(const Perception& perception,
                                        const State& initial_state) {
    std::queue<State> q;
    std::map<Position, State> best_scores;

    q.push(initial_state);

    static int count = 0;

    while (!q.empty()) {
        State s = q.front();
        q.pop();

        int new_score = perception.CalculateScore(s);
        auto previous_score = best_scores.find(s.agent_position_);
        if (previous_score != best_scores.end() && 
            perception.CalculateScore(previous_score->second) > new_score)
            continue;
        best_scores[s.agent_position_] = s;

        dump_path(s.actions_);
        printf(" --> %d\n", new_score);

        if (++count == 10) {
            int i;
            scanf("%d", &i);
            count = 0;
        }

        for (Action a : action_list) {
            if (perception.IsValidAction(a, s)) {
                State new_state = s.ExecuteAction(a);
                q.push(std::move(new_state));
            }
        }
    }

    puts("DA BEST?");
    dump_path(best_scores[Position(0, 0)].actions_);
    printf(" --> %d\n", perception.CalculateScore(best_scores[Position(0, 0)]));

    std::queue<Action> result;
    for (Action a : best_scores[Position(0, 0)].actions_)
        result.push(a);
    result.push(Action::DONE);
    return result;
}

Strategy findStrategy(SearchStrategyType type) {
    switch(type) {
    case SearchStrategyType::BREADTH_FIRST: return BreadthFirstStrategy;
    default: throw input_error("Strategy not yet implemented.");
    }
}

Agent::Agent(SearchStrategyType type) : strategy_(findStrategy(type)) {}

Action Agent::CalculateNextAction(const Perception& perception, const State& current_state) {
    if(next_actions_.empty())
        next_actions_ = strategy_(perception, current_state);

    Action result = next_actions_.front();
    next_actions_.pop();
    return result;
}