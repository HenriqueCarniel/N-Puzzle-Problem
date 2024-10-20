#include "Node.h"

uint Node::idCounter = 0;
std::vector<uint8_t> Node::goalState = {0, 1, 2, 3, 4, 5, 6, 7, 8};
uint8_t Node::sideLenght = 3;

// Specification
const std::array<std::pair<int8_t, int8_t>, 4> Node::DIRECTIONS = {
    std::make_pair(-1, 0),  // Up
    std::make_pair(0, -1),  // Left
    std::make_pair(0, 1),   // Right
    std::make_pair(1, 0)    // Down
};

Node::Node(const std::vector<uint8_t>& state, Node* parent, int cost, int depth)
    : state(state), parent(parent), cost(cost), depth(depth), id(idCounter++)
{
    auto it = std::find(state.begin(), state.end(), 0);
    blankIndex = std::distance(state.begin(), it);
}

void Node::initialize15puzzle()
{
    goalState = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    sideLenght = 4;
}

std::vector<Node*> Node::generateChildren(std::function<int(const std::vector<uint8_t>&, const int)> costFunction)
{
    std::vector<Node*> children;
    
    for (const auto& direction : DIRECTIONS)
    {
        int newRow = blankIndex / sideLenght + direction.first;
        int newCol = blankIndex % sideLenght + direction.second;

        if (newRow >= 0 && newRow < sideLenght && newCol >= 0 && newCol < sideLenght)
        {
            int newIndex = newRow * sideLenght + newCol;
            
            if (parent == nullptr || newIndex != parent->blankIndex)
            {
                std::vector<uint8_t> newState(state);
                std::swap(newState[blankIndex], newState[newIndex]);

                children.emplace_back(new Node(newState, this, costFunction(newState, depth + 1), depth + 1));
            }
        }
    }

    return children;
}

bool Node::isGoalState() const
{
    return state == goalState;
}

int Node::calculateManhattanDistance() const
{
    //if (!parent)
    //   return calculateManhattanDistanceStatic(state);

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

int Node::calculateManhattanDistanceStatic(const std::vector<uint8_t>& state)
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

std::vector<uint8_t> Node::getState() const
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

uint32_t Node::getCost() const
{
    return cost;
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