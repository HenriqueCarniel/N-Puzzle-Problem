#include "SearchAlgorithms.h"

SearchMetrics SearchAlgorithms::metrics;

std::function<int(const std::vector<int>&, const int)> SearchAlgorithms::costFunctionBFS = 
    [](const std::vector<int>&, const int) -> int { return 0; };
std::function<int(const std::vector<int>&, const int)> SearchAlgorithms::costFunctionIDFS = 
    [](const std::vector<int>&, const int) -> int { return 0; };
std::function<int(const std::vector<int>&, const int)> SearchAlgorithms::costFunctionGBFS = 
    [](const std::vector<int>& state, const int) -> int { return Node(state).calculateManhattanDistance(); };
std::function<int(const std::vector<int>&, const int)> SearchAlgorithms::costFunctionASTAR = 
    [](const std::vector<int>& state, const int depth) -> int { return Node(state).calculateManhattanDistance() + depth; };

void SearchAlgorithms::clearMetrics()
{
    metrics = SearchMetrics();
}

void SearchAlgorithms::setPathMetrics(Node* finalNode, bool needAverageValueHeuristic)
{
    if (finalNode == nullptr)
        return;

    int pathLength = 0;
    int sumHeuristcValue = 0;
    Node* currentNode = finalNode;

    if (needAverageValueHeuristic)
    {
        while (currentNode->getParent() != nullptr)
        {
            pathLength++;
            sumHeuristcValue += currentNode->calculateManhattanDistance();
            currentNode = currentNode->getParent();
        }
    }
    else
    {
        while (currentNode->getParent() != nullptr)
        {
            pathLength++;
            currentNode = currentNode->getParent();
        }
    }
    
    metrics.optimalSolutionLength = pathLength;
    metrics.initialValueHeuristic = currentNode->calculateManhattanDistance();
    metrics.averageValueHeuristic = static_cast<float>(sumHeuristcValue) / pathLength;
}

void SearchAlgorithms::printMetrics()
{
    std::cout << metrics.numExpandedNodes << ","
        << metrics.optimalSolutionLength << ","
        << metrics.time << ","
        << metrics.averageValueHeuristic << ","
        << metrics.initialValueHeuristic << std::endl;
}

void SearchAlgorithms::runAlgorithm(Node rootPuzzle, SearchAlgorithm type)
{
    std::optional<Node*> response;
    bool needAverageValueHeuristic = true;
    auto start = std::chrono::high_resolution_clock::now();

    if (type == SearchAlgorithm::BFS)
    {
        response = SearchAlgorithms::bfsGraph(rootPuzzle);
        needAverageValueHeuristic = false;
    }
    else if (type == SearchAlgorithm::IDFS)
    {
        response = SearchAlgorithms::iterativeDeepeningSearch(rootPuzzle);
        needAverageValueHeuristic = false;
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

    metrics.time = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now() - start).count() / 1000.0;
    if (response.has_value())
        setPathMetrics(response.value(), needAverageValueHeuristic);
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