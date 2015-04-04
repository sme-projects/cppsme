// -*- mode: c++ -*-

#include <iostream>
#include <vector>
#include <initializer_list>
#include <set>

#ifndef CPPSME_SME_H_
#define CPPSME_SME_H_

using std::vector;
using std::set;

class Bus {
private:
  int _in = 0;
  int _out = 0;
  void assign_to(Bus**);
public:
  void step();
  int read();
  void write(int v);
  static void assign(std::initializer_list<Bus*>, std::initializer_list<Bus**>);
  static void assign(std::initializer_list<Bus*>, std::initializer_list<Bus**>,
		     std::initializer_list<Bus*>, std::initializer_list<Bus**>);
};

typedef std::initializer_list<Bus*> Busses;
typedef std::initializer_list<Bus**> BussesPtrPtr;


class SyncProcess {
public:
  SyncProcess(const std::string name, Busses ins, Busses outs);
  //void step();
  virtual void step() = 0;
  virtual ~SyncProcess();
  vector<Bus*> ins;
  vector<Bus*> outs;
  Bus* get_in(int);
  Bus* get_out(int);
  vector<Bus*> get_outs();
  vector<Bus*> get_ins();
protected:
    const std::string name;

};

class Run {
public:
  Run(int steps);
  void start();
  void add_proc(std::initializer_list<SyncProcess*>);
  void add_proc(SyncProcess*);
private:
  std::vector<SyncProcess*> procs;
  std::set<Bus*> busses;
  int steps;
};

#endif // CPPSME_SME_H_
