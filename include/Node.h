#ifndef NODE_H
#define NODE_H

#include <vector>
#include <cmath>
#include <iostream>
#include <algorithm>
#include <numeric> // std::iota
#include <functional>

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
    Node(const std::vector<int>& state, Node* parent = nullptr, int cost = 0, int depth = 0);
    std::vector<Node*> generateChildren(std::function<int(const std::vector<int>&, const int)> costFunction);
    int calculateManhattanDistance() const;
    bool isGoalState() const;

    std::vector<int> getState() const;
    Node* getParent() const;
    int getDepth() const;
    int getId() const;
    int getCost() const;

    void printState() const;
    void printPath() const;
};

#endif