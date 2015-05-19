nodes = 10
iterations = 500000

print("""#include <iostream>

#include "sme.h"
#include "threading.h"
#include "cqueue.h"

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
""")

# Create busses
print("Bus* bus = new Bus [%d];" % nodes);

print("auto r = ThreadedRun(%d, 0);" %iterations)

print("r.add_proc(new GenNode(\"gennode\", {&bus[%d]}, {&bus[%d]}, 1));" % (nodes - 1, 4))
for i in range(nodes-1):
    print("r.add_proc(new Node(\"node%d\", {&bus[%d]}, {&bus[%d]}));" % (i, i, i+1))

print("""
r.start<CQueue>();

return 0;

}
""")
