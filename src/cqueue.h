// -*- mode:c++ -*-

#ifndef SME_CQUEUE_H
#define SME_CQUEUE_H

#include <atomic>
#include <vector>
#include <condition_variable>
#include <set>

#include "sme.h"
#include "queue_common.h"

/*struct State {
  volatile std::atomic<int> loc = {0};
  //volatile std::atomic<int> iterations = {0};
  int iterations = 0;
  volatile std::atomic<int> iter_end = {0}; // Number of threads that are idling in current it
  int threads = 1;
  //std::atomic_flag block = ATOMIC_FLAG_INIT;
  std::atomic_flag block[4] = {ATOMIC_FLAG_INIT, ATOMIC_FLAG_INIT, ATOMIC_FLAG_INIT, ATOMIC_FLAG_INIT};
  volatile std::atomic<int> block_count = {0};
  //bool block = true;
  };*/

class CQueue {

private:
  SyncProcess** els;
  int size;
  int threads;
  State* state;

  int loc;
  Bus** busses;

  int subscribed;
public:
  CQueue(int threads, int* iterations, bool* halted);
  ~CQueue();

  // TODO: Accept generic iterable container of parametric type T
  void populate(vector<SyncProcess*> els, std::set<Bus*> busses);

  SyncProcess* next(int);
};

#endif //SME_CQUEUE_H
