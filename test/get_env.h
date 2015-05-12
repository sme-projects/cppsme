#ifndef SME_TEST_GET_ENV_H
#define SME_TEST_GET_ENV_H

#include <cstdlib>
#include <string>

#define SME_TEST_GET_ENV int nodes = 0; int iterations = 0;		\
  char* nodes_s = std::getenv("SME_NODES");				\
  char* iterations_s = std::getenv("SME_ITERATIONS");			\
  if(nodes_s != nullptr) nodes = std::stoi(nodes_s);			\
  if(iterations_s != nullptr) iterations = std::stoi(iterations_s);	\
  std::cout << "Building network with " << nodes << " processes and running for " << iterations << " iterations\n";

#endif
