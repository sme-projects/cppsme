// -*- mode:c++ -*-

// Simple thread-safe lock-free circular queue. will optionally block
// until all threads has finished executing before wrapping

// Statement: By letting last n elements (where n is the number of
// threads) of the queue be a blocking (spinlock) function which
// returns the length of time they have been blocking. Can we then
// always ensure that those threads are spread evenly over the
// CPU-threads

#include <atomic>
#include <vector>

#include "queue.h"
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


class SpinLock: public SyncProcess {
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
};


CQueue::CQueue(int threads, int iterations) {
  this->threads = threads;
  state = new State();
  state->iterations = iterations;
  state->threads = threads;
  for (int i = 0; i < 4; i++) {
    std::cout << "release " << i << std::endl;
    state->block[i].test_and_set(std::memory_order_acquire);
    //state->block[i] = true;
  }

}

// TODO: Accept generic iterable types. Corresponding note in h file
void CQueue::populate(vector<SyncProcess*> new_els){
  // XXX. Look this up
  size = new_els.size();
  std::cout << size
	    << std::endl;
  int i = 0;

  // Allocate array of pointers to objects in the queue
  els = new SyncProcess* [size + threads*2];

  // Copy function references to array
  for(auto el: new_els) {
    els[i] = el;
    ++i;
  }
  for(int i = 0; i < threads - 1; i++) {
    els[size + i] = new SpinLock("lock", {}, {}, state);
  }
  els[size + threads - 1] = new SpinLock("reset", {}, {}, state);
  //for(int i = 0; i < threads; i++) {
    // FIXME: Memory leak
    //els[size + i] = new Barrier("barrier", {}, {}, state, 0, 0);
  //}
  for(int i = threads; i < threads*2; i++) {
    els[size + i] = nullptr;
  }
}

//template <typename T>
SyncProcess* CQueue::next() {
  //std::cout << state->loc.fetch_add(1) << std::endl;
  return els[state->loc.fetch_add(1)];
}

//template <typename T>
CQueue::~CQueue() {
  delete[] els;


}
