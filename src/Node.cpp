#include "Node.h"

int Node::idCounter = 0;

Node::Node(std::vector<int>& state, Node* parent, int cost, int depth)
    : state(state), parent(parent), cost(cost), depth(depth), id(idCounter++)
{
    auto it = std::find(state.begin(), state.end(), 0);
    blankIndex = std::distance(state.begin(), it);
    sideLength = std::sqrt(state.size());
    
    goalState.resize(state.size());
    std::iota(goalState.begin(), goalState.end(), 0);
}

std::vector<Node*> Node::generateChildren()
{
    std::vector<Node*> children;
    
    for (const auto& direction : DIRECTIONS) {
        int newRow = blankIndex / sideLength + direction.first;
        int newCol = blankIndex % sideLength + direction.second;

        if (newRow >= 0 && newRow < sideLength && newCol >= 0 && newCol < sideLength)
        {
            int newIndex = newRow * sideLength + newCol;
            
            if (parent == nullptr || newIndex != parent->blankIndex)
            {
                std::vector<int> newState(state);
                std::swap(newState[blankIndex], newState[newIndex]);

                children.emplace_back(new Node(newState, this, cost + 1, depth + 1));
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

std::vector<int> Node::getState() const
{
    return state;
}

Node* Node::getParent() const
{
    return parent;
}

int Node::getDepth() const
{
    return depth;
}

int Node::getId() const
{
    return id;
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

void Node::printPath() const
{
    if (parent != nullptr)
        parent->printPath();
    printState();
    std::cout << std::endl;
}