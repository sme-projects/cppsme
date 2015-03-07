#include <string>
#include <iostream>

#include <unistd.h>
#include <ncurses.h>

#include "sme.h"

using std::string;
using std::cout;
using std::endl;

int point = 0;
WINDOW* win;

#define add_point() point++;mvaddstr(60, 3, "Point: ");
#define sub_point() point--;mvaddstr(60, 3, "Point: ");

class RightCar : public SyncProcess {
private:
  int y;
  int x = 0;
  int delay;

  Bus* frogch;

public:
  RightCar(const string name, Busses ins, Busses outs, int y, int delay)
    :SyncProcess(name, ins, outs), y{y}, delay{delay}
  {
    frogch = get_in(0);
  }

protected:
  void step() {
    // cout << name << "run" << endl;
    if (delay == 0) {
      go();
    } else {
      delay--;
    }
  }

private:
  void go() {
    int frog = frogch->read();
    x++;
    if(frog == y && x > 30 && x < 35){
      sub_point();
    }
    if(x == 70) {
      mvaddstr(y, x, "    ");
      x = 0;
    }
    mvaddstr(y, x, " >>>> ");
  }
};

class LeftCar : public SyncProcess {
private:
  int y;
  int x = 70;
  int delay;

  Bus* frogch;

public:
  LeftCar(const string name, Busses ins, Busses outs, int y, int delay)
    :SyncProcess(name, ins, outs), y{y}, delay{delay}
  {
    frogch = get_in(0);
  }

protected:
  void step() {
    //    cout << name << "run" << endl;
    if (delay == 0) {
      go();
    } else {
      delay--;
    }

  }

private:
  void go() {
    int frog = frogch->read();
    x--;
    //cout << x << endl;
    if(frog == y && x > 30 && x < 35){
      sub_point();
    }
    if(x == 0) {
      mvaddstr(y, x, "    ");
      x = 70;
    }
    mvaddstr(y, x, " <<<< ");
    //cout << 20 << endl;
  }
};

class Frog : public SyncProcess {
private:
  int x = 35;
  int dy = -1;
  int y = 14;

  Bus* frogch;
public:
  Frog(const string name, Busses ins, Busses outs)
    :SyncProcess(name, ins, outs)
  {
    frogch = get_out(0);
  }

protected:
  void step() {
    //    cout << name << "run" << endl;
    mvaddstr(y, x, " ");
    int k = getch();
    if(k == 32) {
      y = y + dy;
    }
    if(y == 7){
      add_point();
      y = 8;
      dy = 1;
    }
    if(y == 15) {
      add_point();
      y = 14;
      dy = -1;

    }
    frogch->write(y);

    mvaddstr(y, x, "@");
  }
};

class Wait : public SyncProcess {
private:
  unsigned wait;
public:
  Wait(const string name, Busses ins, Busses outs, unsigned wait)
    :SyncProcess(name, ins, outs), wait{wait}
  {}

protected:
  void step() {
    //    cout << name << "run" << endl;
    usleep(wait);
    wrefresh(win);
  }
};

void frogger() {

  win = initscr();
  noecho();
  nodelay(win, true);
  curs_set(0);

  auto fr = Bus(); // frog

  // Start sme
  Run r = Run(1000);
  r.add_proc({new Frog("frog", {}, {&fr}),
	new RightCar("car 1", {&fr}, {}, 10, 0),
	new LeftCar("car 2", {&fr}, {}, 12, 0),
	new RightCar("car 3", {&fr}, {}, 10, 20),
	new LeftCar("car 4", {&fr}, {}, 12, 30),
	new Wait("wait", {}, {}, 100000)});
  r.start();

}
