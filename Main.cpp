#include "Demo.h"
#include "func.h"
#include <random>
int WinWidth=1280,WinHeight=900;
float maxPointSize=64;
std::uint32_t colorSeed=std::random_device{}();
int main(){
 GLFWwindow* window=InitAll(1280,900,false);
 if(!window){EndAll();return 1;}
 int status=0;
 try{
  Demo demo(window);
  glfwSetWindowTitle(window,"OpenGL: variant 2 — all tasks 1–8");
  glfwSetWindowUserPointer(window,&demo);
  glfwSetKeyCallback(window,[](GLFWwindow* w,int key,int,int action,int){
   if(action!=GLFW_PRESS&&action!=GLFW_REPEAT)return;
   if(key==GLFW_KEY_ESCAPE){glfwSetWindowShouldClose(w,GLFW_TRUE);return;}
   if(action==GLFW_REPEAT&&key!=GLFW_KEY_UP&&key!=GLFW_KEY_DOWN)return;
   static_cast<Demo*>(glfwGetWindowUserPointer(w))->key(key);
  });
  bool previousClick=false;
  while(!glfwWindowShouldClose(window)){
   glfwPollEvents();
   int width,height;glfwGetWindowSize(window,&width,&height);
   glfwGetFramebufferSize(window,&WinWidth,&WinHeight);
   if(width<=0||height<=0||WinWidth<=0||WinHeight<=0){glfwWaitEvents();continue;}
   double mx,my;glfwGetCursorPos(window,&mx,&my);
   bool now=glfwGetMouseButton(window,GLFW_MOUSE_BUTTON_LEFT)==GLFW_PRESS;
   demo.frame(width,height,WinWidth,WinHeight,mx,my,now&&!previousClick);previousClick=now;
   glfwSwapBuffers(window);
   demo.runPendingExperiment();
  }
 }catch(const std::exception& e){std::cerr<<e.what()<<'\n';status=1;}
 glfwDestroyWindow(window);EndAll();return status;
}
