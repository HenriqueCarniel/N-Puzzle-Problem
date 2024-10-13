#include <iostream>
#include <vector>
#include "Node.h"
#include "InputHandler.h"
#include "ErrorCodes.h"

void printTest(std::vector<std::vector<int>> initialStates)
{
    for (size_t i = 0; i < initialStates.size(); ++i)
    {
        std::cout << "State " << i + 1 << ":" << std::endl;
        Node rootPuzzle = Node(initialStates[i]);
        //rootPuzzle.printInfo();
        rootPuzzle.printState();
        std::cout << std::endl;

        /*
        std::vector<Node> children = rootPuzzle.generateChildren();
        for (size_t j = 0; j < children.size(); ++j)
        {
            std::cout << "Child " << j + 1 << " of State " << i + 1 << ":" << std::endl;
            children[j].printInfo();
            std::cout << std::endl;

            std::vector<Node> grandson = children[j].generateChildren();
            for (size_t k = 0; k < grandson.size(); ++k)
            {
                std::cout << "Grandson " << k + 1 << " of Child " << j + 1 << ":" << std::endl;
                grandson[k].printInfo();
                std::cout << std::endl;
            }
        }
        break;
        */
    }
}

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

    std::vector<std::vector<int>> initialStates = InputHandler::processInput(argc, argv);

    if (!initialStates.empty())
    {
        printTest(initialStates);
    }
    else
    {
        std::cerr << "No valid initial states provided." << std::endl;
        return static_cast<int>(ErrorCode::NO_INITIAL_STATES);
    }

    return 0;
}