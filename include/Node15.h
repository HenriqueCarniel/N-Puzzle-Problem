#ifndef NODE15_H
#define NODE15_H

#include "Node.h"

class Node15: public Node
{
private:
    static const uint8_t sideLength = 4;
    static const uint64_t goalState;

public:
    Node15(const std::array<uint8_t, 16>& state, Node* parent = nullptr, int cost = 0, int depth = 0, uint8_t blankIndex = -1);

    std::vector<Node*> generateChildren() override;
    void calculateManhattanDistance() override;
    bool isGoalState() const override;

    int calculateManhattanDistanceInitialNode(const std::array<uint8_t, 16>& state);
    static uint64_t encodeState(const std::array<uint8_t, 16>& state);
    std::array<uint8_t, 16> decodeState();
};

#endif