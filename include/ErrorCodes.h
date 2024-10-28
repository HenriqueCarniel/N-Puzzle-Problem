/*
  ErrorCodes.h
  ------------
  Este arquivo contém a definição do enum class `ErrorCode`, que representa 
  os diferentes códigos de erro usados pelo programa para lidar com falhas 
  específicas durante a execução. Cada código de erro é associado a uma situação 
  de erro que pode ocorrer em diferentes etapas do programa.

  Códigos de erro:
  - INVALID_ARGUMENTS (1): Indica que os argumentos fornecidos ao programa são inválidos 
    ou insuficientes.
  - UNKNOWN_ALGORITHM: Indica que o algoritmo solicitado não é reconhecido pelo programa.
  - NO_INITIAL_STATES: Representa a ausência de estados iniciais quando necessário para a execução.
  - PUZZLE_SIZE: Indica um erro relacionado ao tamanho do puzzle, geralmente em casos onde o 
    tamanho do puzzle não é suportado.

  Esses códigos são usados para melhorar o tratamento de erros, oferecendo mensagens 
  mais informativas e facilitando a depuração.
*/

#ifndef ERRORCODES_H
#define ERRORCODES_H

enum class ErrorCode
{
  INVALID_ARGUMENTS = 1,
  UNKNOWN_ALGORITHM,
  NO_INITIAL_STATES,
  PUZZLE_SIZE
};

#endif