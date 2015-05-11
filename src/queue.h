// -*- mode:c++ -*-

#ifndef SME_QUEUE_H
#define SME_QUEUE_H

#include <atomic>
#include <vector>

#include "sme.h"

using std::atomic_size_t;
using std::vector;

typedef int (*fptr)();

struct State {
  volatile std::atomic<int> loc = {0};
  //volatile std::atomic<int> iterations = {0};
  int iterations = 0;
  volatile std::atomic<int> iter_end = {0}; // Number of threads that are idling in current it
  int threads = 1;
  //std::atomic_flag block = ATOMIC_FLAG_INIT;
  std::atomic_flag block[4] = {ATOMIC_FLAG_INIT, ATOMIC_FLAG_INIT, ATOMIC_FLAG_INIT, ATOMIC_FLAG_INIT};
  volatile std::atomic<int> block_count = {0};
  //bool block = true;
};

//template<typename T>
class CQueue {
private:
  SyncProcess** els;
  int size;
  int threads;
  //std::atomic<int>* iterations;
  //int iterations;
  //std::atomic<int>* iterations;
  volatile State* state;

  //atomic_size_t loc;
  //std::atomic<int>* loc;
  int loc;

  // Lock function
  //Lock* lock;

  // State variables
  int subscribed;

  void stop();
public:
  CQueue(int threads, int iterations);
  ~CQueue();
  // TODO: Accept generic iterable container of parametric type T
  void populate(vector<SyncProcess*> els);
  SyncProcess* next();
};

struct Timed {
  void next();
  void spinlock();
};

struct Untimed {
  void next();
  void spinlock();
};


#endif //SME_QUEUE_H
