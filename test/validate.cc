#include <iostream>

#include "sme.h"

#include "get_env.h"

SME_TEST_DEFS

class Validator: public SyncProcess {
public:
  Validator(Name name, Busses inputs, Busses outputs)
    :SyncProcess(name, inputs, outputs) {
    Bus::assign(outputs, {&out});

  }
  void assign_busses() {
    ins = new Bus*[nodes];
    Bus::raw_assign(get_ins(), ins);
  }
protected:
  void step() {
    for (int i = 0; i < nodes; i++) {
      std::cout << "out " << ins[i]->read() << std::endl;
      if (ins[i]->read() != expected - 1) {
        std::cout << "Bus " << i << " doesn't hold expected value " << expected << std::endl;
      }
    }
    val = expected;
    expected++;
    out->write(val);
  }
private:
  int val = 0;
  int expected = 0;
  Bus** ins;
  Bus* out;
};

class PlusOne : public SyncProcess {
public:
  PlusOne(Name name, Busses inputs, Busses outputs)
    :SyncProcess(name, inputs, outputs) {
    Bus::assign(inputs, {&in});
    Bus::assign(outputs, {&out});
  }

protected:
  void step() {
    int inv = in->read();
    std::cout << "in " << inv << std::endl;
    out->write(inv + 1);
  }
private:
  Bus* in;
  Bus* out;
};

int main() {
  SME_TEST_GET_ENV

  Bus* bus = new Bus[nodes];
  Bus single = Bus();
  auto r = ThreadedRun(iterations, 0);
  auto p = new Validator("vaidator", {}, {&single});
  for (int i = 0; i < nodes; i++) {
    p->add_in(&bus[i]);
  }
  p->assign_busses();
  r.add_proc(p);
  for (int i = 0; i < nodes; i++) {
    r.add_proc(new PlusOne("plusone", {&single}, {&bus[i]}));
  }

  //r.start();
  if(test_cqueue)
    r.start<CQueue>();
  if(test_bqueue)
    r.start<BQueue>();

}
