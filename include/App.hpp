#pragma once
#include <chrono>
#include "global.hpp"
#include "Property.hpp"
#include "Game.hpp"

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