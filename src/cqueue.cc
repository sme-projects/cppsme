// -*- mode:c++ -*-

//#include <atomic>
//#include <vector>

#include "cqueue.h"
#include "queue_common.h"
#include "sme.h"

/*class Barrier: public SyncProcess {
public:
  Barrier (const std::string name, Busses ins, Busses outs,
	   volatile State* state, int busses_start, int busses_end)
    :SyncProcess(name, ins, outs), state{state} {}
private:
  volatile State* state;
protected:
  void step() {
    //std::cout << "in barrier" << std::endl;
    // Am I the final process to reach the barrier?
  if (state->iter_end.fetch_add(1) == state->threads -1) {
    state->iterations--;
    //std::cout << "it " << state->iterations << std::endl;
    if (state->iterations > 0) {
      // If we still have iterations left, go back to first process
      state->loc = 0;
      // else, we let the threads continue into the nullptrs ahead
    }
    state->iter_end = 0;
    // Release all other barrier processes to do bus propagation
    for (int i = 0; i < 4; i++) {
      //std::cout << "release " << i << std::endl;
      state->block[i].clear(std::memory_order_release);
    }
    state->block_count = 0;
  } else {
    //std::cout << "in lock" << std::endl;
    // Spin until lock flag is changed
    volatile std::atomic_flag* myflag = &state->block[state->block_count.fetch_add(1)];
    while(myflag->test_and_set(std::memory_order_acquire))
	  ;
    myflag->test_and_set(std::memory_order_release);
  }
  // Do bus propagation here
  }
  };*/


/*class SpinLock: public SyncProcess {
public:
  SpinLock(const std::string name, Busses ins, Busses outs,
	   volatile State* state)
  :SyncProcess(name, ins, outs), state{state} {}

private:
  volatile State* state;
protected:
  void step() {
    //Enter spinlock

    std::cout << "spinlock " << state->loc << std::endl;

    volatile std::atomic_flag* myflag = &state->block[state->block_count.fetch_add(1)];
    while(myflag->test_and_set(std::memory_order_acquire))
      ;
    myflag->test_and_set(std::memory_order_release);
  }
};

class Reset: public SyncProcess {
public:
  Reset(const std::string name, Busses ins, Busses outs,
	 volatile State* state)
    :SyncProcess(name, ins, outs), state{state} {}
private:
  volatile State* state;
protected:
  void step() {
    state->iterations--;
    //std::cout << "it " << state->iterations << std::endl;
    if (state->iterations > 0) {
      // If we still have iterations left, go back to first process
      std::cout << "reset " << state->loc << std::endl;

      state->loc = 0;
      // else, we let the threads continue into the nullptrs ahead
    }
    //state->iter_end = 0;
    //std::cout << "iter end " << state->iter_end << std::endl;
    // Release all other barrier processes to do bus propagation
    for (int i = 0; i < 4; i++) {
      //std::cout << "release " << i << std::endl;
      state->block[i].clear(std::memory_order_release);
    }
    state->block_count = 0;

  }
  };*/


class CQueue_Reset : public SyncProcess {
public:
  CQueue_Reset(int id, State* state)
    :SyncProcess("reset", {}, {}), id{id}, state{state} {}

private:
  int id;
  State* state;

protected:
  void step() {
    std::unique_lock<std::mutex> lk(state->count_mutex);
    state->cv.wait(lk, [this]{return !(state->iter_end < state->threads - 1);});
    state->iter_end.store(0);
    if(--state->iterations > 0) {
      state->loc.store(0);
    }
    state->cv2.notify_all();
  }
};

class CQueue_SingleReset : public SyncProcess {
public:
  CQueue_SingleReset(State* state)
    :SyncProcess("singlereset", {}, {}), state{state} {}

private:
  State* state;

protected:
  void step() {
    if(--state->iterations > 0) {
      state->loc.store(0);
    }
  }
};

CQueue::CQueue(int threads, int iterations) {
  this->threads = threads;
  state = new State();
  state->iter_end.store(0);
  state->iterations = iterations;
  state->threads = threads;

}

// TODO: Accept generic iterable types. Corresponding note in h file
void CQueue::populate(vector<SyncProcess*> new_els, std::set<Bus*> busses){
  int extra_procs;
  if (threads == 1) {
    extra_procs = 3;
  } else {
    extra_procs = 4;
  }

  size = new_els.size();

  int i = 0;
  int busses_count = busses.size();
  // Populate array with busses
  this->busses = new Bus*[busses_count];
  for (auto bus:busses) {
    this->busses[i++] = bus;
  }

  i = 0;

  // Allocate array of pointers to objects in the queue
  els = new SyncProcess* [size + threads*extra_procs];

  // Copy function references to array
  for(auto el: new_els) {
    els[i++] = el;
  }
  // We only need to sync before propagating busses when using
  // multithreaded mode
  if (threads > 1) {
    for(i = size; i < size + threads - 1; i++) {
      els[i] = new Locker(0, state);
    }
    els[i++] = new Syncer(0, state);
  }
  // Add bus steppers
  int busses_part = busses_count/threads;
  volatile int j = 0;
  for (; j < threads - 1; j++) {
    els[i++] = new BusStep(state, this->busses, busses_part*j, busses_part*(j+1)-1);
  }
  els[i++] = new BusStep(state, this->busses, busses_part*j, busses_count - 1);
  // Add final sync step

  for(j = 0; j < threads - 1; j++) {
    els[i++] = new Locker(0, state);
  }
  if (threads > 1) {
      els[i++] = new CQueue_Reset(0, state);
  } else {
    els[i++] = new CQueue_SingleReset(state);
  }
  // Place a nullpointer at the end of array to make threads terminate
  for(j = 0; j < threads - 1; j++) {
    els[i++] = nullptr;
  }
  els[i] = nullptr;
}

//template <typename T>
SyncProcess* CQueue::next(int i) {
  //std::cout << state->loc.fetch_add(1) << std::endl;
  return els[state->loc.fetch_add(1, std::memory_order_relaxed)];
}

//template <typename T>
CQueue::~CQueue() {
  delete[] els;


}
