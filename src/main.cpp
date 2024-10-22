#include <iostream>
#include <vector>
#include "Node.h"
#include "InputHandler.h"
#include "ErrorCodes.h"
#include "SearchAlgorithms.h"

int HeuristicNumberCalls = 0;
double AverageValueHeuristic = 0.0f;

int main(int argc, char* argv[])
{
    if (argc < 2)
    {
        std::cerr << "Use: ./main -<search_algorithm> <initial_states> or ./main -<search_algorithm> < <input_file>" << std::endl;
        return static_cast<int>(ErrorCode::INVALID_ARGUMENTS);
    }

    SearchAlgorithm search_algorithm = InputHandler::selectAlgorithm(argv[1]);
    if (search_algorithm == SearchAlgorithm::UNKNOWN)
    {
        std::cerr << "Unknown algorithm." << std::endl;
        return static_cast<int>(ErrorCode::UNKNOWN_ALGORITHM);
    }

    std::vector<std::vector<uint8_t>> initialStates = InputHandler::processInput(argc, argv);
    if (!initialStates.empty())
    {
        size_t puzzleSize = initialStates[0].size();
        for (std::vector<uint8_t> initialState: initialStates)
        {
            std::array<uint8_t, 9> arrayState;
            std::copy(initialState.begin(), initialState.end(), arrayState.begin());
            Node rootPuzzle(arrayState);

            SearchAlgorithms::runAlgorithm(rootPuzzle, search_algorithm);
            SearchAlgorithms::printMetrics();

            Node::desalocateAllNodes();
        }
    }
    else
    {
        std::cerr << "No valid initial states provided." << std::endl;
        return static_cast<int>(ErrorCode::NO_INITIAL_STATES);
    }

    return 0;
}