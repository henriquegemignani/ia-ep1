#include "agent.h"
#include "environment.h"
#include "errors.h"

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <cassert>

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

void openFile(const std::string& filename, MapMatrix& matrix, std::set<Position>& gold_locations) {
    std::ifstream input(filename);
    if(!input.is_open())
        throw input_error("Unable to open file '" + filename + "'.");
    
    matrix.clear();
    gold_locations.clear();
    
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
    
    matrix.resize(size);
    
    for (int j = 0; j < matrix.size(); ++j) {
        auto& matrix_line = matrix[j];

        getline(input, line);
        if(line.size() != matrix_line.size())
            throw input_error("'" + line + "' size doesn't match declared size.");
            
        for (std::string::size_type i = 0; i < line.size(); ++i) {
            switch (line[i]) {
                case '*':
                    gold_locations.emplace(static_cast<int>(i),
                                           static_cast<int>(j));
                    // No break on purpose.
                case '0':
                    matrix_line[i] = false; break;
                case '1':
                    matrix_line[i] = true; break;
                default:
                    throw input_error(std::string("Unknown character '") + line[i] + "' on line ");
            }
        }
    }
    if (static_cast<int>(gold_locations.size()) != matrix.size() / 2)
        throw input_error("Wrong ammount of gold locations. Expected " 
                    + std::to_string(matrix.size() / 2)
                    + " got " + std::to_string(gold_locations.size()));
    
    assert(matrix.size() == size);
    
}

int main(int argc, char* argv[])
try {
    if(argc != 3) {
        fprintf(stderr, "Usage: %s <map-file-path> <search-type>\n", argv[0]);
        return EXIT_SUCCESS;
    }
    SearchStrategyType type = ConvertToStrategy(argv[2]);

    Environment env;
    openFile(argv[1], env.data().matrix_, env.data().gold_locations_);

    env.set_agent(std::unique_ptr<Agent>(new Agent(type)));

    State result = env.Run();

    printf("%d pontos\n", env.data().CalculateScore(result));

    return 0;
    
} catch(const input_error& err) {
    fprintf(stderr, "Input error: %s\n", err.what());
    return EXIT_FAILURE;
}