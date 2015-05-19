#include <iostream>
#include <string>

#include "sme.h"
#include "threading.h"
#include "cqueue.h"

using std::string;
using std::cout;
using std::endl;

class Const : public SyncProcess {
public:
  Const (const std::string name, Busses inputs, Busses outputs, unsigned value)
    :SyncProcess(name, inputs, outputs), value{value} {
    Bus::assign(outputs, {&out1});
  }

protected:
  void step() {
    //std::cout << "foo" << out1->get_name() << std::endl;
    out1->write(value);
    //cout << "write " << value << endl;
  }

private:
  unsigned value;
  Bus* out1;
};

class Show: public SyncProcess {
public:
  Show(const std::string name, Busses inputs, Busses outputs)
    :SyncProcess(name, inputs, outputs) {
    Bus::assign(inputs, {&res});
  }

protected:
  void step() {
    int val = res->read();
    std::cout << val << std::endl;
  }

private:
  Bus* res;
};

class Add:public SyncProcess {
public:
  Add(const string name, Busses inputs, Busses outputs)
    :SyncProcess(name, inputs, outputs)
  {
    Bus::assign(inputs, {&in0, &in1});
    Bus::assign(outputs, {&out0});
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

  auto r = ThreadedRun(10, 0);
  r.add_proc({new Const("const", {}, {&in1}, 2),
	new Const("const", {}, {&in2}, 4),
	new Add("add", {&in1, &in2}, {&res}),
	new Show("print", {&res}, {})});
  r.start<CQueue>();
}
