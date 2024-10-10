#ifndef NODE_H
#define NODE_H

#include <vector>
#include <cmath>
#include <iostream>

class Node
{
private:
    std::vector<int> state;
    Node* parent;
    int cost;
    int depth;

    bool isGoalState() const;
    int calculateManhattanDistance() const;

public:
    Node(const std::vector<int>& state, Node* parent = nullptr, int cost = 0, int depth = 0);
    std::vector<Node> generateChildren() const;
    void printState() const;
};

#endif