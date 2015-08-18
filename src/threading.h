// -*- mode: c++ -*-

#ifndef CPPSME_THREADING_H_
#define CPPSME_THREADING_H_

#include <thread>

#include "sme.h"
#include "runner.h"
#include "bqueue.h"
#include "cqueue.h"

class ThreadedRun : public Runner {
public:
  ThreadedRun(int steps, int threads);
  template <class T>
  void start();

private:
  template <class T>
  static void instance(T*, int);

  unsigned int threads = 1;
};

#endif //CPPSME_THREADING_H_
