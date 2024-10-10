#include "Node.h"

Node::Node(const std::vector<int>& state, Node* parent, int cost, int depth)
    : state(state), parent(parent), cost(cost), depth(depth) {}

std::vector<Node> Node::generateChildren() const
{
    std::vector<Node> children;

    // TODO

    return children;
}

void Node::printState() const
{
    int sideLength = std::sqrt(state.size());

    for (int i = 0; i < state.size(); ++i)
    {
        if (i % sideLength == 0 && i != 0)
            std::cout << std::endl;
        std::cout << state[i] << " ";
    }
    std::cout << std::endl;
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