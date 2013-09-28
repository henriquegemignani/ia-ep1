#include "agent.h"
#include "environment.h"
#include "errors.h"
#include "input.h"

#include <string>
#include <vector>
#include <fstream>
#include <stdexcept>
#include <cassert>

namespace {
char ActionToChar(Action a) {
    switch (a) {
        case Action::MOVE_LEFT: return 'E';
        case Action::MOVE_UP: return 'C';
        case Action::MOVE_RIGHT: return 'D';
        case Action::MOVE_DOWN: return 'B';
        case Action::PICK_GOLD: return 'P';
        default: break;
    }
    throw input_error("Converting Action with no textual representation.");
    return '\0';
}

void PrintResults(const Environment::Results& results) {
    printf("%d pontos\n", results.score);
    for (Action a : results.actions)
        printf("%c ", ActionToChar(a));
    puts("");
}
}

int main(int argc, char* argv[])
try {
    if(argc != 3) {
        fprintf(stderr, "Usage: %s <map-file-path> <search-type>\n", argv[0]);
        return EXIT_SUCCESS;
    }

    Environment env = BuildEnvironment(argv[1]);
    env.set_agent(BuildAgent(argv[2]));

    auto result = env.Run();
    PrintResults(result);
    return 0;
    
} catch(const input_error& err) {
    fprintf(stderr, "Input error: %s\n", err.what());
    return EXIT_FAILURE;
}
