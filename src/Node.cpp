#include "Node.h"

extern std::vector<Node*> AllNodes;
extern int HeuristicNumberCalls;
extern double AverageValueHeuristic;

uint Node::idCounter = 0;
const std::array<uint8_t, 9> Node::goalState = {0, 1, 2, 3, 4, 5, 6, 7, 8}; //9, 10, 11, 12, 13, 14, 15};
const uint8_t Node::sideLenght = 3;

// Specification
const std::array<std::pair<int8_t, int8_t>, 4> Node::DIRECTIONS = {
    std::make_pair(-1, 0),  // Up
    std::make_pair(0, -1),  // Left
    std::make_pair(0, 1),   // Right
    std::make_pair(1, 0)    // Down
};

Node::Node(const std::array<uint8_t, 9>& state, Node* parent, int cost, int depth, uint8_t blankIndex)
    : state(state), parent(parent), cost(cost), depth(depth), id(idCounter++)
{
    if (!parent)
    {
        auto it = std::find(state.begin(), state.end(), 0);
        this->blankIndex = std::distance(state.begin(), it);
    }
    else
    {
        this->blankIndex = blankIndex;
    }
        
}

std::vector<Node*> Node::generateChildren()
{
    std::vector<Node*> children;
    
    for (const auto& direction : DIRECTIONS)
    {
        int newRow = blankIndex / sideLenght + direction.first;
        int newCol = blankIndex % sideLenght + direction.second;

        if (newRow >= 0 && newRow < sideLenght && newCol >= 0 && newCol < sideLenght)
        {
            uint8_t newIndex = newRow * sideLenght + newCol;
            
            if (parent == nullptr || newIndex != parent->blankIndex)
            {
                std::array<uint8_t, 9> newState(state);
                std::swap(newState[blankIndex], newState[newIndex]);
                //std::swap(blankIndex, newIndex);

                Node* childNode = new Node(newState, this, 0, depth + 1, newIndex);
                AllNodes.emplace_back(childNode);
                children.emplace_back(childNode);
            }
        }
    }

    return children;
}

bool Node::isGoalState() const
{
    return state == goalState;
}

void Node::calculateManhattanDistance()
{
    HeuristicNumberCalls += 1;

    if (!parent)
    {
        heuristicValue = calculateManhattanDistanceInitialNode(state);
        AverageValueHeuristic += heuristicValue;
        return;
    }

    /*
    uint8_t movedTile = state[parent->blankIndex];
    int goalRow = movedTile / sideLenght;
    int goalCol = movedTile % sideLenght;
    int oldRow = parent->blankIndex / sideLenght;
    int oldCol = parent->blankIndex % sideLenght;
    int newRow = blankIndex / sideLenght;
    int newCol = blankIndex % sideLenght;
    int oldDistance = std::abs(goalRow - oldRow) + std::abs(goalCol - oldCol);
    int newDistance = std::abs(goalRow - newRow) + std::abs(goalCol - newCol);
    
    heuristicValue = parent->heuristicValue;
    if (newDistance < oldDistance)
        heuristicValue -= 1;
    else
        heuristicValue += 1;

    AverageValueHeuristic += heuristicValue;
    */

    int distance = 0;
    int N = state.size();

    for (int i = 0; i < N; ++i)
    {
        if (state[i] != 0)
        {
            int goalRow = state[i] / sideLenght;
            int goalCol = state[i] % sideLenght;
            int currentRow = i / sideLenght;
            int currentCol = i % sideLenght;

            distance += std::abs(currentRow - goalRow) + std::abs(currentCol - goalCol);
        }
    }

    heuristicValue = distance;
    AverageValueHeuristic += heuristicValue;
}

int Node::calculateManhattanDistanceInitialNode(const std::array<uint8_t, 9>& state)
{
    int distance = 0;
    int N = state.size();

    for (int i = 0; i < N; ++i)
    {
        if (state[i] != 0)
        {
            int goalRow = state[i] / sideLenght;
            int goalCol = state[i] % sideLenght;
            int currentRow = i / sideLenght;
            int currentCol = i % sideLenght;

            distance += std::abs(currentRow - goalRow) + std::abs(currentCol - goalCol);
        }
    }

    return distance;
}

void Node::desalocateAllNodes()
{
    for (Node* node : AllNodes)
        delete node;
    AllNodes.clear();
}

std::array<uint8_t, 9> Node::getState() const
{
    return state;
}

Node* Node::getParent() const
{
    return parent;
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

void Node::setCost(int costValue)
{
    cost = costValue;
}

void Node::printState() const
{
    for (int i = 0; i < state.size(); ++i)
    {
        if (i % sideLenght == 0 && i != 0)
            std::cout << std::endl;
        std::cout << state[i] << " ";
    }
    std::cout << std::endl;
}

void Node::printPath() const
{
    if (parent != nullptr)
        parent->printPath();
    printState();
    std::cout << std::endl;
}