#include "environment.h"

#include "agent.h"
#include "errors.h"

#include <cassert>
    
int Perception::CalculateScore(const State& state) const {
    int num_gold = static_cast<int>(gold_locations_.size() - state.available_gold()->size());
    int num_steps = state.size() - num_gold;
    int score = num_gold * 4 * matrix_.size() - num_steps;
    return score;
}

bool Perception::IsValidAction(Action a, const State& state) const {
    switch (a) {
        case Action::MOVE_DOWN: {
            Position next_pos(state.agent_position().x,
                              state.agent_position().y + 1);
            return matrix_.IsInside(next_pos) && !matrix_(next_pos);
        }

        case Action::MOVE_UP: {
            Position next_pos(state.agent_position().x,
                              state.agent_position().y - 1);
            return matrix_.IsInside(next_pos) && !matrix_(next_pos);
        }

        case Action::MOVE_RIGHT: {
            Position next_pos(state.agent_position().x + 1,
                              state.agent_position().y);
            return matrix_.IsInside(next_pos) && !matrix_(next_pos);
        }

        case Action::MOVE_LEFT: {
            Position next_pos(state.agent_position().x - 1,
                              state.agent_position().y);
            return matrix_.IsInside(next_pos) && !matrix_(next_pos);
        }

        case Action::PICK_GOLD: {
            return is_in(gold_locations_, state.agent_position()) &&
                   is_in(*state.available_gold(), state.agent_position());
        }

        default: {
            return false;
        }
    }
}

std::shared_ptr<const State> State::ExecuteAction(Action a) const {
    std::shared_ptr<State> result(new State(*this));
    result->previous_ = shared_from_this();

    switch (a) {
        case Action::MOVE_DOWN:  result->agent_position_.y += 1; break;
        case Action::MOVE_UP:    result->agent_position_.y -= 1; break;
        case Action::MOVE_RIGHT: result->agent_position_.x += 1; break;
        case Action::MOVE_LEFT:  result->agent_position_.x -= 1; break;
        case Action::PICK_GOLD: {
            auto new_set = std::make_shared<std::set<Position>>(*this->available_gold_);
            new_set->erase(agent_position_);
            result->available_gold_ = new_set;
            break;
        }
        default: assert(false);
    }
    result->next_action_ = a;
    ++result->size_;
    return result;
}
    
namespace {
void to_action_list(const State& state, std::list<Action>& result) {
    if (state.previous())
        to_action_list(*state.previous(), result);
    if (state.next_action() != Action::DONE)
        result.push_back(state.next_action());
}
}

std::list<Action> State::CreateActionList() const {
    std::list<Action> result;
    to_action_list(*this, result);
    return result;
}

Environment::Environment() {}

Environment::~Environment() {}

Environment::Results Environment::Run() {
    current_state_ = std::make_shared<State>(data_.gold_locations_);
    while (true) {
        Action a = agent_->CalculateNextAction(data_, current_state_);
        if (a == Action::DONE)
            break;
        if (!data_.IsValidAction(a, *current_state_))
            throw input_error("Agent requested invalid action.");
        current_state_ = current_state_->ExecuteAction(a);
    }
    return Results(current_state_->CreateActionList(), data_.CalculateScore(*current_state_));
}

void Environment::set_agent(std::unique_ptr<Agent>&& agent) {
    agent_ = std::move(agent);
}
