#include "SearchAlgorithms.h"

SearchMetrics SearchAlgorithms::metrics;

std::function<int(const std::array<uint8_t, 9>&, const int)> SearchAlgorithms::costFunctionBFS = 
    [](const std::array<uint8_t, 9>&, const int) -> int { return 0; };
std::function<int(const std::array<uint8_t, 9>&, const int)> SearchAlgorithms::costFunctionIDFS = 
    [](const std::array<uint8_t, 9>&, const int) -> int { return 0; };
std::function<int(const std::array<uint8_t, 9>&, const int)> SearchAlgorithms::costFunctionGBFS = 
    [](const std::array<uint8_t, 9>& state, const int) -> int { return Node::calculateManhattanDistanceStatic(state); };
std::function<int(const std::array<uint8_t, 9>&, const int)> SearchAlgorithms::costFunctionASTAR = 
    [](const std::array<uint8_t, 9>& state, const int depth) -> int { return Node::calculateManhattanDistanceStatic(state) + depth; };

void SearchAlgorithms::clearMetrics()
{
    metrics = SearchMetrics();
    heuristicNumberCalls = 0;
    averageValueHeuristic = 0;
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
    metrics.initialValueHeuristic = currentNode->calculateManhattanDistance();
}

void SearchAlgorithms::printMetrics()
{
    std::cout << metrics.numExpandedNodes << ","
        << metrics.optimalSolutionLength << ","
        << metrics.time << ","
        << averageValueHeuristic / heuristicNumberCalls << ","
        << metrics.initialValueHeuristic << std::endl;
}

void SearchAlgorithms::runAlgorithm(Node rootPuzzle, SearchAlgorithm type)
{
    std::optional<Node*> response;
    auto start = std::chrono::high_resolution_clock::now();

    if (type == SearchAlgorithm::BFS)
    {
        response = SearchAlgorithms::bfsGraph(rootPuzzle);
    }
    else if (type == SearchAlgorithm::IDFS)
    {
        response = SearchAlgorithms::iterativeDeepeningSearch(rootPuzzle);
    }
    else if (type == SearchAlgorithm::ASTAR)
    {
        response = SearchAlgorithms::astar(rootPuzzle);
    }
    else if (type == SearchAlgorithm::IDASTAR)
    {
        // TODO
    }
    else if (type == SearchAlgorithm::GBFS)
    {
        response = SearchAlgorithms::greedyBestFirstSearch(rootPuzzle);
    }

    auto end = std::chrono::high_resolution_clock::now();
    metrics.time = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count() / 1000000.0;
    if (response.has_value())
        setPathMetrics(response.value());
}

struct NodeHash
{
    size_t operator()(const Node* node) const
    {
        size_t hash = 0;
        for (int num : node->getState())
            hash ^= std::hash<int>()(num) + 0x9e3779b9 + (hash << 6) + (hash >> 2);
        return hash;
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

    std::deque<Node*> open;
    open.push_back(&initialNode);

    std::unordered_set<Node*, NodeHash, NodeEqual> closed;
    closed.insert(&initialNode);

    while (!open.empty())
    {
        Node* currentNode = open.front();
        open.pop_front();
        metrics.numExpandedNodes++;

        for (Node* child: currentNode->generateChildren(costFunctionBFS))
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

std::optional<Node*> SearchAlgorithms::depthLimitedSearch(Node* initialNode, int depthLimit)
{
    if (initialNode->isGoalState())
        return initialNode;

    if (depthLimit > 0)
    {
        metrics.numExpandedNodes++;
        for (Node* child: initialNode->generateChildren(costFunctionIDFS))
        {
            std::optional<Node*> solution = depthLimitedSearch(child, depthLimit - 1);
            if (solution.has_value())
                return solution;
        }
    }

    return std::nullopt;
}

std::optional<Node*> SearchAlgorithms::iterativeDeepeningSearch(Node& initialNode)
{
    clearMetrics();

    for (int depthLimit = 0; ; ++depthLimit)
    {
        std::optional<Node*> solution = depthLimitedSearch(&initialNode, depthLimit);
        if (solution.has_value())
            return solution;
    }

    return std::nullopt;
}

struct CompareNodeGbfs
{
    bool operator()(Node* const& n1, Node* const& n2)
    {
        int n1f = n1->getCost();
        int n2f = n2->getCost();

        if (n1f == n2f)
        {
            int n1g = n1->getDepth();
            int n2g = n2->getDepth();

            if (n1g == n2g)
            {
                int n1id = n1->getId();
                int n2id = n2->getId();
                
                return n1id < n2id;
            }
            return n1g < n2g;
        }
        
        return n1f > n2f;
    }
};

std::optional<Node*> SearchAlgorithms::greedyBestFirstSearch(Node& initialNode)
{
    clearMetrics();

    std::priority_queue<Node*, std::vector<Node*>, CompareNodeGbfs> open;
    std::unordered_set<Node*, NodeHash, NodeEqual> closed;

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
            for (Node* child: currentNode->generateChildren(costFunctionGBFS))
                open.push(child);
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

        if (n1f == n2f)
        {
            int n1g = n1->calculateManhattanDistance();
            int n2g = n2->calculateManhattanDistance();

            if (n1g == n2g)
            {
                int n1id = n1->getId();
                int n2id = n2->getId();
                
                return n1id < n2id;
            }

            return n1g > n2g;
        }
        
        return n1f > n2f;
    }
};

std::optional<Node*> SearchAlgorithms::astar(Node& initialNode)
{
    clearMetrics();

    std::priority_queue<Node*, std::vector<Node*>, CompareNodeAstar> open;
    std::unordered_set<Node*, NodeHash, NodeEqual> closed;

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
            for (Node* child: currentNode->generateChildren(costFunctionASTAR))
                open.push(child);
        }
    }

    return std::nullopt;
}