#include <iostream>
#include <vector>
#include <csignal>
#include "Node8.h"
#include "Node15.h"
#include "InputHandler.h"
#include "ErrorCodes.h"
#include "SearchAlgorithms.h"

int HeuristicNumberCalls = 0;
double AverageValueHeuristic = 0.0f;
std::vector<Node*> AllNodes;

void handle_sigterm(int signum)
{
    Node::desalocateAllNodes();
    std::cout << "-,-,-,-,-" << std::endl;
    exit(signum);
}

int main(int argc, char* argv[])
{
    std::signal(SIGTERM, handle_sigterm);

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
        for (std::vector<uint8_t> initialState: initialStates)
        {
            size_t puzzleSize = initialState.size();
            if (puzzleSize == 16)
            {
                std::array<uint8_t, 16> arrayState;
                std::copy(initialState.begin(), initialState.end(), arrayState.begin());
                Node15 rootPuzzle(arrayState);
                SearchAlgorithms::runAlgorithm(rootPuzzle, search_algorithm);
            }
            else if (puzzleSize == 9)
            {
                std::array<uint8_t, 9> arrayState;
                std::copy(initialState.begin(), initialState.end(), arrayState.begin());
                Node8 rootPuzzle(arrayState);
                SearchAlgorithms::runAlgorithm(rootPuzzle, search_algorithm);
            }
            else
            {
                std::cerr << "Invalid puzzle size. Only 8-puzzle and 15-puzzle are supported." << std::endl;
                return static_cast<int>(ErrorCode::PUZZLE_SIZE);
            }

            SearchAlgorithms::printMetrics();
            Node::desalocateAllNodes();
        }

        //std::cout << "Size of Node class: " << sizeof(Node) << std::endl;
        //std::cout << "Size of Node8 class: " << sizeof(Node8) << std::endl;
        //std::cout << "Size of Node15 class: " << sizeof(Node15) << std::endl;
    }
    else
    {
        std::cerr << "No valid initial states provided." << std::endl;
        return static_cast<int>(ErrorCode::NO_INITIAL_STATES);
    }

    return 0;
}