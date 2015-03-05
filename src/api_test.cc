//

#include <iostream>

#include "sme.h"

//using namespace sme;

class MyProc : public SyncProcess {
public:
  MyProc (const std::string foo):SyncProcess(foo){}
public:
  void run() {
    std::cout << foo << std::endl;
  }
};


int main(void) {
  auto sp = MyProc("foo");
  auto sp2 = MyProc("bar");
  auto sp3 = MyProc("bax");
  auto sp4 = MyProc("baz");
  Run r = Run({&sp, &sp2});
  r.add_proc({&sp3});
  r.add_proc(&sp4);
  r.start();
  //sp.start();
}


/*

class Cons : public SyncProcess {
protected:
  return 0;
};

class Show : public SyncProcess {
protected:
  return 0;
};
$
class Add : public SyncProess {
protected:
  return 0;
};

Bus inbus0 = Bus();
Bus inbus1 = Bus();
Bus resbus = Bus();

SyncComponent adder = SyncComponent("Adder", new Intputs(), new Outputs(),
			    Processes(new Const(Inputs(), Outputs(inbus0)),
				      new Const(Inputs(), Outputs(inbus1)),
				      new Add(Inputs(inbus0, inbus1), Outputs(resbus)),
				      new Show(Inputs(resbus), Outputs())));
adder.run();


*/
