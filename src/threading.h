// -*- mode: c++ -*-

#ifndef CPPSME_THREADING_H_
#define CPPSME_THREADING_H_

#include <thread>

#include "sme.h"
#include "runner.h"
#include "bqueue.h"

class ThreadedRun : public Runner {
public:
  ThreadedRun(int steps, int threads);
  void start();
private:
  static void instance(BQueue*, int);

  unsigned int threads = 1;
};


#endif //CPPSME_THREADING_H_
