#ifndef NODE_H
#define NODE_H

#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>

class Node
{
private:
    int blankIndex;
    int sideLength;

    std::vector<int> state;
    Node* parent;
    int cost;
    int depth;
    int lastBlankIndex;

    std::vector<std::pair<int, int>> DIRECTIONS = {
        {-1, 0},    // Up
        {1, 0},     // Down
        {0, -1},    // Left
        {0, 1}      // Right
    };

    bool isGoalState() const;
    int calculateManhattanDistance() const;

public:
    Node(const std::vector<int>& state, Node* parent = nullptr, int cost = 0, int depth = 0, int lastBlankIndex = -1);
    std::vector<Node> generateChildren() const;
    void printState() const;
    void printInfo() const;
};

#endif