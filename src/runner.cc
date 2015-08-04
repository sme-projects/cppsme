#include <vector>
#include <chrono>

#include "sme.h"
#include "runner.h"

using std::vector;

Runner::Runner(int steps)
  :steps{steps} {}

void Runner::add_proc(Objects list) {
  for (auto e:list) {
    add_proc(e);
  }
}

void Runner::add_proc(SyncObject* proc) {
  auto ins = proc->get_ins();
  auto outs = proc->get_outs();

  for (Bus* p: ins) {
    busses.insert(p);
  }
  for (Bus* p: outs) {
    busses.insert(p);
  }

  // FIXME: We don't support all possible types here
  // and the static_cast shouldn't be there
  append_procs(static_cast<SyncProcess*>(proc));
}

void Runner::append_procs(SyncProcess* p) {
  procs.push_back(p);
}

void Runner::append_procs(vector<SyncComponent*> proc) {
  for (SyncProcess* p: procs) {
    append_procs(p);
  }
}

vector<SyncProcess*> Runner::get_proc() {
  return procs;
}

void Runner::start() {
  std::cout << "oldver\n";
  int i;
  auto start = std::chrono::high_resolution_clock::now();
  for(i = 0; i < steps; i++){
    for (SyncProcess* e:this->procs) {
      e->step();
    }
    // TODO: Decide on bus handling implementation
    for (Bus* b: busses){
      b->step();
    }
    //Bus::swapval();
  }
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<double> diff = end-start;
  std::cout << "Executed network in " << diff.count() << "s\n";
  for (SyncProcess* e: procs) {
    delete e;
  }
}
