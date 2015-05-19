// -*- mode: c++ -*-

#include <iostream>
#include <vector>
#include <string>
#include <initializer_list>
#include <set>
#include <map>

#ifndef CPPSME_SME_H_
#define CPPSME_SME_H_

// Macros for reducing boilerplate related to defining and retrieving busses
// May the keepers of pretty code punish me hard
#define GET_MACRO(_1, _2, ...) __VA_ARGS__
#define SME_OBUS_1(b) b = get_out(#b)
#define SME_OBUS_2(b, n) n = get_out(#b)
#define SME_OBUS(...) GET_MACRO(__VA_ARGS__, SME_OBUS_2, SME_OBUS_1)(__VA_ARGS__)
#define SME_IBUS_1(b) b = get_out(#b)
#define SME_IBUS_2(b, n) n = get_out(#b)
#define SME_IBUS(...) GET_MACRO(__VA_ARGS__, SME_IBUS_2, SME_IBUS_1)(__VA_ARGS__)
#define SME_MKBUS(n) auto n = Bus(#n)
#define SME_BDEC(n) Bus* n

using std::vector;
using std::set;
using std::map;

typedef const std::string Name;


class Bus {
private:
  int vals[2] = {0,0};
  static int offseta;
  static int offsetb;

  int _in = 0;
  int _out = 1;
  Name name;
  bool named = false;

  void assign_to(Bus**);

public:
  Bus();
  Bus(Name);
  void step();
  int read();
  void write(int v);
  Name get_name();
  bool is_named();
  static void assign(std::initializer_list<Bus*>, std::initializer_list<Bus**>);
  static void raw_assign(std::vector<Bus*>,  Bus**);
  static void assign(std::initializer_list<Bus*>, std::initializer_list<Bus**>,
		     std::initializer_list<Bus*>, std::initializer_list<Bus**>);
  static void initval();
  static void swapval();

};

typedef std::initializer_list<Bus*> Busses;
typedef std::initializer_list<Bus**> BussesPtrPtr;

// Class to serve as common ancestor of Sync* classes
class SyncObject {
public:
  virtual vector<Bus*> get_outs()=0;
  virtual vector<Bus*> get_ins()=0;
};

typedef std::initializer_list<SyncObject*> Objects;

class SyncProcess : public SyncObject {
public:
  SyncProcess(Name name, Busses ins, Busses outs);
  virtual void step() = 0;
  virtual ~SyncProcess();
  vector<Bus*> ins;
  vector<Bus*> outs;

  // FIXME: Do we need these?
  map<Name, Bus*> ins_map;
  map<Name, Bus*> outs_map;
  void add_in(Bus*);
  void add_out(Bus*);
  Bus* get_in(int);
  Bus* get_in(Name);
  Bus* get_out(int);
  Bus* get_out(Name);
  vector<Bus*> get_outs();
  vector<Bus*> get_ins();
protected:
    const std::string name;
};

typedef std::initializer_list<SyncObject*> Processes;

class SyncComponent : public SyncObject {
private:
  const std::string name;
public:
  SyncComponent(Name name, Processes procs, Busses ins, Busses outs);
  vector<SyncObject*> procs;
};


class Run {
public:
  Run(int steps);
  void start();
  void add_proc(std::initializer_list<SyncObject*>);
  void add_proc(SyncObject*);
private:
  std::vector<SyncProcess*> procs;
  std::set<Bus*> busses;
  int steps;

  void append_procs(SyncProcess* p);
  void append_procs(vector<SyncComponent*> proc);
};

#endif // CPPSME_SME_H_
