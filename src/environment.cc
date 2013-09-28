#include "environment.h"

#include "agent.h"
#include "errors.h"

Environment::Environment() {}

Environment::~Environment() {}

void Environment::Run() {
    while (true) {
        Action a = agent_->CalculateNextAction(Perception(matrix_, actions_, agent_position_));
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
            matrix_[agent_position_.y + 1][agent_position_.x] != MatrixItem::OBSTACLE);

    case Action::MOVE_UP:
        return (agent_position_.y > 0 &&
            matrix_[agent_position_.y - 1][agent_position_.x] != MatrixItem::OBSTACLE);

    case Action::MOVE_RIGHT:
        return (agent_position_.x < static_cast<int>(matrix_.size() - 1) &&
            matrix_[agent_position_.y][agent_position_.x + 1] != MatrixItem::OBSTACLE);

    case Action::MOVE_LEFT:
        return (agent_position_.x > 0 &&
            matrix_[agent_position_.y][agent_position_.x - 1] != MatrixItem::OBSTACLE);

    case Action::PICK_GOLD:
        return matrix_[agent_position_.y][agent_position_.x] == MatrixItem::GOLD;

    default:
        return false;
    }
}

void Environment::ExecuteAction(Action a) {
    if (!IsValidAction(a))
        throw input_error("Agent requested an invalid action.");
    switch (a) {
    case Action::MOVE_DOWN:  agent_position_.y += 1; break;
    case Action::MOVE_UP:    agent_position_.y -= 1; break;
    case Action::MOVE_RIGHT: agent_position_.x += 1; break;
    case Action::MOVE_LEFT:  agent_position_.x -= 1; break;
    case Action::PICK_GOLD:  matrix_[agent_position_.y][agent_position_.x] = MatrixItem::EMPTY; break;
    }
    actions_.push_back(a);
}