#include "Node8.h"

extern int HeuristicNumberCalls;
extern double AverageValueHeuristic;

const uint8_t sideLength = 3;
const std::array<uint8_t, 9> Node8::goalState = {0, 1, 2, 3, 4, 5, 6, 7, 8};

Node8::Node8(const std::array<uint8_t, 9>& state, Node* parent, int cost, int depth, uint8_t blankIndex)
    : Node(parent, cost, depth, blankIndex), state(state) 
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

std::vector<Node*> Node8::generateChildren()
{
    std::vector<Node*> children;

    for (const auto& direction : DIRECTIONS)
    {
        int newRow = blankIndex / sideLength + direction.first;
        int newCol = blankIndex % sideLength + direction.second;

        if (newRow >= 0 && newRow < sideLength && newCol >= 0 && newCol < sideLength)
        {
            uint8_t newIndex = newRow * sideLength + newCol;

            if (parent == nullptr || newIndex != parent->getBlankIndex())
            {
                std::array<uint8_t, 9> newState = state;
                std::swap(newState[blankIndex], newState[newIndex]);

                Node* childNode = new Node8(newState, this, 0, depth + 1, newIndex);
                children.emplace_back(childNode);
            }
        }
    }

    return children;
}

void Node8::calculateManhattanDistance()
{
    HeuristicNumberCalls += 1;

    if (!parent)
    {
        heuristicValue = calculateManhattanDistanceInitialNode(state);
        AverageValueHeuristic += heuristicValue;
        return;
    }

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

    heuristicValue = distance;
    AverageValueHeuristic += heuristicValue;
}

int Node8::calculateManhattanDistanceInitialNode(const std::array<uint8_t, 9>& state)
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

bool Node8::isGoalState() const
{
    return state == goalState;
}

std::vector<uint8_t> Node8::getState() const
{
    return std::vector<uint8_t>(state.begin(), state.end());
}