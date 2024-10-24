#include "Node.h"

extern std::vector<Node*> AllNodes;

uint Node::idCounter = 0;
const std::array<std::pair<int8_t, int8_t>, 4> Node::DIRECTIONS = {
    std::make_pair(-1, 0),  // Up
    std::make_pair(0, -1),  // Left
    std::make_pair(0, 1),   // Right
    std::make_pair(1, 0)    // Down
};

Node::Node(uint64_t state, Node* parent, int cost, int depth, uint8_t blankIndex)
    : state(state), parent(parent), cost(cost), depth(depth), id(idCounter++) {}

uint64_t Node::getState() const
{
    return state;
}

uint32_t Node::getDepth() const
{
    return depth;
}

uint32_t Node::getId() const
{
    return id;
}

uint8_t Node::getHeuristicValue() const
{
    return heuristicValue;
}

uint32_t Node::getCost() const
{
    return cost;
}

uint8_t Node::getBlankIndex() const
{
    return blankIndex;
}

Node* Node::getParent() const
{
    return parent;
}

void Node::setCost(int costValue)
{
    cost = costValue;
}

void Node::desalocateAllNodes()
{
    for (Node* node : AllNodes)
        delete node;
    AllNodes.clear();
}