#include <iostream>
#include <string>

#include "sme.h"

using std::string;
using std::cout;
using std::endl;

class Const : public SyncProcess {
public:
  Const (const std::string name, Busses inputs, Busses outputs, unsigned value)
    :SyncProcess(name, inputs, outputs), value{value} {
    out1 = get_out(0);
  }

protected:
  void step() {
    out1->write(value);
  }

private:
  unsigned value;
  Bus* out1;
};

class Show: public SyncProcess {
public:
  Show(const std::string name, Busses inputs, Busses outputs)
    :SyncProcess(name, inputs, outputs) {
    in1 = get_in(0);
  }

protected:
  void step() {
    int val = in1->read();
    std::cout << val << std::endl;
  }

private:
  Bus* in1;
};

class Add:public SyncProcess {
public:
  Add(const string name, Busses inputs, Busses outputs)
    :SyncProcess(name, inputs, outputs)
  {
    in0 = get_in(0);
    in1 = get_in(1);
    out0 = get_out(0);
  }

protected:
  void step() {
    out0->write(in0->read() + in1->read());
  }

private:
  Bus* in0;
  Bus* in1;
  Bus* out0;
};


void adder() {
  auto in1 = Bus();
  auto in2 = Bus();
  auto res = Bus();

  auto r = Run(10);
  r.add_proc({new Const("const", {}, {&in1}, 2),
	new Const("const", {}, {&in2}, 4),
	new Add("add", {&in1, &in2}, {&res}),
	new Show("print", {&res}, {})});
  r.start();
}
