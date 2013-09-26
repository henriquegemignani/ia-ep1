#include "agent.h"
#include "environment.h"
#include "errors.h"

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>

enum class MatrixItem {
    INVALID,
    EMPTY,
    OBSTACLE,
    GOLD
};

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

typedef std::vector<std::vector<MatrixItem>> MapMatrix;

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
    
    matrix.resize(size, std::vector<MatrixItem>(size));
    
    for(auto& matrix_line : matrix) {
        getline(input, line);
        if(line.size() != matrix_line.size())
            throw input_error("'" + line + "' size doesn't match declared size.");
            
        for(std::string::size_type i = 0; i < line.size(); ++i)
            matrix_line[i] = ConvertToMatrixItem(line[i]);
    }
}

int main(int argc, char** argv)
try {
    if(argc != 3) {
        fprintf(stderr, "Usage: %s <map-file-path> <search-type>\n", argv[0]);
        return EXIT_SUCCESS;
    }
    MapMatrix matrix;
    openFile(argv[1], matrix);
    return 0;
    
} catch(const input_error& err) {
    fprintf(stderr, "Input error: %s\n", err.what());
    return EXIT_FAILURE;
}