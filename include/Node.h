#ifndef NODE_H
#define NODE_H

#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <numeric> // std::iota
#include <functional>
#include <bitset>

extern int heuristicNumberCalls;
extern double averageValueHeuristic;

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
    static const uint8_t sideLenght;
    static const std::array<uint8_t, 9> goalState;
    static const std::array<std::pair<int8_t, int8_t>, 4> DIRECTIONS;
    
public:
    Node(const std::array<uint8_t, 9>& state, Node* parent = nullptr, int cost = 0, int depth = 0);
    static void initialize15puzzle();
    std::vector<Node*> generateChildren(std::function<int(const std::array<uint8_t, 9>&, const int)> costFunction);
    int calculateManhattanDistance() const;
    static int calculateManhattanDistanceStatic(const std::array<uint8_t, 9>& state);
    bool isGoalState() const;

    std::array<uint8_t, 9> getState() const;
    Node* getParent() const;
    uint32_t getDepth() const;
    uint32_t getId() const;
    uint32_t getCost() const;

    void printState() const;
    void printPath() const;
};

#endif