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

struct SearchMetrics
{
    int numExpandedNodes = 0;
    int optimalSolutionLength = 0;
    double time = 0.0f;
    double averageValueHeuristic = 0.0f;
    int initialValueHeuristic = 0;
};

class SearchAlgorithms
{
private:
    static SearchMetrics metrics;

    static void clearMetrics();
    static void setPathMetrics(Node* finalNode, bool needAverageValueHeuristic);

    static std::optional<Node*> bfsGraph(Node& initialNode);
    static std::optional<Node*> depthLimitedSearch(Node* initialNode, int depthLimit);
    static std::optional<Node*> iterativeDeepeningSearch(Node& initialNode);
    static std::optional<Node*> greedyBestFirstSearch(Node& initialNode);

public:
    static void runAlgorithm(Node initialState, SearchAlgorithm type);
    static void printMetrics();
};

#endif