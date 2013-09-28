#include "environment.h"

#include "agent.h"
#include "errors.h"

#include <cassert>
    
int Perception::CalculateScore(const State& state) const {
    int score = 0;
    for (Action a : state.actions_) {
        switch (a) {
            case Action::MOVE_DOWN:
            case Action::MOVE_UP:
            case Action::MOVE_RIGHT:
            case Action::MOVE_LEFT:
                score += -1;
                break;
            case Action::PICK_GOLD:
                score += 4 * matrix_.size();
                break;
            default:
                break;
        }
    }
    return score;
}

bool Perception::IsValidAction(Action a, const State& state) const {
    switch (a) {
        case Action::MOVE_DOWN: {
            Position next_pos(state.agent_position_.x,
                              state.agent_position_.y + 1);
            return matrix_.IsInside(next_pos) && !matrix_(next_pos);
        }

        case Action::MOVE_UP: {
            Position next_pos(state.agent_position_.x,
                              state.agent_position_.y - 1);
            return matrix_.IsInside(next_pos) && !matrix_(next_pos);
        }

        case Action::MOVE_RIGHT: {
            Position next_pos(state.agent_position_.x + 1,
                              state.agent_position_.y);
            return matrix_.IsInside(next_pos) && !matrix_(next_pos);
        }

        case Action::MOVE_LEFT: {
            Position next_pos(state.agent_position_.x - 1,
                              state.agent_position_.y);
            return matrix_.IsInside(next_pos) && !matrix_(next_pos);
        }

        case Action::PICK_GOLD: {
            return is_in(gold_locations_, state.agent_position_) &&
                !is_in(state.picked_gold_, state.agent_position_);
        }

        default: {
            return false;
        }
    }
}

State State::ExecuteAction(Action a) const {
    State result(*this);

    switch (a) {
        case Action::MOVE_DOWN:  result.agent_position_.y += 1; break;
        case Action::MOVE_UP:    result.agent_position_.y -= 1; break;
        case Action::MOVE_RIGHT: result.agent_position_.x += 1; break;
        case Action::MOVE_LEFT:  result.agent_position_.x -= 1; break;
        case Action::PICK_GOLD:  result.picked_gold_.insert(agent_position_); break;
        default: assert(false);
    }
    result.actions_.push_back(a);
    return result;
}

Environment::Environment() {}

Environment::~Environment() {}

State Environment::Run() {
    while (true) {
        Action a = agent_->CalculateNextAction(data_, current_state_);
        if (a == Action::DONE)
            break;
        if (!data_.IsValidAction(a, current_state_))
            throw input_error("Agent requested invalid action.");
        current_state_ = current_state_.ExecuteAction(a);
    }
    return current_state_;
}

void Environment::set_agent(std::unique_ptr<Agent>&& agent) {
    agent_ = std::move(agent);
}
