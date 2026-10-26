#pragma once
#include "Experiment.h"
#include "tasks.h"
class Demo {
 GLFWwindow* window;
 std::vector<Model> models;
 int page=0;
 int shading=2;
 float pointSize=24,lineWidth=4;
 bool antialias=true;
 bool pendingExperiment=false;
 PointExperiment experiment;
 void rebuild();
public:
 explicit Demo(GLFWwindow* w);
 void key(int key);
 void runPendingExperiment();
 void frame(int width,int height,int framebufferWidth,int framebufferHeight,double mouseX,double mouseY,bool click);
 void showPage(int value){page=value;}
};