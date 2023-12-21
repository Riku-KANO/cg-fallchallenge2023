#pragma once
#include <chrono>
#include "global.h"
#include "Property.h"
#include "Game.h"

class App {
public:
  App() {}
  App(const Property &_props) : props(_props) {}
  void init();
  void run();

private:
  Property props;
  Game game;
  clock_t start_time;
};