/*
    main.cpp
    --------
    Arquivo principal do programa que resolve os problemas de 8-puzzle e 15-puzzle utilizando diversos algoritmos de busca. 
    Esse programa inicializa o ambiente de execução, captura entradas e aplica o algoritmo selecionado para resolver o puzzle.

    Funcionalidades:
    - Configuração e execução do algoritmo de busca: Aceita o algoritmo de busca como argumento de entrada e instancia o puzzle de 
      acordo com o tamanho do estado inicial fornecido (8-puzzle ou 15-puzzle).
    - Controle de sinal SIGTERM: Implementa uma função `handle_sigterm` para desalocar memória e finalizar o programa 
      com segurança caso receba o sinal de término.
    - Processamento de Entrada: Usa `InputHandler` para processar a entrada do usuário, que pode ser feita por 
      argumentos no terminal ou via arquivo redirecionado.
    - Execução dos Algoritmos: Através de `SearchAlgorithms`, aplica o algoritmo de busca selecionado (como BFS, IDFS, A*, etc.)
      ao estado inicial do puzzle.
    - Relatórios de Métricas: Imprime métricas de desempenho ao final da execução de cada algoritmo e desaloca a memória de 
      todos os nós armazenados na lista global `AllNodes` para evitar vazamentos de memória.

    Parâmetros:
    - Argumentos via linha de comando: `-<search_algorithm>` (nome do algoritmo) e `<initial_states>` (opcional)
    - Alternativamente, um arquivo de entrada pode ser redirecionado, contendo o estado inicial.

    Dependências:
    - "Node8.h" e "Node15.h": Implementações específicas para os puzzles de 8 e 15 tiles, respectivamente.
    - "InputHandler.h": Responsável por selecionar o algoritmo e processar as entradas.
    - "ErrorCodes.h": Enumera os códigos de erro específicos para tratar exceções no programa.
    - "SearchAlgorithms.h": Define os algoritmos de busca que serão executados.
*/

#include <iostream>
#include <vector>
#include <csignal>
#include "Node8.h"
#include "Node15.h"
#include "InputHandler.h"
#include "ErrorCodes.h"
#include "SearchAlgorithms.h"

int HeuristicNumberCalls = 0;
double AverageValueHeuristic = 0.0f;
std::vector<Node*> AllNodes;

void handle_sigterm(int signum)
{
    Node::desalocateAllNodes();
    std::cout << "-,-,-,-,-" << std::endl;
    exit(signum);
}

int main(int argc, char* argv[])
{
    std::signal(SIGTERM, handle_sigterm);

    if (argc < 2)
    {
        std::cerr << "Use: ./main -<search_algorithm> <initial_states> or ./main -<search_algorithm> < <input_file>" << std::endl;
        return static_cast<int>(ErrorCode::INVALID_ARGUMENTS);
    }

    SearchAlgorithm search_algorithm = InputHandler::selectAlgorithm(argv[1]);
    if (search_algorithm == SearchAlgorithm::UNKNOWN)
    {
        std::cerr << "Unknown algorithm." << std::endl;
        return static_cast<int>(ErrorCode::UNKNOWN_ALGORITHM);
    }

    std::vector<std::vector<uint8_t>> initialStates = InputHandler::processInput(argc, argv);
    if (!initialStates.empty())
    {
        for (std::vector<uint8_t> initialState: initialStates)
        {
            size_t puzzleSize = initialState.size();
            if (puzzleSize == 16)
            {
                std::array<uint8_t, 16> arrayState;
                std::copy(initialState.begin(), initialState.end(), arrayState.begin());
                Node15 rootPuzzle(arrayState);
                SearchAlgorithms::runAlgorithm(rootPuzzle, search_algorithm);
            }
            else if (puzzleSize == 9)
            {
                std::array<uint8_t, 9> arrayState;
                std::copy(initialState.begin(), initialState.end(), arrayState.begin());
                Node8 rootPuzzle(arrayState);
                SearchAlgorithms::runAlgorithm(rootPuzzle, search_algorithm);
            }
            else
            {
                std::cerr << "Invalid puzzle size. Only 8-puzzle and 15-puzzle are supported." << std::endl;
                return static_cast<int>(ErrorCode::PUZZLE_SIZE);
            }

            SearchAlgorithms::printMetrics();
            Node::desalocateAllNodes();
        }
    }
    else
    {
        std::cerr << "No valid initial states provided." << std::endl;
        return static_cast<int>(ErrorCode::NO_INITIAL_STATES);
    }

    return 0;
}