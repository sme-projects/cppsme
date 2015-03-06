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
  int _val = 0;
  int _out = 0;
public:
  void step();
  int read();
  void write(int v);
};

typedef std::initializer_list<Bus*> Busses;

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

//template <class T>
class Run {
public:
  //Run(std::initializer_list<SyncProcess*>);
  Run(int steps);
  void start();
  void add_proc(std::initializer_list<SyncProcess*>);
  void add_proc(SyncProcess*);
private:
  std::vector<SyncProcess*> procs;
  std::set<Bus*> busses;
  int steps;
};

/*template <class T>
class SyncManager {
public:
  SyncManager(std::initializer_list<T>);
  int start(int iterations);
  };*/


#endif // CPPSME_SME_H_
