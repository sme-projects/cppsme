// SME


#include <iostream>
#include <initializer_list>
#include <vector>
#include <set>
#include <map>

//#include "threads.h"
#include "sme.h"

SyncProcess::SyncProcess(const std::string name,
			 Busses inputs,
			 Busses outputs)
  :name{name} {
  // TODO: Is it possible to validate that the bus pointers
  //       are pointing to an actual instance of Bus()?
  //       We're getting some wierd errors if thats not the case.
  for(Bus* b : inputs){
    ins.push_back(b);
    if (b->is_named()) {
      ins_map.insert({b->get_name(), b});
    }
  }
  for(Bus* b : outputs){
    outs.push_back(b);
    if (b->is_named()) {
      outs_map.insert({(b->get_name()), b});
    }
  }
}

SyncProcess::~SyncProcess() {}

void SyncProcess::add_in(Bus* b) {
  ins.push_back(b);
  if (b->is_named()) {
    ins_map.insert({b->get_name(), b});
  }
}

void SyncProcess::add_out(Bus* b) {
  outs.push_back(b);
  if (b->is_named()) {
    outs_map.insert({b->get_name(), b});
  }
}

Bus* SyncProcess::get_in(int k) {
  return ins[k];
}
Bus* SyncProcess::get_in(Name n) {
  return ins_map[n];
}

Bus* SyncProcess::get_out(int k) {
  return outs[k];
}

Bus* SyncProcess::get_out(Name n) {
  return outs_map[n];
}

vector<Bus*> SyncProcess::get_ins() {
  return ins;
}

vector<Bus*> SyncProcess::get_outs() {
  return outs;
}

SyncComponent::SyncComponent(Name name,
			     Processes procs,
			     Busses ins,
			     Busses outs)
  :name{name} {

}


Bus::Bus() {}
Bus::Bus(Name name)
  :name{static_cast<Name>(name)}, named{true} {}

void Bus::step() {  _out = _in;
  _in = 0;
  // For now, just clear the value of bus to be written to in
  // next iteration in order to preserve network invariants
  //vals[Bus::offseta] = 0;
  // TODO: Benchmark handling of bus values
}

int Bus::read(){
  return _out;
  //return vals[Bus::offseta];
}

void Bus::write(int v) {
  _in = v;
  //vals[Bus::offsetb] = v;
}

Name Bus::get_name() {
   return name;
 }

bool Bus::is_named() {
   return named;
 }

void Bus::assign_to(Bus** b) {
  *b = this;
}

// int Bus::offseta = 0;
// int Bus::offsetb = 1;
// void Bus::swapval() {
//   Bus::offseta = (Bus::offseta + 1) % 2;
//   Bus::offsetb = (Bus::offsetb + 1) % 2;
// }

void Bus::assign(Busses from, BussesPtrPtr to) {
  if (from.size() != to.size()) {
    std::cout << "Cannot assign busses. to,from not equal length." << std::endl;
    // TODO: how to handle error?
  }
  auto fromit = from.begin();
  auto toit = to.begin();
  auto fromit_end = from.end();

  while (fromit != fromit_end) {
    (*fromit)->assign_to(*toit);
    fromit++;
    toit++;
  }
}

void Bus::raw_assign(std::vector<Bus*> from,  Bus** to) {
  auto fromit = from.begin();
  auto fromit_end = from.end();
  while (fromit != fromit_end) {
    (*fromit)->assign_to(to);
    fromit++;
    to++;
  }
}

void Bus::assign(Busses from1, BussesPtrPtr to1, Busses from2, BussesPtrPtr to2) {
  Bus::assign(from1, to1);
  Bus::assign(from2, to2);
}

//
