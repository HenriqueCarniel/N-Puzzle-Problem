/*
    SearchAlgorithms.h - Implementação de algoritmos de busca para problemas de puzzle (como 8-puzzle ou 15-puzzle).
    
    A classe SearchAlgorithms define uma série de algoritmos de busca, incluindo:
        - BFS
        - DLS
        - IDS
        - Greedy Best-First Search
        - A*
        - IDA*
    
    Estrutura:
        - SearchMetrics: estrutura para armazenar métricas, como o número de nós expandidos, 
          comprimento da solução, tempo de execução e valor heurístico inicial.
        - NodeHash / NodeEqual: Funções auxiliares para hash e comparação de nós, permitindo 
          evitar duplicatas durante a busca.

    Funcionamento:
        - Cada algoritmo usa uma estrutura específica (fila, pilha, ou fila de prioridade) para gerenciar 
          os nós expandidos, garantindo que os nós sejam explorados conforme a estratégia do algoritmo.
        - Funções de comparação (CompareNodeGbfs e CompareNodeAstar) ajudam na ordenação dos nós 
          nas buscas gulosa e A*, permitindo otimização da expansão com base no custo ou heurística.

    Observações:
        - Gerenciamento de Memória: Alguns algoritmos, como DLS e IDA*, fazem desalocação manual de nós 
          para otimizar uso de memória.
        - Otimizações e Controle de Métricas: Inclui contagem de chamadas heurísticas e média para 
          auxiliar na análise do custo heurístico em diferentes algoritmos.
*/

#include "SearchAlgorithms.h"

extern int HeuristicNumberCalls;
extern double AverageValueHeuristic;

SearchMetrics SearchAlgorithms::metrics;

void SearchAlgorithms::clearMetrics()
{
    metrics = SearchMetrics();
    HeuristicNumberCalls = 0;
    AverageValueHeuristic = 0;
}

void SearchAlgorithms::setPathMetrics(Node* finalNode)
{
    if (finalNode == nullptr)
        return;

    int pathLength = 0;
    int sumHeuristcValue = 0;
    Node* currentNode = finalNode;

    while (currentNode->getParent() != nullptr)
    {
        pathLength++;
        currentNode = currentNode->getParent();
    }
    
    metrics.optimalSolutionLength = pathLength;
    metrics.initialValueHeuristic = currentNode->getHeuristicValue();
}

void SearchAlgorithms::printMetrics()
{
    // Means that calculated the manhattan distance only for the initial node
    if (HeuristicNumberCalls == 1)
        AverageValueHeuristic = 0;

    std::cout << metrics.numExpandedNodes << ","
        << metrics.optimalSolutionLength << ","
        << metrics.time << ","
        << AverageValueHeuristic / HeuristicNumberCalls << ","
        << metrics.initialValueHeuristic << std::endl;
}

void SearchAlgorithms::runAlgorithm(Node& rootPuzzle, SearchAlgorithm type)
{
    std::optional<Node*> response;
    auto start = std::chrono::high_resolution_clock::now();

    if (type == SearchAlgorithm::BFS)
        response = SearchAlgorithms::bfsGraph(rootPuzzle);
    else if (type == SearchAlgorithm::IDFS)
        response = SearchAlgorithms::iterativeDeepeningSearch(rootPuzzle);
    else if (type == SearchAlgorithm::ASTAR)
        response = SearchAlgorithms::astar(rootPuzzle);
    else if (type == SearchAlgorithm::IDASTAR)
        response = SearchAlgorithms::idastar(rootPuzzle);
    else if (type == SearchAlgorithm::GBFS)
        response = SearchAlgorithms::greedyBestFirstSearch(rootPuzzle);

    auto end = std::chrono::high_resolution_clock::now();
    metrics.time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000.0;
    if (response.has_value())
        setPathMetrics(response.value());
}

struct NodeHash
{
    size_t operator()(const Node* node) const
    {
        return std::hash<uint64_t>()(node->getState());
    }
};

struct NodeEqual
{
    bool operator()(const Node* a, const Node* b) const
    {
        return a->getState() == b->getState();
    }
};

std::optional<Node*> SearchAlgorithms::bfsGraph(Node& initialNode)
{
    clearMetrics();

    if (initialNode.isGoalState())
        return &initialNode;

    initialNode.calculateManhattanDistance();

    std::deque<Node*> open;
    open.push_back(&initialNode);

    std::unordered_set<Node*, NodeHash, NodeEqual> closed;
    closed.insert(&initialNode);

    while (!open.empty())
    {
        Node* currentNode = open.front();
        open.pop_front();
        metrics.numExpandedNodes++;

        for (Node* child: currentNode->generateChildren())
        {
            if (child->isGoalState())
                return child;

            if (closed.find(child) == closed.end())
            {
                closed.insert(child);
                open.push_back(child);
            }
        }
    }

    return std::nullopt;
}

