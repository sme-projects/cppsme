#include <atomic>
#include <vector>
#include <chrono>
#include <thread>

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
    if(--state->iterations > 0) {
      for(int i = 0; i < state->threads; i++) {
	state->thread_loc[i] = 0;
      }
    }
    state->cv2.notify_all();
  }
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
  int bend;
  int bstart;
  int i;
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
  BusStep(State* state, Bus** busses, int bstart, int bend)
    :SyncProcess("bussetp", {}, {}), state{state},busses{busses}, bstart{bstart}, bend{bend} {}
private:
  State* state;
  Bus** busses;
  int bend;
  int bstart;
  int i;
protected:
  void step() {
    for(i=bstart; i <= bend; i++) {
      busses[i]->step();
    }
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
};


BQueue::BQueue(int threads, int iterations) {
  this->threads = threads;
  state = new State();
  state->iterations = iterations;
  state->threads = threads;
  for (int i = 0; i < 4; i++) {
  //  std::cout << "release " << i << std::endl;
  //  state->block[i].test_and_set(std::memory_order_acquire);
    state->block[i] = true;
  }
}

void BQueue::populate(vector<SyncProcess*> new_els, set<Bus*> busses) {
  constexpr int extra_procs = 4;

  // Get busses
  int busses_count = busses.size();
  int busses_part = busses_count/threads;
  this->busses = new Bus*[busses_num];
  i = 0;
  for(auto bus:busses) {
    this->busses[i++] = bus;
  }

  // Number of processes
  size = new_els.size();

  //How many threads to partition into

  // ceil(size/threads)
  int part_ceil = (size + threads - 1)/threads;
  int part_floor = size/threads;

  // Allocate array size upper bound
  int c = (part_ceil + extra_procs) * threads;
  els = new SyncProcess* [c];

  int thread_num = 0;

  int i = 0; //new_els counter
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
  els[j++] = new Syncer(thread_num, state);
  els[j++] = new BusStep(state, this->busses, busses_part*k, busses_count-1);
  els[j++] = new Reset(thread_num, state)
  els[j] = nullptr;

  // Set thread offsets
  for(int i = 0; i < threads; i++) {
    thread_offsets[i] = (part_floor + 2)*i;
  }

}

SyncProcess* BQueue::next(int id) {
  return els[state->thread_loc[id]++ + thread_offsets[id]];
}

BQueue::~BQueue() {
  delete[] els;
}
