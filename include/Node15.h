/*
    Node15.h
    ---------
    A classe Node8 herda da classe base Node e implementa funcionalidades específicas para 
    manipular e operar no 15-Puzzle, incluindo a geração de estados filhos, cálculo da 
    distância de Manhattan e verificação do estado objetivo.
*/

#ifndef NODE15_H
#define NODE15_H

#include "Node.h"

class Node15: public Node
{
private:
    static const uint8_t sideLength = 4;
    static const uint64_t goalState;

public:
    Node15(const std::array<uint8_t, 16>& state, Node* parent = nullptr, int cost = 0, int depth = 0, uint8_t blankIndex = -1);

    std::vector<Node*> generateChildren() override;
    void calculateManhattanDistance() override;
    bool isGoalState() const override;

    int calculateManhattanDistanceInitialNode(const std::array<uint8_t, 16>& state);
    static uint64_t encodeState(const std::array<uint8_t, 16>& state);
    std::array<uint8_t, 16> decodeState();
};

#endif