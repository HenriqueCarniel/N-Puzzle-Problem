#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <string>
#include <vector>

enum class SearchAlgorithm { BFS, IDFS, ASTAR, IDASTAR, GBFS, UNKNOWN };

class InputHandler
{
public:
    static SearchAlgorithm selectAlgorithm(const std::string& input);
    static std::vector<std::vector<int>> processInput(int argc, char* argv[]);

private:
    static std::vector<int> parseState(const std::string& state);
    static std::vector<std::vector<int>> readFromStdin();
};

#endif
