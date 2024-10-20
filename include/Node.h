#ifndef NODE_H
#define NODE_H

#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <numeric> // std::iota
#include <functional>
#include <bitset>

class Node
{
private:
    const uint32_t id;
    uint8_t blankIndex;

    std::vector<uint8_t> state;
    Node* parent;
    uint32_t cost;
    uint32_t depth;

    static uint idCounter;
    static uint8_t sideLenght;
    static std::vector<uint8_t> goalState;
    static const std::array<std::pair<int8_t, int8_t>, 4> DIRECTIONS;
    
public:
    Node(const std::vector<uint8_t>& state, Node* parent = nullptr, int cost = 0, int depth = 0);
    static void initialize15puzzle();
    std::vector<Node*> generateChildren(std::function<int(const std::vector<uint8_t>&, const int)> costFunction);
    int calculateManhattanDistance() const;
    static int calculateManhattanDistanceStatic(const std::vector<uint8_t>& state);
    bool isGoalState() const;

    std::vector<uint8_t> getState() const;
    Node* getParent() const;
    uint32_t getDepth() const;
    uint32_t getId() const;
    uint32_t getCost() const;

    void printState() const;
    void printPath() const;
};

#endif