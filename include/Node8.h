#ifndef NODE8_H
#define NODE8_H

#include "Node.h"

class Node8: public Node
{
private:
    static const uint8_t sideLength = 3;
    static const uint64_t goalState;

public:
    Node8(const std::array<uint8_t, 9>& state, Node* parent = nullptr, int cost = 0, int depth = 0, uint8_t blankIndex = -1);

    std::vector<Node*> generateChildren() override;
    void calculateManhattanDistance() override;
    bool isGoalState() const override;

    int calculateManhattanDistanceInitialNode(const std::array<uint8_t, 9>& state);
    static uint64_t encodeState(const std::array<uint8_t, 9>& state);
    std::array<uint8_t, 9> decodeState(); 
};

#endif