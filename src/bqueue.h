// -*- mode:c++ -*-

#ifndef SME_BQUEUE_H
#define SME_BQUEUE_H

#include <atomic>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <set>

#include "sme.h"

using std::atomic_size_t;
using std::vector;

struct State {
  volatile std::atomic<int> loc = {0};
  int iterations = 0;
  volatile std::atomic<int> iter_end = {0}; // Number of threads that are idling in current it
  int threads = 1;
  //int thread_loc[4] = {0,0,0,0};
  int* thread_loc;
  std::mutex count_mutex;
  std::condition_variable cv;
  std::condition_variable cv2;
  };

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
  int thread_offsets[4] = {0,0,0,0};

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