std::optional<Node*> SearchAlgorithms::depthLimitedSearch(Node* node, int depthLimit)
{
    if (node->isGoalState())
        return node;

    if (depthLimit > 0)
    {
        metrics.numExpandedNodes++;
        for (Node* child: node->generateChildren())
        {
            std::optional<Node*> solution = depthLimitedSearch(child, depthLimit - 1);
            if (solution.has_value())
                return solution;

            delete child;
        }
    }

    return std::nullopt;
}

std::optional<Node*> SearchAlgorithms::iterativeDeepeningSearch(Node& initialNode)
{
    clearMetrics();

    initialNode.calculateManhattanDistance();
    for (int depthLimit = 0; ; ++depthLimit)
    {
        std::optional<Node*> solution = depthLimitedSearch(&initialNode, depthLimit);
        if (solution.has_value())
            return solution;

        Node::desalocateAllNodes();
    }

    return std::nullopt;
}

struct CompareNodeGbfs
{
    bool operator()(Node* const& n1, Node* const& n2) {
        int n1f = n1->getCost();
        int n2f = n2->getCost();

        if (n1f != n2f) return n1f > n2f;

        int n1g = n1->getDepth();
        int n2g = n2->getDepth();

        if (n1g != n2g) return n1g < n2g;

        return n1->getId() < n2->getId();
    }
};

std::optional<Node*> SearchAlgorithms::greedyBestFirstSearch(Node& initialNode)
{
    clearMetrics();

    std::priority_queue<Node*, std::vector<Node*>, CompareNodeGbfs> open;
    std::unordered_set<Node*, NodeHash, NodeEqual> closed;

    initialNode.calculateManhattanDistance();
    initialNode.setCost(initialNode.getHeuristicValue() + initialNode.getDepth());
    open.push(&initialNode);

    while(!open.empty())
    {
        Node* currentNode = open.top();
        open.pop();

        if (closed.find(currentNode) == closed.end())
        {
            closed.insert(currentNode);
            if (currentNode->isGoalState())
                return currentNode;

            metrics.numExpandedNodes++;
            for (Node* child: currentNode->generateChildren())
            {
                child->calculateManhattanDistance();
                child->setCost(child->getHeuristicValue());
                open.push(child);
            }
                
        }
    }

    return std::nullopt;
}

struct CompareNodeAstar
{
    bool operator()(Node* const& n1, Node* const& n2)
    {
        int n1f = n1->getCost();
        int n2f = n2->getCost();

        if (n1f != n2f) return n1f > n2f;

        int n1g = n1->getHeuristicValue();
        int n2g = n2->getHeuristicValue();

        if (n1g != n2g) return n1g > n2g;

        return n1->getId() < n2->getId();
    }
};

std::optional<Node*> SearchAlgorithms::astar(Node& initialNode)
{
    clearMetrics();

    std::priority_queue<Node*, std::vector<Node*>, CompareNodeAstar> open;
    std::unordered_set<Node*, NodeHash, NodeEqual> closed;

    open.push(&initialNode);
    initialNode.calculateManhattanDistance();
    initialNode.setCost(initialNode.getHeuristicValue() + initialNode.getDepth());

    while(!open.empty())
    {
        Node* currentNode = open.top();
        open.pop();
        
        if (closed.find(currentNode) == closed.end())
        {
            closed.insert(currentNode);
            if (currentNode->isGoalState())
                return currentNode;

            metrics.numExpandedNodes++;
            for (Node* child: currentNode->generateChildren())
            {
                child->calculateManhattanDistance();
                child->setCost(child->getHeuristicValue() + child->getDepth());
                open.push(child);
            }
        }
    }

    return std::nullopt;
}

std::pair<int, std::optional<Node*>> SearchAlgorithms::depthLimitedIdastar(Node* node, int fLimit)
{
    node->calculateManhattanDistance();
    node->setCost(node->getHeuristicValue() + node->getDepth());

    if (node->getCost() > fLimit)
        return {node->getCost(), std::nullopt};

    if (node->isGoalState())
        return {fLimit, node};

    int nextLimit = MAX_INT;
    metrics.numExpandedNodes++;

    for (Node* child : node->generateChildren())
    {
        auto result = depthLimitedIdastar(child, fLimit);

        if (result.second.has_value())
            return result;

        nextLimit = std::min(nextLimit, result.first);
        delete child;
    }

    return {nextLimit, std::nullopt};
}

std::optional<Node*> SearchAlgorithms::idastar(Node& initialNode)
{
    clearMetrics();

    initialNode.calculateManhattanDistance();
    int fLimit = initialNode.getHeuristicValue();
    
    while (fLimit != MAX_INT)
    {
        std::pair<int, std::optional<Node*>> result = depthLimitedIdastar(&initialNode, fLimit);

        if (result.second.has_value())
            return result.second;

        fLimit = result.first;
        Node::desalocateAllNodes();
    }

    return std::nullopt;
}