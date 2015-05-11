// -*- mode: c++ -*-
// Define interfaces for network runners

#ifndef SME_RUNNER_H
#define SME_RUNNER_H

class Runner {
protected:
  int steps;
  std::vector<SyncProcess*> procs;
  std::set<Bus*> busses;

public:
  Runner(int steps);
  virtual void start();
  void add_proc(std::initializer_list<SyncObject*>);
  void add_proc(SyncObject*);
  void append_procs(SyncProcess*);
  void append_procs(vector<SyncComponent*>);
  std::vector<SyncProcess*> get_proc();
};


#endif //SME_RUNNER_H
