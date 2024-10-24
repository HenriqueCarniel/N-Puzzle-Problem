#ifndef NODE_H
#define NODE_H

#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <numeric> // std::iota
#include <functional>
#include <bitset>

extern int HeuristicNumberCalls;
extern double AverageValueHeuristic;

class Node
{
private:
    const uint32_t id;
    uint8_t blankIndex;

    std::array<uint8_t, 9> state;
    Node* parent;
    uint32_t cost;
    uint32_t depth;
    uint8_t heuristicValue;

    static uint idCounter;
    static std::vector<Node*> allNodes;
    static const uint8_t sideLenght;
    static const std::array<uint8_t, 9> goalState;
    static const std::array<std::pair<int8_t, int8_t>, 4> DIRECTIONS;
    
public:
    Node(const std::array<uint8_t, 9>& state, Node* parent = nullptr, int cost = 0, int depth = 0, uint8_t blankIndex = -1);
    std::vector<Node*> generateChildren();
    void calculateManhattanDistance();
    static int calculateManhattanDistanceInitialNode(const std::array<uint8_t, 9>& state);
    static void desalocateAllNodes();
    bool isGoalState() const;

    std::array<uint8_t, 9> getState() const;
    Node* getParent() const;
    uint32_t getDepth() const;
    uint32_t getId() const;
    uint8_t getHeuristicValue() const;
    uint32_t getCost() const;
    void setCost(int costValue);

    void printState() const;
    void printPath() const;
};

#endif