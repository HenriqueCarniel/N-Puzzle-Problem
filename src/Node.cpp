#include "Node.h"

Node::Node(const std::vector<int>& state, Node* parent, int cost, int depth, int lastBlankIndex)
    : state(state), parent(parent), cost(cost), depth(depth), lastBlankIndex(lastBlankIndex)
{
    auto it = std::find(state.begin(), state.end(), 0);
    blankIndex = std::distance(state.begin(), it);
    sideLength = std::sqrt(state.size());
}

std::vector<Node> Node::generateChildren() const
{
    std::vector<Node> children;
    
    for (int i = 0; i < DIRECTIONS.size(); ++i) {
        int newRow = blankIndex / sideLength + DIRECTIONS[i].first;
        int newCol = blankIndex % sideLength + DIRECTIONS[i].second;

        if (newRow >= 0 && newRow < sideLength && newCol >= 0 && newCol < sideLength)
        {
            int newIndex = newRow * sideLength + newCol;
            
            if (parent == nullptr || newIndex != lastBlankIndex)
            {
                std::vector<int> newState = state;
                std::swap(newState[blankIndex], newState[newIndex]);

                //children.emplace_back(newState, this, cost + 1, depth + 1, newIndex);
            }
        }
    }

    return children;
}

// TODO: Maybe exists a better solution???
bool Node::isGoalState() const
{
    for (size_t i = 0; i < state.size(); ++i)
        if (state[i] != (i + 1)) return false;
    return true;
}

// TODO
int Node::calculateManhattanDistance() const
{
    return 0;
}

void Node::printState() const
{
    for (int i = 0; i < state.size(); ++i)
    {
        if (i % sideLength == 0 && i != 0)
            std::cout << std::endl;
        std::cout << state[i] << " ";
    }
    std::cout << std::endl;
}

void Node::printInfo() const
{
    std::cout << "Node Information:" << std::endl;

    printState(); // Chama a função existente para imprimir o estado
    std::cout << "Blank Index: " << blankIndex << std::endl;
    std::cout << "Last Blank Index: " << lastBlankIndex << std::endl;
    std::cout << "Cost: " << cost << std::endl;
    std::cout << "Depth: " << depth << std::endl;

    // Se houver um pai, imprima informações sobre ele
    if (parent != nullptr) {
        std::cout << "Parent Node Information:" << std::endl;
        parent->printInfo(); // Chama recursivamente para imprimir informações do pai
    } else {
        std::cout << "No Parent Node." << std::endl;
    }
}