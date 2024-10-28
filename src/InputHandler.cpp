/*
    InputHandler.cpp
    ----------------
    Este arquivo implementa a classe `InputHandler`, que lida com o processamento e 
    validação da entrada para os algoritmos de busca. A `InputHandler` é responsável 
    por selecionar o algoritmo com base nos argumentos do usuário, bem como por 
    processar a entrada de estados do puzzle, seja via terminal ou redirecionamento 
    de arquivo.

    O `InputHandler` é estruturado para ser modular e adaptável a várias fontes de entrada, 
    facilitando a adição de novos tipos de algoritmo ou ajustes na leitura de estados.
*/

#include "InputHandler.h"
#include <sstream>
#include <iostream>
#include <unistd.h> // for isatty, fileno

SearchAlgorithm InputHandler::selectAlgorithm(const std::string& input)
{
    if (input == "-bfs") return SearchAlgorithm::BFS;
    if (input == "-idfs") return SearchAlgorithm::IDFS;
    if (input == "-astar") return SearchAlgorithm::ASTAR;
    if (input == "-idastar") return SearchAlgorithm::IDASTAR;
    if (input == "-gbfs") return SearchAlgorithm::GBFS;
    return SearchAlgorithm::UNKNOWN;
}

std::vector<uint8_t> InputHandler::parseState(const std::string& state)
{
    std::vector<uint8_t> parsedState;
    std::stringstream ss(state);
    int value;

    while (ss >> value)
    {
        parsedState.push_back(value);
        if (ss.peek() == ' ') ss.ignore();
    }

    return parsedState;
}

std::vector<std::vector<uint8_t>> InputHandler::readFromStdin()
{
    std::vector<std::vector<uint8_t>> states;
    std::string line;

    while (std::getline(std::cin, line))
    {
        std::vector<uint8_t> parsedState = InputHandler::parseState(line);
        states.push_back(parsedState);
    }

    return states;
}

std::vector<std::vector<uint8_t>> InputHandler::processInput(int argc, char* argv[])
{
    std::vector<std::vector<uint8_t>> initialStates;

    // If input is via stdin (redirected file)
    if (!isatty(fileno(stdin)))
    {
        initialStates = InputHandler::readFromStdin();
        return initialStates;
    }

    // If the input is standard, via terminal
    std::string statesInput;
    for (int i = 2; i < argc; ++i)
    {
        statesInput += argv[i];
        if (i < argc - 1)
            statesInput += " ";
    }

    std::stringstream stateStream(statesInput);
    std::string state;
    while (std::getline(stateStream, state, ','))
    {
        std::vector<uint8_t> parsedState = InputHandler::parseState(state);
        initialStates.push_back(parsedState);
    }

    return initialStates;
}