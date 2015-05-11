#include <iostream>
#include <chrono>
#include <thread>

#include "sme.h"
#include "threading.h"

class GenNode: public SyncProcess {
public:
GenNode(const std::string name, Busses inputs, Busses outputs, int val)
  :SyncProcess(name, inputs, outputs), val{val}, its{its} {
    Bus::assign(inputs, {&in});
    Bus::assign(outputs, {&out});
}

protected:
  void step() {
    std::cout << "Iterations " << val++ << std::endl;
    if(in->read() == val) {
        std::cout << "Iterations " << val++ << std::endl;
        out->write(val);
    }
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
if (val > 0) {
   out->write(val);
}
}

private:
  Bus* in;
  Bus* out;

};

class Sleeper: public SyncProcess {
public:
Sleeper(const std::string name, Busses inputs, Busses outputs)
   :SyncProcess(name, inputs, outputs) {
      Bus::assign(inputs, {&in});
      Bus::assign(outputs, {&out});
}
protected:
void step() {
  std::this_thread::sleep_for(std::chrono::seconds(1));
  int val = in->read();
  if (val > 0) {
   out->write(val);
}
}

private:
  Bus* in;
  Bus* out;
};


int main() {

Bus* bus = new Bus [12];
auto r = ThreadedRun(10, 0);
r.add_proc(new GenNode("gennode", {&bus[11]}, {&bus[0]}, 1));
r.add_proc(new Sleeper("node0", {&bus[0]}, {&bus[1]}));
r.add_proc(new Node("node1", {&bus[1]}, {&bus[2]}));
r.add_proc(new Node("node2", {&bus[2]}, {&bus[3]}));
r.add_proc(new Node("node3", {&bus[3]}, {&bus[4]}));
r.add_proc(new Node("node4", {&bus[4]}, {&bus[5]}));
r.add_proc(new Node("node5", {&bus[5]}, {&bus[6]}));
r.add_proc(new Node("node6", {&bus[6]}, {&bus[7]}));
r.add_proc(new Node("node7", {&bus[7]}, {&bus[8]}));
r.add_proc(new Node("node8", {&bus[8]}, {&bus[9]}));
r.add_proc(new Node("node9", {&bus[9]}, {&bus[10]}));
r.add_proc(new Sleeper("node10", {&bus[10]}, {&bus[11]}));

r.start();

return 0;

}

