#include "agent.h"
#include "environment.h"
#include "errors.h"

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <cassert>

MatrixItem ConvertToMatrixItem(char c) {
    switch(c) {
    case '0': return MatrixItem::EMPTY;
    case '1': return MatrixItem::OBSTACLE;
    case '*': return MatrixItem::GOLD;
    default:
        throw input_error(std::string("Unknown character '") + c + "'");
    }
    return MatrixItem::INVALID;
}

SearchStrategyType ConvertToStrategy(const std::string& identifier) {
    if(identifier.size() == 1) {
        switch(identifier[0]) {
        case 'L': return SearchStrategyType::BREADTH_FIRST;
        case 'P': return SearchStrategyType::LIMITED_DEPTH_FIRST;
        case 'A': return SearchStrategyType::A_STAR;
        default: 
            /* Nothing */;
        }
    }
    throw input_error("Unknown strategy: '" + identifier + "'");
    return SearchStrategyType::INVALID;
}

void openFile(const std::string& filename, MapMatrix& matrix) {
    std::ifstream input(filename);
    if(!input.is_open())
        throw input_error("Unable to open file '" + filename + "'.");
    
    matrix.clear();
    
    std::string line;
    getline(input, line);
    int size;
    try {
        size = stoi(line);
    } catch(const std::invalid_argument&) {
        throw input_error("'" + line + "' is not an int.");
    } catch(const std::out_of_range&) {
        throw input_error("'" + line + "' is out of range, doesn't fit in an int.");
    }
    if(size < 1 || size > 50)
        throw input_error("Invalid size, size must be in range [1, 50].");
    
    matrix.resize(size, std::vector<MatrixItem>(size));
    
    for(auto& matrix_line : matrix) {
        getline(input, line);
        if(line.size() != matrix_line.size())
            throw input_error("'" + line + "' size doesn't match declared size.");
            
        for(std::string::size_type i = 0; i < line.size(); ++i)
            matrix_line[i] = ConvertToMatrixItem(line[i]);
    }
    
    assert(matrix.size() == static_cast<MapMatrix::size_type>(size));
    
}

int main(int argc, char* argv[])
try {
    if(argc != 3) {
        fprintf(stderr, "Usage: %s <map-file-path> <search-type>\n", argv[0]);
        return EXIT_SUCCESS;
    }
    Environment env;
    openFile(argv[1], env.matrix());
    SearchStrategyType type = ConvertToStrategy(argv[2]);

    env.set_agent(std::unique_ptr<Agent>(new Agent(type)));
    return 0;
    
} catch(const input_error& err) {
    fprintf(stderr, "Input error: %s\n", err.what());
    return EXIT_FAILURE;
}