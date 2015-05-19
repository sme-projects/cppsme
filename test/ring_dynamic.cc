#include <iostream>

#include "sme.h"
#include "threading.h"
#include "get_env.h"
#include "cqueue.h"
#include "bqueue.h"

class GenNode: public SyncProcess {
public:
GenNode(const std::string name, Busses inputs, Busses outputs, int val)
  :SyncProcess(name, inputs, outputs), val{val}, its{its} {
    Bus::assign(inputs, {&in});
    Bus::assign(outputs, {&out});
}

protected:
  void step() {
val = in->read();
//if( val > 0) {
//std::cout << val << std::endl;
out->write(++val);
//} else {
//out->write(1); 
//]}
    //std::cout << "Iterations " << val++ << std::endl;
    //if(in->read() == val) {
        //std::cout << "Iterations " << val++ << std::endl;
        //out->write(val);
    //}
  }

private:
  int val;
  Bus* in;
  Bus* out;
  int first = 1;
  int its;
  int cits = 0;
};

class Node: public SyncProcess {
public:
Node(const std::string name, Busses inputs, Busses outputs)
   :SyncProcess(name, inputs, outputs) {
      Bus::assign(inputs, {&in});
      Bus::assign(outputs, {&out});
}
protected:
void step() {
int val = in->read();
//if (val > 0) {
   out->write(++val);
//}
}

private:
  Bus* in;
  Bus* out;

};

int main() {

  //int nodes=100000;
  //int iterations=50000;

  SME_TEST_DEFS;
  SME_TEST_GET_ENV;

  Bus* bus = new Bus[nodes];
  auto r = ThreadedRun(iterations, 0);
  r.add_proc(new GenNode("gennode", {&bus[nodes-1]}, {&bus[0]}, 1));
  for(int i = 0; i < nodes - 1; i++) {
    r.add_proc(new Node("node", {&bus[i]}, {&bus[i+1]}));
  }
  if(test_cqueue)
    r.start<CQueue>();
  if(test_bqueue)
    r.start<BQueue>();

return 0;

}
