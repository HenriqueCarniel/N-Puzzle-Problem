#include "Node.h"

Node::Node(std::vector<int>& state, Node* parent, int cost, int depth, int lastBlankIndex)
    : state(state), parent(parent), cost(cost), depth(depth), lastBlankIndex(lastBlankIndex)
{
    auto it = std::find(state.begin(), state.end(), 0);
    blankIndex = std::distance(state.begin(), it);
    sideLength = std::sqrt(state.size());
    
    goalState.resize(state.size());
    std::iota(goalState.begin(), goalState.end(), 0);
}

std::vector<Node> Node::generateChildren()
{
    std::vector<Node> children;
    
    for (const auto& direction : DIRECTIONS) {
        int newRow = blankIndex / sideLength + direction.first;
        int newCol = blankIndex % sideLength + direction.second;

        if (newRow >= 0 && newRow < sideLength && newCol >= 0 && newCol < sideLength)
        {
            int newIndex = newRow * sideLength + newCol;
            
            if (parent == nullptr || newIndex != lastBlankIndex)
            {
                std::vector<int> newState = state;
                std::swap(newState[blankIndex], newState[newIndex]);

                children.emplace_back(newState, this, cost + 1, depth + 1, blankIndex);
            }
        }
    }

    return children;
}

bool Node::isGoalState()
{
    return state == goalState;
}

int Node::calculateManhattanDistance()
{
    int distance = 0;
    int N = state.size();

    for (int i = 0; i < N; ++i)
    {
        if (state[i] != 0)
        {
            int goalRow = state[i] / sideLength;
            int goalCol = state[i] % sideLength;
            int currentRow = i / sideLength;
            int currentCol = i % sideLength;

            distance += std::abs(currentRow - goalRow) + std::abs(currentCol - goalCol);
        }
    }

    return distance;
}

void Node::printState()
{
    for (int i = 0; i < state.size(); ++i)
    {
        if (i % sideLength == 0 && i != 0)
            std::cout << std::endl;
        std::cout << state[i] << " ";
    }
    std::cout << std::endl;
}

void Node::printInfo()
{
    std::cout << "Node Information:" << std::endl;

    printState();
    std::cout << "Blank Index: " << blankIndex << std::endl;
    std::cout << "Last Blank Index: " << lastBlankIndex << std::endl;
    std::cout << "Cost: " << cost << std::endl;
    std::cout << "Depth: " << depth << std::endl;

    if (parent != nullptr) 
    {
        std::cout << "Parent Node Information:" << std::endl;
        parent->printInfo();
    } 
    else 
    {
        std::cout << "No Parent Node." << std::endl;
    }
}