#ifndef SME_QUEUE_COMMON_H
#define SME_QUEUE_COMMON_H

#include <atomic>
#include <mutex>
#include <condition_variable>

#include "sme.h"

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

class Syncer : public SyncProcess {
public:
  Syncer(int id, State* state)
    :SyncProcess("locker", {}, {}), id{id}, state{state} {}

private:
  int id;
  State* state;
protected:
  void step() {
    std::unique_lock<std::mutex> lk(state->count_mutex);
    state->cv.wait(lk, [this]{return !(state->iter_end < state->threads - 1);});
    state->iter_end.store(0);
    state->cv2.notify_all();
  }
};


class Locker : public SyncProcess {
public:
  Locker(int id, State* state)
    :SyncProcess("locker", {}, {}), id{id}, state{state} {}

private:
  int id;
  State* state;
protected:
  void step() {
    std::unique_lock<std::mutex> lk(state->count_mutex);
    ++state->iter_end;
    state->cv.notify_all();
    state->cv2.wait(lk);
  }
};

class BusStep : public SyncProcess {
public:
  BusStep(State* state, Bus** busses, int start, int end)
    :SyncProcess("bussetp", {}, {}), state{state},busses{busses}, start{start}, end{end} {}
private:
  State* state;
  Bus** busses;
  int start = 0;
  int end = 0;
  int i;
protected:
  void step() {
    for(i=start; i <= end; i++) {
      busses[i]->step();
    }
  }
};


#endif
