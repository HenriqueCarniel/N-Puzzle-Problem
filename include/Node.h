/*
    Node.h
    ------
    Este arquivo define a classe abstrata `Node`, que serve como base para representar um nó em um 
    problema de N-Puzzle. A classe `Node` contém atributos e métodos essenciais para a manipulação
    de estados do puzzle e facilita o desenvolvimento de algoritmos de busca. A classe inclui métodos
    virtuais puros, projetados para serem implementados em classes derivadas para puzzles de tamanhos específicos.

    Esta classe é desenhada para permitir a criação de diferentes versões especializadas para tamanhos específicos de puzzles,
    mantendo a maior parte da lógica de busca e manipulação de estado em uma estrutura comum e extensível.
*/

#ifndef NODE_H
#define NODE_H

#include <vector>
#include <iostream>
#include <algorithm>
#include <array>

class Node
{
protected:
    Node* parent;
    uint64_t state;
    const uint32_t id;
    uint32_t cost;
    uint32_t depth;
    uint8_t blankIndex;
    uint8_t heuristicValue;

    static uint idCounter;
    static const std::array<std::pair<int8_t, int8_t>, 4> DIRECTIONS;
    
public:
    Node(uint64_t state, Node* parent = nullptr, int cost = 0, int depth = 0, uint8_t blankIndex = -1);

    virtual std::vector<Node*> generateChildren() = 0;
    virtual void calculateManhattanDistance() = 0;
    virtual bool isGoalState() const = 0;

    uint64_t getState() const;
    uint32_t getDepth() const;
    uint32_t getId() const;
    uint8_t getHeuristicValue() const;
    uint32_t getCost() const;
    uint8_t getBlankIndex() const;
    Node* getParent() const;
    void setCost(int costValue);
    
    static void desalocateAllNodes();
};

#endif