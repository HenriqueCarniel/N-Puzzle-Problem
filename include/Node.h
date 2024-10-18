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
    static int idCounter;

    int id;
    int blankIndex;
    int sideLength;

    std::vector<int> state;
    std::vector<int> goalState;
    Node* parent;
    int cost;
    int depth;

    // Specification
    std::vector<std::pair<int, int>> DIRECTIONS = {
        {-1, 0},    // Up
        {0, -1},    // Left
        {0, 1},     // Right
        {1, 0}      // Down
    };

public:
    Node(std::vector<int>& state, Node* parent = nullptr, int cost = 0, int depth = 0);
    std::vector<Node*> generateChildren();
    int calculateManhattanDistance() const;
    bool isGoalState() const;

    std::vector<int> getState() const;
    Node* getParent() const;
    int getDepth() const;
    int getId() const;

    void printState() const;
    void printPath() const;
};

#endif