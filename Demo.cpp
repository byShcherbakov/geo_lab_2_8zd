#include "Demo.h"
Demo::Demo(GLFWwindow* w):window(w){pointSize=std::min(pointSize,maxPointSize);rebuild();}
void Demo::rebuild(){
 models.clear();
 Model(*operations[])(GLFWwindow*)={task1,task2,task3,task4,task5_1,task5_2,task5_3_1,task6,task7,task8a,task8b,task8c};
 for(auto operation:operations){models.push_back(operation(window));models.back().load_shaders("vertex_shader.txt","fragment_shader.txt");}
}
void Demo::key(int k){
 if(k==GLFW_KEY_SPACE || k==GLFW_KEY_RIGHT)page=(page+1)%12;
 if(k==GLFW_KEY_BACKSPACE || k==GLFW_KEY_LEFT)page=(page+11)%12;
 if(k==GLFW_KEY_S)shading=(shading+1)%3;
 if(k==GLFW_KEY_A)antialias=!antialias;
 if(k==GLFW_KEY_UP)pointSize=std::min(maxPointSize,pointSize+4);
 if(k==GLFW_KEY_DOWN)pointSize=std::max(1.0f,pointSize-4);
 if(k==GLFW_KEY_M)pointSize=maxPointSize;
 if(k==GLFW_KEY_L)lineWidth=lineWidth>=12?1:lineWidth+1;
 if(k==GLFW_KEY_E)pendingExperiment=true;
 if(k==GLFW_KEY_R){++colorSeed;rebuild();}
 if(k>=GLFW_KEY_1 && k<=GLFW_KEY_8){const int map[]={0,1,2,3,4,7,8,9};page=map[k-GLFW_KEY_1];}
}
void Demo::runPendingExperiment(){if(pendingExperiment){experiment=measurePoints();pendingExperiment=false;}}
void Demo::frame(int,int,int fw,int fh,double,double,bool){
 glViewport(0,0,fw,fh);glClearColor(1,1,1,1);glClear(GL_COLOR_BUFFER_BIT);
 const bool colors=page>=4 && page<=8;
 const bool faces=page>=9;
 const bool split=faces||(colors&&shading==2);
 const int count=split?2:1;
 const float panelWidth=float(fw)/count;
 const int side=std::max(1,int(std::min(panelWidth-16.0f,float(fh)-16.0f)));
 for(int i=0;i<count;++i){
  const int x=int(i*panelWidth+(panelWidth-side)/2);
  const int y=(fh-side)/2;
  glViewport(x,y,side,side);
  RenderOptions options;options.smooth=colors&&(shading==1||(shading==2&&i==1));
  options.antialias=antialias;options.pointSize=faces?8.0f:pointSize;options.lineWidth=lineWidth;
  options.mirror=faces&&i==1;
  models[page].render(options);
 }
}