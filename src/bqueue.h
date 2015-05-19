// -*- mode:c++ -*-

#ifndef SME_BQUEUE_H
#define SME_BQUEUE_H

#include <atomic>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <set>

#include "sme.h"
#include "queue_common.h"

using std::atomic_size_t;
using std::vector;

//template<typename T>
class BQueue {
private:
  int size;
  int threads;

  SyncProcess** els;
  Bus** busses;
  State* state;

  //std::atomic<int>* loc;
  int loc;

  // State variables
  int* thread_offsets;

  void stop();
public:
  BQueue(int threads, int iterations);
  ~BQueue();
  // TODO: Accept generic iterable container of parametric type T
  void populate(vector<SyncProcess*> els, std::set<Bus*> busses);
  SyncProcess* next(int);
};

struct Timed {
  void next();
  void spinlock();
};

struct Untimed {
  void next();
  void spinlock();
};


#endif //SME_BQUEUE_H
