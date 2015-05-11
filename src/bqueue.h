// -*- mode:c++ -*-

#ifndef SME_BQUEUE_H
#define SME_BQUEUE_H

#include <atomic>
#include <vector>
#include <mutex>
#include <condition_variable>

#include "sme.h"

using std::atomic_size_t;
using std::vector;

struct State {
  volatile std::atomic<int> loc = {0};
  int iterations = 0;
  volatile std::atomic<int> iter_end = {0}; // Number of threads that are idling in current it
  int threads = 1;
  int thread_loc[4] = {0,0,0,0};
  std::mutex count_mutex;
  std::condition_variable cv;
  std::condition_variable cv2;
  //bool block = true;
  };

//template<typename T>
class BQueue {
private:
  SyncProcess** els;
  Bus** busses;
  int size;
  int threads;
  //std::atomic<int>* iterations;
  //int iterations;
  //std::atomic<int>* iterations;
  State* state;

  //atomic_size_t loc;
  //std::atomic<int>* loc;
  int loc;

  // Lock function
  //Lock* lock;

  // State variables
  int subscribed;

  //int thread_loc[4] = {0,0,0,0};
  int thread_offsets[4] = {0,0,0,0};

  void stop();
public:
  BQueue(int threads, int iterations);
  ~BQueue();
  // TODO: Accept generic iterable container of parametric type T
  void populate(vector<SyncProcess*> els, map<Bus*> busses);
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
