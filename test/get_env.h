#ifndef SME_TEST_GET_ENV_H
#define SME_TEST_GET_ENV_H

#include <cstdlib>
#include <string>

/*inline bool test_cqueue(int flags) {
  return (flags >> 1) & 1 > 0;
}
inline bool test_bqueue(int flags) {
  return (flags >> 2) & 1 > 0;
  }*/

#define SME_TEST_DEFS int nodes = 0; int iterations = 0;                 \
  int flags = 0; bool test_cqueue = false; bool test_bqueue = false;

#define SME_TEST_GET_ENV                                                \
  char* nodes_s = std::getenv("SME_NODES");				\
  char* iterations_s = std::getenv("SME_ITERATIONS");			\
  char* flags_s = std::getenv("SME_SCHEDS");				\
  if(nodes_s != nullptr) nodes = std::stoi(nodes_s);			\
  if(iterations_s != nullptr) iterations = std::stoi(iterations_s);	\
  if(flags_s != nullptr) {						\
  flags = std::stoi(flags_s);						\
  test_bqueue = ((flags >> 1) & 1) > 0;                                 \
  test_cqueue = ((flags >> 2) & 1) > 0;}                                \
  std::cout << "Building network with " << nodes << " processes and running for " << iterations << " iterations\n";\
  if(test_cqueue) std::cout << "Testing using CQueue\n";		\
  if(test_bqueue) std::cout << "Testing using BQueue\n";

#endif
