#include <atomic>
#include <vector>
#include <chrono>
#include <thread>
#include <set>

#include "bqueue.h"
#include "sme.h"


/*class Barrier: public SyncProcess {
public:
  Barrier (volatile State* state, int busses_start, int busses_end)
    :SyncProcess("barrier", {}, {}), state{state} {}
private:
  volatile State* state;
protected:
  void step() {

    std::cout << "in barrier "<< state->iter_end << std::endl;
    // Am I the final process to reach the barrier?

    if (state->iter_end.fetch_add(1) == state->threads -1) {
      std::this_thread::sleep_for(std::chrono::seconds(10));
      state->iter_end.store(0);
      state->iterations--;
      //std::cout << "it " << state->iterations << std::endl;
      if (state->iterations > 0) {
      // If we still have iterations left, go back to first process
	//state->loc = 0;
	for (int i = 0; i < state->threads; i++) {
	  std::cout << "reset thread locs " << std::endl;
	  state->thread_loc[i] = 0;
	}
	// else, we let the threads continue into the nullptrs ahead
    }

      // Release all other barrier processes to do bus propagation
      for (int i = 0; i < 4; i++) {
	//std::cout << "release " << i << std::endl;
	state->block[i].clear();//std::memory_order_release);
      }
      state->block_count = 0;

      //std::this_thread::sleep_for(std::chrono::milliseconds(5));
    } else {
      //std::this_thread::sleep_for(std::chrono::milliseconds(5));

      //std::cout << "in lock" << std::endl;
      // Spin until lock flag is changed
      volatile std::atomic_flag* myflag = &state->block[state->block_count.fetch_add(1)];
      while(myflag->test_and_set());
      //while(state->block[state->block_count.fetch_add(1)].test_and_set(std::memory_order_acquire))
	;
      myflag->test_and_set();//std::memory_order_release);
    }
    // Do bus propagation here
  }
  };

class NOP: public SyncProcess {
public:
  NOP()
    :SyncProcess("nop", {}, {}) {}
protected:
  void step() {
    ;
  }
  };*/

class SingleReset : public SyncProcess {
public:
  SingleReset(int id, State* state)
    :SyncProcess("singlereset", {}, {}), id{id}, state{state} {
      this->state = state;
    }
private:
  int id;
  State* state;

protected:
  void step() {
    if(--(*state->iterations) != 0 && !(*state->halted)) {
      for(int i = 0; i < state->threads; i++) {
	state->thread_loc[i] = 0;
      }
    }
  }
};


class Reset : public SyncProcess {
public:
  Reset(int id, State* state)
    :SyncProcess("releaser", {}, {}), id{id}, state{state} {}

private:
  int id;
  State* state;

protected:
  void step() {
    std::unique_lock<std::mutex> lk(state->count_mutex);
    state->cv.wait(lk, [this]{return !(state->iter_end < state->threads - 1);});
    state->iter_end.store(0);
    if(--(*state->iterations) != 0 && !(*state->halted)) {
      for(int i = 0; i < state->threads; i++) {
	state->thread_loc[i] = 0;
      }
    }
    state->cv2.notify_all();
  }
};



BQueue::BQueue(int threads, int* iterations, bool* halted) {
  this->threads = threads;
  state = new State();
  state->halted = halted;
  state->iterations = iterations;
  state->threads = threads;
  state->thread_loc = new int[threads];
  thread_offsets = new int[threads];
  for (int i = 0; i < threads; i++) {
    state->thread_loc[i] = 0;
  }
}

void BQueue::populate(vector<SyncProcess*> new_els, std::set<Bus*> busses) {
  int extra_procs;
  if (threads == 1) {
    extra_procs = 3;
  } else {
    extra_procs = 4;
  }

  // Get busses
  int busses_count = busses.size();
  int busses_part = busses_count/threads;
  this->busses = new Bus*[busses_count];
  int i = 0;
  for(auto bus:busses) {
    this->busses[i++] = bus;
  }

  // Number of processes
  size = new_els.size();

  // ceil(size/threads)
  int part_ceil = (size + threads - 1)/threads;
  int part_floor = size/threads;

  // Allocate array size upper bound
  int c = (part_ceil + extra_procs) * threads;
  els = new SyncProcess* [c];

  int thread_num = 0;

  i = 0; //new_els counter
  int j = 0; //els counter
  int k = 0; //partition counter
  els[j++] = new_els[i++];
  for (;i < size; i++) {
    if (k <  threads - 1 && i % part_floor == 0) {
      els[j++] = new Locker(thread_num, state);
      els[j++] = new BusStep(state, this->busses, busses_part*k, busses_part*(k+1)-1);
      els[j++] = new Locker(thread_num++, state);
      els[j++] = nullptr;
      k++;
    }
    els[j++] = new_els[i];
  }
  if (threads > 1) {
    els[j++] = new Syncer(thread_num, state);
    els[j++] = new BusStep(state, this->busses, busses_part*k, busses_count-1);
    els[j++] = new Reset(thread_num, state);
  } else {
    els[j++] = new BusStep(state, this->busses, busses_part*k, busses_count-1);
    els[j++] = new SingleReset(thread_num, state);
  }
  els[j] = nullptr;

  // Set thread offsets
  for(int i = 0; i < threads; i++) {
    thread_offsets[i] = (part_floor + extra_procs)*i;
  }
}

SyncProcess* BQueue::next(int id) {
  int index=state->thread_loc[id]++ + thread_offsets[id];
  return els[index];
}

BQueue::~BQueue() {
  delete[] els;
}
