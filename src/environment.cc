#include "environment.h"

#include "agent.h"
#include "errors.h"

MapMatrix::MapMatrix() {}

int MapMatrix::size() const {
    return static_cast<int>(data_.size());
}

void MapMatrix::clear() {
    data_.clear();
}

void MapMatrix::resize(int n) {
    data_.resize(n, std::vector<bool>(n, false));
}

std::vector<bool>& MapMatrix::operator[](int n) {
    return data_[n];
}

bool MapMatrix::operator() (int x, int y) const {
    return data_[y][x];
}

bool MapMatrix::operator() (const Position& pos) const {
    return data_[pos.y][pos.x];
}
    
bool MapMatrix::IsInside(const Position& pos) const {
    return 0 <= pos.x && pos.x < static_cast<int>(data_.size())
        && 0 <= pos.y && pos.y < static_cast<int>(data_.size());
}

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
        case Action::MOVE_DOWN: {
            Position next_pos(current_state_.agent_position_.x,
                              current_state_.agent_position_.y + 1);
            return data_.matrix_.IsInside(next_pos) && !data_.matrix_(next_pos);
        }

        case Action::MOVE_UP: {
            Position next_pos(current_state_.agent_position_.x,
                              current_state_.agent_position_.y - 1);
            return data_.matrix_.IsInside(next_pos) && !data_.matrix_(next_pos);
        }

        case Action::MOVE_RIGHT: {
            Position next_pos(current_state_.agent_position_.x + 1,
                              current_state_.agent_position_.y);
            return data_.matrix_.IsInside(next_pos) && !data_.matrix_(next_pos);
        }

        case Action::MOVE_LEFT: {
            Position next_pos(current_state_.agent_position_.x - 1,
                              current_state_.agent_position_.y);
            return data_.matrix_.IsInside(next_pos) && !data_.matrix_(next_pos);
        }

        case Action::PICK_GOLD: {
            return is_in(data_.gold_locations_, current_state_.agent_position_) &&
                !is_in(current_state_.picked_gold_, current_state_.agent_position_);
        }

        default: {
            return false;
        }
    }
}
    
int Environment::CalculateScore() const {
    int score = 0;
    for (Action a : current_state_.actions_) {
        switch (a) {
            case Action::MOVE_DOWN:
            case Action::MOVE_UP:
            case Action::MOVE_RIGHT:
            case Action::MOVE_LEFT:
                score += -1;
                break;
            case Action::PICK_GOLD:
                score += 4 * data_.matrix_.size();
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
        case Action::MOVE_DOWN:  current_state_.agent_position_.y += 1; break;
        case Action::MOVE_UP:    current_state_.agent_position_.y -= 1; break;
        case Action::MOVE_RIGHT: current_state_.agent_position_.x += 1; break;
        case Action::MOVE_LEFT:  current_state_.agent_position_.x -= 1; break;
        case Action::PICK_GOLD:  current_state_.picked_gold_.insert(current_state_.agent_position_); break;
    }
    current_state_.actions_.push_back(a);
}