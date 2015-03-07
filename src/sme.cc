// SME


#include <iostream>
#include <initializer_list>
#include <vector>
#include <set>

#include "sme.h"

SyncProcess::SyncProcess(const std::string name,
			 std::initializer_list<Bus*> inputs,
			 std::initializer_list<Bus*> outputs)
  :name{name} {
  for(Bus* b : inputs){
    ins.push_back(b);
  }
  for(Bus* b : outputs){
    outs.push_back(b);
  }
}

SyncProcess::~SyncProcess() {}

Bus* SyncProcess::get_in(int k) {
  return ins[k];
}

Bus* SyncProcess::get_out(int k) {
  return outs[k];
}

vector<Bus*> SyncProcess::get_ins() {
  return ins;
}

vector<Bus*> SyncProcess::get_outs() {
  return outs;
}

Run::Run(int steps)
  :steps{steps} {}

void Run::add_proc(std::initializer_list<SyncProcess*> list) {
  for (auto e:list) {
    add_proc(e);
  }
}

void Run::add_proc(SyncProcess* p) {
  procs.push_back(p);
  auto ins = p->get_ins();
  auto outs = p->get_outs();

  for (Bus* p: ins) {
    busses.insert(p);
  }
  for (Bus* p: outs) {
    busses.insert(p);
  }
}

void Run::start() {
  int i;
  for(i = 0; i < steps; i++){
    for (Bus* b: busses){
      b->step();
    }
    for (SyncProcess* e:this->procs) {
      e->step();
    }
  }
  for (SyncProcess* e: procs) {
    delete e;
  }
}

void Bus::step() {
  _out = _in;
}

int Bus::read(){
  return _out;
}

void Bus::write(int v) {
  _in = v;
}

//
