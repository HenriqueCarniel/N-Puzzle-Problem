#include "Node.h"

extern std::vector<Node*> AllNodes;

uint Node::idCounter = 0;
const std::array<std::pair<int8_t, int8_t>, 4> Node::DIRECTIONS = {
    std::make_pair(-1, 0),  // Up
    std::make_pair(0, -1),  // Left
    std::make_pair(0, 1),   // Right
    std::make_pair(1, 0)    // Down
};

Node::Node(Node* parent, int cost, int depth, uint8_t blankIndex)
    : parent(parent), cost(cost), depth(depth), id(idCounter++) {}

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

void Node::setCost(int costValue)
{
    cost = costValue;
}

Node* Node::getParent() const
{
    return parent;
}

void Node::desalocateAllNodes()
{
    for (Node* node : AllNodes)
        delete node;
    AllNodes.clear();
}