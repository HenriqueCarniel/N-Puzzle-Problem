/*
    Node8.cpp
    ---------
    Implementação da classe `Node8`, que representa um nó específico para o problema do 8-puzzle.
    Este arquivo contém a implementação dos métodos para gerar filhos, calcular a distância de Manhattan,
    e verificar se o estado atual é o estado objetivo.
*/

#include "Node8.h"

extern int HeuristicNumberCalls;
extern double AverageValueHeuristic;

const uint8_t sideLength = 3;
const uint64_t Node8::goalState = encodeState({0, 1, 2, 3, 4, 5, 6, 7, 8});

Node8::Node8(const std::array<uint8_t, 9>& state, Node* parent, int cost, int depth, uint8_t blankIndex)
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

uint64_t Node8::encodeState(const std::array<uint8_t, 9>& state)
{
    uint64_t encodedState = 0;
    for (int i = 0; i < 9; ++i)
        encodedState |= (static_cast<uint64_t>(state[i]) << (i * 4));
    return encodedState;
}

std::array<uint8_t, 9> Node8::decodeState()
{
    std::array<uint8_t, 9> puzzle;
    for (int i = 0; i < 9; ++i)
        puzzle[i] = (state >> (i * 4)) & 0xF;
    return puzzle;
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
                auto decodedState = decodeState();
                std::array<uint8_t, 9> newState = decodedState;
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

    auto decodedState = decodeState();

    if (!parent)
    {
        heuristicValue = calculateManhattanDistanceInitialNode(decodedState);
        AverageValueHeuristic += heuristicValue;
        return;
    }

    int distance = 0;

    for (int i = 0; i < 9; ++i)
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

int Node8::calculateManhattanDistanceInitialNode(const std::array<uint8_t, 9>& state)
{
    int distance = 0;

    for (int i = 0; i < 9; ++i)
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