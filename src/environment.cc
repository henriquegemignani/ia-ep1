#include "environment.h"

#include "agent.h"
#include "errors.h"

Environment::Environment() {}

Environment::~Environment() {}

void Environment::Run() {
    while (true) {
        Action a = agent_->CalculateNextAction(data_);
        if (a == Action::DONE)
            break;
        ExecuteAction(a);
    }
}

void Environment::set_agent(std::unique_ptr<Agent>&& agent) {
    agent_ = std::move(agent);
}

bool Environment::IsValidAction(Action a) const {
    switch (a) {
    case Action::MOVE_DOWN:
        return (agent_position_.y < static_cast<int>(matrix_.size() - 1) &&
            !matrix_[agent_position_.y + 1][agent_position_.x]);

    case Action::MOVE_UP:
        return (agent_position_.y > 0 &&
            !matrix_[agent_position_.y - 1][agent_position_.x]);

    case Action::MOVE_RIGHT:
        return (agent_position_.x < static_cast<int>(matrix_.size() - 1) &&
            !matrix_[agent_position_.y][agent_position_.x + 1]);

    case Action::MOVE_LEFT:
        return (agent_position_.x > 0 &&
            !matrix_[agent_position_.y][agent_position_.x - 1]);

    case Action::PICK_GOLD:
        return gold_locations_.find(agent_position_) != gold_locations_.end();

    default:
        return false;
    }
}
    
int Environment::CalculateScore() const {
    int score = 0;
    for (Action a : actions_) {
        switch (a) {
            case Action::MOVE_DOWN:
            case Action::MOVE_UP:
            case Action::MOVE_RIGHT:
            case Action::MOVE_LEFT:
                score += -1;
                break;
            case Action::PICK_GOLD:
                score += 4 * static_cast<int>(matrix_.size());
                break;
            default:
                break;
        }
    }
    return score;
}

void Environment::ExecuteAction(Action a) {
    if (!IsValidAction(a))
        throw input_error("Agent requested an invalid action.");
    switch (a) {
    case Action::MOVE_DOWN:  agent_position_.y += 1; break;
    case Action::MOVE_UP:    agent_position_.y -= 1; break;
    case Action::MOVE_RIGHT: agent_position_.x += 1; break;
    case Action::MOVE_LEFT:  agent_position_.x -= 1; break;
    case Action::PICK_GOLD:  gold_locations_.erase(agent_position_); break;
    }
    actions_.push_back(a);
}