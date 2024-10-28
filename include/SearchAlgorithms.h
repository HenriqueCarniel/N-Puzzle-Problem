/*
    SearchAlgorithms.h
    -------------------
    Este arquivo de cabeçalho define a classe `SearchAlgorithms`, responsável por implementar e executar diversos algoritmos 
    de busca para resolver puzzles, como o 8-puzzle e o 15-puzzle. A classe também gerencia as métricas de execução de cada 
    algoritmo para fins de análise e otimização.
*/

#ifndef SEARCH_ALGORITHMS
#define SEARCH_ALGORITHMS

#include "Node.h"
#include "InputHandler.h"
#include <deque>
#include <unordered_set>
#include <functional>
#include <chrono>
#include <optional>
#include <queue>
#include <limits>

struct SearchMetrics
{
    int numExpandedNodes = 0;
    int optimalSolutionLength = 0;
    double time = 0.0f;
    int initialValueHeuristic = 0;
};

class SearchAlgorithms
{
private:
    static const int MAX_INT = std::numeric_limits<int>::max();

    static void clearMetrics();
    static void setPathMetrics(Node* finalNode);

    static std::optional<Node*> bfsGraph(Node& initialNode);
    static std::optional<Node*> depthLimitedSearch(Node* node, int depthLimit);
    static std::optional<Node*> iterativeDeepeningSearch(Node& initialNode);
    static std::optional<Node*> greedyBestFirstSearch(Node& initialNode);
    static std::optional<Node*> astar(Node& initialNode);
    static std::pair<int, std::optional<Node*>> depthLimitedIdastar(Node* node, int fLimit);
    static std::optional<Node*> idastar(Node& initialNode);

public:
    static SearchMetrics metrics;
    static void runAlgorithm(Node& rootPuzzle, SearchAlgorithm type);
    static void printMetrics();
};

#endif