/*
    InputHandler.h
    ---------------
    Este arquivo contém a definição da classe `InputHandler` e do enum `SearchAlgorithm`, 
    utilizados para gerenciar a entrada do programa e selecionar o algoritmo de busca adequado 
    com base nos argumentos do usuário.

    Esta estrutura modulariza o processamento de entrada e a seleção de algoritmos, permitindo um tratamento 
    eficiente dos argumentos do programa e oferecendo uma base para a expansão de algoritmos de busca.
*/

#ifndef INPUT_HANDLER_H
#define INPUT_HANDLER_H

#include <string>
#include <vector>

enum class SearchAlgorithm { BFS, IDFS, ASTAR, IDASTAR, GBFS, UNKNOWN };

class InputHandler
{
public:
    static SearchAlgorithm selectAlgorithm(const std::string& input);
    static std::vector<std::vector<uint8_t>> processInput(int argc, char* argv[]);

private:
    static std::vector<uint8_t> parseState(const std::string& state);
    static std::vector<std::vector<uint8_t>> readFromStdin();
};

#endif