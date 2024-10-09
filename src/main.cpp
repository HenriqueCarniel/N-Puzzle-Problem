#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include "NPuzzle.h"

enum SearchAlgorithm
{
    BFS, IDFS, ASTAR, IDASTAR, GBFS, UNKNOWN
};

SearchAlgorithm selectAlgorithm(const std::string& input)
{
    if (input == "-bfs") return BFS;
    if (input == "-idfs") return IDFS;
    if (input == "-astar") return ASTAR;
    if (input == "-idastar") return IDASTAR;
    if (input == "-gbfs") return GBFS;
    return UNKNOWN;
}

std::vector<int> parseState(const std::string& state)
{
    std::vector<int> parsedState;
    std::stringstream ss(state);
    int value;

    while (ss >> value)
    {
        parsedState.push_back(value);
        if (ss.peek() == ' ') ss.ignore();
    }

    return parsedState;
}

int main(int argc, char* argv[])
{
    if (argc < 3)
    {
        std::cerr << "Use: ./main -<search_algorithm> <initial_states>" << std::endl;
        return 1;
    }

    SearchAlgorithm search_algorithm = selectAlgorithm(argv[1]);
    if (search_algorithm == UNKNOWN)
    {
        std::cerr << "Unknown algorithm." << std::endl;
        return 1;
    }

    // Process the initial states
    std::vector<std::vector<int>> initialStates;
    std::string statesInput;

    // Concatenate remaining arguments into a single string
    for (int i = 2; i < argc; ++i)
    {
        statesInput += argv[i];
        if (i < argc - 1)
            statesInput += " ";
    }

    std::stringstream stateStream(statesInput);
    std::string state;

    while (std::getline(stateStream, state, ','))
    {
        std::vector<int> parsedState = parseState(state);
        initialStates.push_back(parsedState);
    }

    std::cout << "Chosen algorithm: " << argv[1] << std::endl;
    for (size_t i = 0; i < initialStates.size(); ++i)
    {
        std::cout << "Initial state " << i + 1 << ": ";
        for (int num : initialStates[i]) 
            std::cout << num << " ";
        std::cout << std::endl;
    }

    return 0;
}
