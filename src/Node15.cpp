#include "Node15.h"

extern int HeuristicNumberCalls;
extern double AverageValueHeuristic;

const uint8_t sideLength = 4;
const uint64_t Node15::goalState = encodeState({0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15});

Node15::Node15(const std::array<uint8_t, 16>& state, Node* parent, int cost, int depth, uint8_t blankIndex)
    : Node(encodeState(state), parent, cost, depth, blankIndex)
{
    if (!parent)
    {
        auto decodedState = decodeState();
        auto it = std::find(decodedState.begin(), decodedState.end(), 0);
        this->blankIndex = std::distance(decodedState.begin(), it);
    }
    else
    {
        this->blankIndex = blankIndex;
    }
}

uint64_t Node15::encodeState(const std::array<uint8_t, 16>& state)
{
    uint64_t encodedState = 0;
    for (int i = 0; i < 16; ++i)
        encodedState |= (static_cast<uint64_t>(state[i]) << (i * 4));
    return encodedState;
}

std::array<uint8_t, 16> Node15::decodeState()
{
    std::array<uint8_t, 16> puzzle;
    for (int i = 0; i < 16; ++i)
        puzzle[i] = (state >> (i * 4)) & 0xF;
    return puzzle;
}

std::vector<Node*> Node15::generateChildren()
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
                auto decodedState = decodeState();
                std::array<uint8_t, 16> newState = decodedState;
                std::swap(newState[blankIndex], newState[newIndex]);

                Node* childNode = new Node15(newState, this, 0, depth + 1, newIndex);
                children.emplace_back(childNode);
            }
        }
    }

    return children;
}

void Node15::calculateManhattanDistance()
{
    HeuristicNumberCalls += 1;

    auto decodedState = decodeState();

    if (!parent)
    {
        heuristicValue = calculateManhattanDistanceInitialNode(decodedState);
        AverageValueHeuristic += heuristicValue;
        return;
    }

    int distance = 0;

    for (int i = 0; i < 16; ++i)
    {
        if (decodedState[i] != 0)
        {
            int goalRow = decodedState[i] / sideLength;
            int goalCol = decodedState[i] % sideLength;
            int currentRow = i / sideLength;
            int currentCol = i % sideLength;

            distance += std::abs(currentRow - goalRow) + std::abs(currentCol - goalCol);
        }
    }

    heuristicValue = distance;
    AverageValueHeuristic += heuristicValue;
}

int Node15::calculateManhattanDistanceInitialNode(const std::array<uint8_t, 16>& state)
{
    int distance = 0;

    for (int i = 0; i < 16; ++i)
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

bool Node15::isGoalState() const
{
    return state == goalState;
}