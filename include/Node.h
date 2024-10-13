#ifndef NODE_H
#define NODE_H

#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <numeric> // std::iota

class Node
{
private:
    int blankIndex;
    int sideLength;

    std::vector<int> state;
    std::vector<int> goalState;
    Node* parent;
    int cost;
    int depth;
    int lastBlankIndex;

    // Specification
    std::vector<std::pair<int, int>> DIRECTIONS = {
        {-1, 0},    // Up
        {0, -1},    // Left
        {0, 1},     // Right
        {1, 0}      // Down
    };

    bool isGoalState();
    int calculateManhattanDistance();

public:
    Node(std::vector<int>& state, Node* parent = nullptr, int cost = 0, int depth = 0, int lastBlankIndex = -1);
    std::vector<Node> generateChildren();
    void printState();
    void printInfo();
};

#endif