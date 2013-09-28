#include "agent.h"

#include "actions.h"
#include "errors.h"
#include "environment.h"

#include <map>
#include <list>    
#include <utility>
    
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

std::queue<Action> BreadthFirstStrategy(const Perception& perception, const State& initial_state) {
    std::list<State> solutions;

    std::queue<QueueItem> q;
    q.emplace(State(initial_state), std::map<Position, int>());

    while (!q.empty()) {
        QueueItem qi = q.front();
        q.pop();

        State& s = qi.current_state;
        if (s.agent_position_ == Position(0, 0))
            solutions.push_back(s);

        qi.scores[s.agent_position_] = perception.CalculateScore(s);

        for (Action a : action_list) {
            if (perception.IsValidAction(a, s)) {
                State new_state = s.ExecuteAction(a);
                if (is_worse(perception, new_state, qi.scores))
                    q.emplace(std::move(new_state), qi.scores);
            }
        }
    }

    puts("SOLUTIONS:");
    auto best_solution = solutions.begin();
    for (auto sol = solutions.begin(); sol != solutions.end(); ++sol) {
        if (perception.CalculateScore(*sol) > perception.CalculateScore(*best_solution))
            best_solution = sol;
    }

    puts("FIRST BEST ONE FOUND:");
    dump_path(best_solution->actions_);
    printf(" --> %d\n", perception.CalculateScore(*best_solution));

    std::queue<Action> result;
    for (Action a : best_solution->actions_)
        result.push(a);
    result.push(Action::DONE);
    return result;
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

std::queue<Action> LimitedDepthFirstStrategy(const Perception& perception, const State& initial_state) {
    std::map<Position, int> best;
    auto result_list = RecursiveDepthFirst(perception, initial_state, best);
    std::queue<Action> result;
    for (Action a : result_list)
        result.push(a);
    result.push(Action::DONE);
    return result;
}

Strategy findStrategy(SearchStrategyType type) {
    switch(type) {
    case SearchStrategyType::BREADTH_FIRST: return BreadthFirstStrategy;
    case SearchStrategyType::LIMITED_DEPTH_FIRST: return LimitedDepthFirstStrategy;
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