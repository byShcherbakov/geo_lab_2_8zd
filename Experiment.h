#pragma once
#include "Model.h"
struct PointExperiment {
 bool completed=false;
 bool reachedDriverLimit=false;
 bool saved=false;
 float maximum=0;
 std::string message;
};
PointExperiment measurePoints();
