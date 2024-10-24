#ifndef NODE_H
#define NODE_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <array>

class Node
{
protected:
    Node* parent;                 // Ponteiro, 8 bytes (mover para o topo)
    const uint32_t id;            // 4 bytes
    uint32_t cost;                // 4 bytes
    uint32_t depth;               // 4 bytes
    uint8_t blankIndex;           // 1 byte
    uint8_t heuristicValue;       // 1 byte

    static uint idCounter;
    static const std::array<std::pair<int8_t, int8_t>, 4> DIRECTIONS;
    
public:
    Node(Node* parent = nullptr, int cost = 0, int depth = 0, uint8_t blankIndex = -1);
    virtual std::vector<Node*> generateChildren() = 0;
    virtual void calculateManhattanDistance() = 0;
    virtual bool isGoalState() const = 0;
    virtual std::vector<uint8_t> getState() const = 0;

    uint32_t getDepth() const;
    uint32_t getId() const;
    uint8_t getHeuristicValue() const;
    uint32_t getCost() const;
    uint8_t getBlankIndex() const;
    void setCost(int costValue);

    Node* getParent() const;

    static void desalocateAllNodes();
};

#endif