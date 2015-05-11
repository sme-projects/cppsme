#include <iostream>
#include <string>
#include <vector>
#include <array>

#include "sme.h"

using std::vector;
using std::array;
using std::string;
using std::cout;
using std::endl;

class UCode : public SyncProcess {
private:
  Bus* regsrc1;
  Bus* regsrc2;
  Bus* regdest;
  Bus* regwrt;
  Bus* aluop;

public:
  UCode(const string name, Busses ins, Busses outs)
    :SyncProcess(name, ins, outs)
  {
    Bus::assign(outs, {&regsrc1, &regsrc2, &regdest, &regwrt, &aluop});
  }

protected:
  void step() {
    regsrc1->write(0);
    regsrc2->write(1);
    regdest->write(1);
    regwrt->write(1);
    aluop->write(0);
  }
};

class ALU : public SyncProcess {
private:
  Bus* src_a;
  Bus* src_b;
  Bus* op;
  Bus* result;

public:
  ALU(const string name, Busses ins, Busses outs)
    :SyncProcess(name, ins, outs)
  {
    Bus::assign(ins, {&src_a, &src_b, &op},
		outs, {&result});
  }

protected:
  void step() {
    int res = 0;
    int a = src_a->read();
    int b = src_b->read();
    switch (op->read()) {
    case 0:
      res = a + b;
      break;
    case 1:
      res = a - b;
      break;
    case 2:
      res = a * b;
      break;
    case 3:
      res = a / b;
      break;
    }
    result->write(res);
    cout << res << endl;
  }
};

class Register : public SyncProcess {
private:
  std::array<int, 32> regs;
  // outputs
  Bus* out1;
  Bus* out2;
  //inputs
  Bus* src1;
  Bus* src2;
  Bus* dest;
  Bus* wrt;
  Bus* input;

public:
  Register(const string name, Busses ins, Busses outs):
    SyncProcess(name, ins, outs)
  {
    Bus::assign(outs, {&out1, &out2},
		ins, {&src1, &src2, &dest, &wrt, &input});

    regs.fill(1);
  }

protected:
  void step(){
    out1->write(regs[src1->read()]);
    out2->write(regs[src2->read()]);
    if(wrt->read() > 0){
      regs[dest->read()] = input->read();
    }
  }

};


void simplecpu() {
 auto ssel1 = Bus();
 auto ssel2 = Bus();
 auto dsel = Bus();
 auto wrt = Bus();
 auto dbus = Bus();
 auto sbus1 = Bus();
 auto sbus2 = Bus();
 auto inst = Bus();

 auto r = Run(100);
 r.add_proc({new Register("regsiter",
       {&ssel1, &ssel2, &dsel, &wrt, &dbus},
       {&sbus1, &sbus2}),
       new ALU("ALU", {&sbus1, &sbus2, &inst}, {&dbus}),
       new UCode("uCode", {}, {&ssel1, &ssel2, &dsel, &wrt, &inst})});
 r.start();
}
