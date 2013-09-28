#ifndef GOLDMINE_MAPMATRIX_H_
#define GOLDMINE_MAPMATRIX_H_

#include "forward_declarations.h"

#include <vector>

class MapMatrix {
  public:
    MapMatrix();

    int size() const;
    void clear();
    void resize(int n);

    std::vector<bool>& operator[](int n);

    bool operator() (int x, int y) const;
    bool operator() (const Position&) const;

    bool IsInside(const Position&) const;

  private:
    std::vector<std::vector<bool>> data_;
};

#endif // GOLDMINE_MAPMATRIX_H_