#ifndef NODE8_H
#define NODE8_H

#include "Node.h"

class Node8: public Node
{
private:
    static const uint8_t sideLength = 3;
    std::array<uint8_t, 9> state; 
    static const std::array<uint8_t, 9> goalState;

public:
    Node8(const std::array<uint8_t, 9>& state, Node* parent = nullptr, int cost = 0, int depth = 0, uint8_t blankIndex = -1);
    std::vector<Node*> generateChildren() override;
    void calculateManhattanDistance() override;
    bool isGoalState() const override;
    std::vector<uint8_t> getState() const override;

    int calculateManhattanDistanceInitialNode(const std::array<uint8_t, 9>& state);
    std::array<uint8_t, 9> getState();
};

#endif