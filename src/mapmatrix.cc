#include "mapmatrix.h"

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
