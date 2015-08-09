#include <iostream>

#include "sme.h"

class Sender: public SyncProcess {
private:
  Bus* b1;
  Bus* b2;
  Bus* b3;

public:
  Sender (Name name, Busses ins, Busses outs)
    :SyncProcess(name, ins, outs) {
    Bus::assign(outs, {&b1, &b2, &b3});
  }

protected:
  void step() {
    b1->write(1);
    b2->write(2);
    b3->write(3);
  }
};

class Receiver: public SyncProcess {
private:
  Bus* b1;
  Bus* b2;
  Bus* b3;

public:
  Receiver(Name name, Busses ins, Busses outs)
    :SyncProcess(name, ins, outs) {
    Bus::assign(ins, {&b1, &b2, &b3});
  }

protected:
  void step() {
    std::cout << b1->read() << b2->read() << b3->read() << std::endl;
  }
};

int main() {
  DelayedBus b1 = DelayedBus(1);
  DelayedBus b2 = DelayedBus(4);
  DelayedBus b3 = DelayedBus(8);

  auto r = Runner(10);
  r.add_proc(new Sender("loop", {},  {&b1, &b2, &b3}));
  r.add_proc(new Receiver("recv", {&b1, &b2, &b3}, {}));
  r.start();

  return 0;
}
